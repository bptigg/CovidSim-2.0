#include "Button.h"

Button::Button(const glm::vec2& location, const glm::vec2& size, Layer* layer, bool menu)
	:scriptable_object(location, size, layer)
{
	m_state = State::None;
	m_default_func = true;
	
	button_func = BIND_BUTTON_FN(Button::m_default_function);

	m_menu_object = menu;
	m_set_up = false;
}

Button::~Button()
{
}

void Button::update()
{

}

void Button::render()
{
	switch (m_state)
	{
	case Button::State::None:
		Renderer::draw_rectangle_color(m_location, m_size, { 0.0f,1.0f,0.5f,1.0f }, 1, m_menu_object);
		break;
	case Button::State::Hover:
		Renderer::draw_rectangle_color(m_location, m_size, { 0.0f,1.0f,0.5f,1.0f },1, m_menu_object);
		Renderer::draw_box(m_location, m_size, 5.0f, { 1.0f,0.0f,0.5f, 1.0f },2, m_menu_object);
		break;
	case Button::State::Press:
		Renderer::draw_rectangle_color(m_location, m_size, { 0.5f,0.0f,1.0f,1.0f },1, m_menu_object);
		Renderer::draw_box(m_location, m_size, 5.0f, { 1.0f,0.0f,0.5f, 1.0f },2, m_menu_object);
		break;
	default:
		break;
	}
}

void Button::event_callback(Events::Event& e)
{
	Events::Event_Dispatcher dispatcher(e);
	switch (e.Get_Event_Type())
	{
	case Events::Event_Type::Mouse_Moved:
		dispatcher.Dispatch<Events::Mouse_Moved_Event>(BIND_EVENT_FN(Button::on_mouse_move));
		break;
	case Events::Event_Type::Mouse_Button_Pressed:
		dispatcher.Dispatch<Events::Mouse_Button_Pressed_Event>(BIND_EVENT_FN(Button::on_mouse_click));
		break;
	default:
		break;
	}
}

void Button::update_position(const float& zoom, const glm::vec2& camera_pos, const glm::mat4& camera_mat)
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

void Button::m_default_function()
{
	Log::info("Button pressed");
}

bool Button::on_mouse_move(Events::Mouse_Moved_Event& e)
{
	if (m_zoom == 1)
	{
		std::pair<float, float> loc = std::make_pair(e.GetX(), e.GetY());
		
		float x_scaler = e.get_width() / 1280;
		float y_scaler = e.get_height() / 720;

		float mouse_x = loc.first - (e.get_width() / 2.0f) + m_camera_position.x;
		float mouse_y = (e.get_height() / 2.0f - loc.second) + m_camera_position.y;
		
		glm::vec4 mouse_loc = glm::vec4(mouse_x, mouse_y, 1.0f, 1.0f);
		glm::vec4 mouse_loc_new = (mouse_loc * m_camera_matrix) / glm::vec4(x_scaler, y_scaler, 1.0f, 1.0f);

		glm::vec4 vec1 = glm::vec4(m_collison_box.lower_bound, 1.0f, 1.0f) * m_camera_matrix;
		glm::vec4 vec2 = glm::vec4(m_collison_box.upper_bound, 1.0f, 1.0f) * m_camera_matrix;

		if (mouse_loc.x >= vec1.x && mouse_loc.x <= vec2.x)
		{
			if (mouse_loc.y >= vec1.y && mouse_loc.y <= vec2.y)
			{
				m_state = State::Hover;
				return false;
			}
		}
	}
	m_state = State::None;
	return false;
}

bool Button::on_mouse_click(Events::Mouse_Button_Pressed_Event& e)
{
	if (m_state == State::Hover)
	{
		m_state = State::Press;
		button_func();
		return true;
	}
	return false;
}
