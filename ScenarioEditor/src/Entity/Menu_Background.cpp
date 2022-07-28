#include "Menu_Background.h"

Menu_Background::Menu_Background(glm::vec2 pos, glm::vec2 size, Layer* layer, glm::vec4 colour, const std::function<void()> exit_Func, unsigned int rendering_layer, int id)
	:scriptable_object(pos, size, layer, id), m_background_color(colour), m_exit_func(exit_Func)
{
	m_menu_object = true;
	m_rendering_layer = rendering_layer;
	m_type = entity_type::BACKGROUND;
}

Menu_Background::~Menu_Background()
{
}

void Menu_Background::render()
{
	Renderer::draw_rectangle_color(m_location, m_size, m_background_color, m_rendering_layer, true);
}

void Menu_Background::update()
{
	render();
}

void Menu_Background::event_callback(Events::Event& e)
{
	Events::Event_Dispatcher dispatcher(e);
	switch (e.Get_Event_Type())
	{
	case Events::Event_Type::Key_Pressed:
		dispatcher.Dispatch<Events::Key_Pressed_Event>(BIND_EVENT_FN(Menu_Background::on_keyboard_press));
		break;
	case Events::Event_Type::Mouse_Button_Pressed:
		dispatcher.Dispatch<Events::Mouse_Button_Pressed_Event>(BIND_EVENT_FN(Menu_Background::on_mouse_click));
		break;
	default:
		break;
	}
}

void Menu_Background::update_position(const float& zoom, const glm::vec2& camera_pos, const glm::mat4& camera_mat)
{
	if (!m_set_up)
	{
		m_zoom = zoom;
		m_camera_position = camera_pos;
		m_camera_matrix = camera_mat;
	}

	if (m_menu_object)
	{
		m_set_up = true;
	}
}

bool Menu_Background::is_mouse_over(float width, float height, float x, float y)
{
	glm::vec2 loc = { x, y };
	//return !(loc.x - width / 2.0f < m_location.x - m_size.x / 2.0f || loc.x - width / 2.0f > m_location.x + m_size.x / 2.0f || height / 2.0f - loc.y < m_location.y - m_size.y / 2.0f || height / 2.0f - loc.y > m_location.y + m_size.y / 2.0f);
	
	float x_scaler = width / 1280;
	float y_scaler = height / 720;
	
	float mouse_x = loc.x - (width / 2.0f);
	float mouse_y = (height / 2.0f - loc.y);
	
	glm::vec4 mouse_loc = glm::vec4(mouse_x, mouse_y, 1.0f, 1.0f);
	glm::vec4 mouse_loc_new = (mouse_loc * m_camera_matrix) / glm::vec4(x_scaler, y_scaler, 1.0f, 1.0f);

	glm::vec4 vec1 = glm::vec4(m_collison_box.lower_bound, 1.0f, 1.0f) * m_camera_matrix;
	glm::vec4 vec2 = glm::vec4(m_collison_box.upper_bound, 1.0f, 1.0f) * m_camera_matrix;

	if (mouse_loc_new.x >= vec1.x && mouse_loc_new.x <= vec2.x)
	{
		if (mouse_loc_new.y >= vec1.y && mouse_loc_new.y <= vec2.y)
		{
			return true;
		}
	}
	return false;
}

bool Menu_Background::on_mouse_click(Events::Mouse_Button_Pressed_Event& e)
{
	if (Input::Is_MouseButton_Pressed(CS_MOUSE_BUTTON_LEFT) && !is_mouse_over(e.get_width(), e.get_height(), e.get_x(), e.get_y()))
	{
		if (m_exit_func != nullptr)
		{
			m_exit_func();
			return true;
		}
	}
	return false;
}

bool Menu_Background::on_keyboard_press(Events::Key_Pressed_Event& e)
{
	if (Input::Is_Key_Pressed(CS_KEY_ESCAPE))
	{
		m_exit_func();
		return true;
	}
	return false;
}
