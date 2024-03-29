#include "Button.h"

Button::Button(const glm::vec2& location, const glm::vec2& size, Layer* layer, bool menu, int id)
	:scriptable_object(location, size, layer, id)
{
	m_state = State::None;
	m_default_func = true;
	
	button_func = BIND_BUTTON_FN(Button::m_default_function);
	right_button_func = BIND_BUTTON_FN(Button::m_default_function);

	m_menu_object = menu;
	m_set_up = false;
	m_render = true;

	rendering_layer = 1;

	m_text = Text("", location, 20.f, { 1.0f, 1.0f, 1.0f, 1.0f }, true);

	m_type = entity_type::BUTTON;

	action_needed = false;
}

Button::Button(const std::string text, const glm::vec2& location, const glm::vec2& size, Layer* layer, bool menu, float scale, int id)
	:scriptable_object(location, size, layer, id)
{
	m_state = State::None;
	m_default_func = true;

	button_func = BIND_BUTTON_FN(Button::m_default_function);
	right_button_func = BIND_BUTTON_FN(Button::m_default_function);

	m_menu_object = menu;
	m_set_up = false;
	m_render = true;

	rendering_layer = 1;

	m_text = Text(text, location, scale, { 1.0f, 1.0f, 1.0f, 1.0f }, true);
	m_type = entity_type::BUTTON;

	action_needed = false;
}

Button::~Button()
{
}

void Button::update()
{
	if (!m_render)
	{
		return;
	}

	if (m_state == State::Hover && !m_hover)
	{
		if (is_mouse_over() == false)
		{
			m_state = State::None;
		}
	}

	render();
	if (m_text.m_text != "")
	{
		m_text.render(0, 0, rendering_layer + 1, m_menu_object);
	}
}

void Button::render()
{
	switch (m_state)
	{
	case Button::State::None:
		if (m_hover && m_Layer->get_selected() != m_object_id)
		{
			Renderer::draw_rectangle_color(m_location, m_size, base_colour, rendering_layer, m_menu_object);
			Renderer::draw_box(m_location, m_size, 2.0f, box_colour, rendering_layer + 1, m_menu_object);
		}
		else
		{
			Renderer::draw_rectangle_color(m_location, m_size, base_colour, rendering_layer, m_menu_object);
		}
		break;
	case Button::State::Hover:
		Renderer::draw_rectangle_color(m_location, m_size, base_colour, rendering_layer, m_menu_object);
		Renderer::draw_box(m_location, m_size, 2.0f, box_colour, rendering_layer + 1, m_menu_object);
		break;
	case Button::State::Press:
		Renderer::draw_rectangle_color(m_location, m_size, selected_colour, rendering_layer, m_menu_object);
		Renderer::draw_box(m_location, m_size, 2.0f, box_colour, rendering_layer + 1, m_menu_object);
		break;
	default:
		break;
	}

	//editor specific
	if (action_needed && m_state == Button::State::None)
	{
		Renderer::draw_box(m_location, m_size, 2.0f, { 1.0f, 0.0f, 0.0f, 1.0f }, rendering_layer + 1, m_menu_object);
	}
}

void Button::event_callback(Events::Event& e)
{
	if (!m_render)
	{
		return;
	}

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

void Button::change_position(const glm::vec2& pos)
{
	m_location = pos;
	m_text.update_location(pos);

	m_collison_box.lower_bound = { m_location.x - (m_size.x / 2.0f), m_location.y - (m_size.y / 2.0f) };
	m_collison_box.upper_bound = { m_location.x + (m_size.x / 2.0f), m_location.y + (m_size.y / 2.0f) };
	m_collison_box.width = 2.0f;
}

void Button::m_default_function()
{
	Log::info("Button pressed");
}

bool Button::on_mouse_move(Events::Mouse_Moved_Event& e)
{
	glm::vec4 mouse_loc_new;
	{
		std::pair<float, float> loc = std::make_pair(e.GetX(), e.GetY());

		float x_scaler = e.get_width() / 1280;
		float y_scaler = e.get_height() / 720;

		float mouse_x, mouse_y;


		if (!m_menu_object)
		{
			mouse_x = (loc.first - (e.get_width() / 2.0f) + m_camera_position.x) * m_zoom;
			mouse_y = ((e.get_height() / 2.0f - loc.second) + m_camera_position.y) * m_zoom;
			glm::vec4 mouse_loc = glm::vec4(mouse_x, mouse_y, 1.0f, 1.0f);
			mouse_loc_new = (mouse_loc * m_camera_matrix * m_zoom);
		}
		else
		{
			mouse_x = loc.first - (e.get_width() / 2.0f);
			mouse_y = (e.get_height() / 2.0f - loc.second);
			glm::vec4 mouse_loc = glm::vec4(mouse_x, mouse_y, 1.0f, 1.0f);
			mouse_loc_new = (mouse_loc * m_camera_matrix * m_zoom) / glm::vec4(x_scaler, y_scaler, 1.0f, 1.0f);
		}

	}

	glm::vec4 vec1 = glm::vec4(m_collison_box.lower_bound, 1.0f, 1.0f) * (m_camera_matrix * m_zoom);
	glm::vec4 vec2 = glm::vec4(m_collison_box.upper_bound, 1.0f, 1.0f) * (m_camera_matrix * m_zoom);

	if (mouse_loc_new.x >= vec1.x && mouse_loc_new.x <= vec2.x)
	{
		if (mouse_loc_new.y >= vec1.y && mouse_loc_new.y <= vec2.y)
		{
			m_state = State::Hover;
			return false;
		}
	}
	
	m_state = State::None;

	return false;
}

bool Button::on_mouse_click(Events::Mouse_Button_Pressed_Event& e)
{
	if (m_state == State::Hover)
	{
		m_Layer->Set_Text_Box(false);
		m_state = State::Press;
		if (m_persist)
		{
			m_hover = true;
		}
		else
		{
			m_hover = false;
		}
		m_Layer->set_selected(m_object_id);
		if (e.Get_Mouse_Button() == CS_MOUSE_BUTTON_LEFT)
		{
			button_func();
		}
		else if (e.Get_Mouse_Button() == CS_MOUSE_BUTTON_RIGHT)
		{
			right_button_func();
		}
		else
		{
			return false;
		}
		return true;
	}
	return false;
}

bool Button::is_mouse_over(bool position)
{
	glm::vec2 pos = Input::Get_Mouse_Position();
	std::pair<float, float> loc = std::make_pair(pos.x, pos.y);

	glm::vec4 mouse_loc_new;
	{
		std::pair<float, float> loc = std::make_pair(pos.x, pos.y);

		glm::vec2 size = Input::Get_Window_Size();

		float x_scaler = size.x / 1280;
		float y_scaler = size.y / 720;

		float mouse_x, mouse_y;


		if (!m_menu_object)
		{
			mouse_x = (loc.first - (size.x / 2.0f) + m_camera_position.x) * m_zoom;
			mouse_y = ((size.y / 2.0f - loc.second) + m_camera_position.y) * m_zoom;
			glm::vec4 mouse_loc = glm::vec4(mouse_x, mouse_y, 1.0f, 1.0f);
			mouse_loc_new = (mouse_loc * m_camera_matrix * m_zoom);
		}
		else
		{
			mouse_x = loc.first - (size.x / 2.0f);
			mouse_y = (size.y / 2.0f - loc.second);
			glm::vec4 mouse_loc = glm::vec4(mouse_x, mouse_y, 1.0f, 1.0f);
			mouse_loc_new = (mouse_loc * m_camera_matrix * m_zoom) / glm::vec4(x_scaler, y_scaler, 1.0f, 1.0f);
		}

	}

	glm::vec4 vec1 = glm::vec4(m_collison_box.lower_bound, 1.0f, 1.0f) * (m_camera_matrix * m_zoom);
	glm::vec4 vec2 = glm::vec4(m_collison_box.upper_bound, 1.0f, 1.0f) * (m_camera_matrix * m_zoom);

	if (mouse_loc_new.x >= vec1.x && mouse_loc_new.x <= vec2.x)
	{
		if (mouse_loc_new.y >= vec1.y && mouse_loc_new.y <= vec2.y)
		{
			//m_state = State::Hover;

			if (position)
			{
				float dist = 1 / (vec2.x - vec1.x);
				float x_dist = mouse_loc_new.x - vec1.x;
				if (x_dist != 0)
				{
					float x_rel = x_dist * dist;
					relative_position.x = x_rel;
				}
				else
				{
					relative_position.x = 0;
				}

				dist = 1 / (vec2.y - vec1.y);
				float y_dist = mouse_loc_new.y - vec1.y;
				if (y_dist != 0)
				{
					float y_rel = y_dist * dist;
					relative_position.y = y_rel;
				}
				else
				{
					relative_position.y = 0;
				}
			}

			return true;
		}
	}

	//m_state = State::None;

	return false;
}
