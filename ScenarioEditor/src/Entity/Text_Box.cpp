#include "Text_Box.h"

Text_Box::Text_Box(const glm::vec2& position, const glm::vec2& size, Layer* layer, bool int_only, unsigned int rendering_layer, bool centred_text)
    :scriptable_object(position, size, layer)
{
    m_over = false;
	m_selected = false;
	caps = false;

	x_offset = 0.0f;
	
	m_interger_only = int_only;
	m_is_integer_only = false;
	
	if (centred_text)
	{
		m_text = Text("", { position.x, position.y }, size.y * 1.5f, { 0.0f, 0.0f, 0.0f, 1.0f }, centred_text);
	}
	else
	{
		m_text = Text("", { position.x - (size.x / 2.0f) + 15.0f, position.y - (size.y / 2.0f) + 7.0f }, size.y * 1.5f, { 0.0f, 0.0f, 0.0f, 1.0f }, centred_text);
	}

	m_menu_object = true;
	m_set_up = false;

	m_rendering_layer = rendering_layer;

	m_type = entity_type::TEXT_BOX;
}

Text_Box::~Text_Box()
{
}

void Text_Box::update()
{
	if (!text.empty() && m_interger_only == true)
	{
		m_is_integer_only = true;
		std::string::const_iterator it = text.begin();
		while (it != text.end() && std::isdigit(*it)) ++it;

		if (it != text.end())
		{
			m_is_integer_only = false;
		}
	}

	render();
}

void Text_Box::render()
{
	if (m_interger_only == false || text.empty())
	{
		Renderer::draw_rectangle_color(m_location, m_size, { 1.0f, 1.0f, 1.0f, 1.0f }, m_rendering_layer, true);
		if (m_over)
		{
			Renderer::draw_box(m_location, { m_size.x + 12.0f, m_size.y + 12.0f }, 6.0f, { 0.2f, 0.2f, 0.2f, 1.0f }, m_rendering_layer + 1, true);
		}
		else
		{
			Renderer::draw_box(m_location, { m_size.x + 8.0f, m_size.y + 8.0f }, 4.0f, { 0.2f, 0.2f, 0.2f, 1.0f }, m_rendering_layer + 1, true);
		}
		if (!text.empty())
		{
			m_text.render(0, 0, m_rendering_layer + 2, true, &x_offset);
		}
	}
	else if (m_interger_only && m_is_integer_only)
	{
		Renderer::draw_rectangle_color(m_location, m_size, { 1.0f, 1.0f, 1.0f, 1.0f }, m_rendering_layer, true);
		if (m_over)
		{
			Renderer::draw_box(m_location, { m_size.x + 12.0f, m_size.y + 12.0f }, 6.0f, { 0.3125f, 0.78125f, 0.46875f, 1.0f }, m_rendering_layer + 1, true);
		}
		else
		{
			Renderer::draw_box(m_location, { m_size.x + 8.0f, m_size.y + 8.0f }, 4.0f, { 0.3125f, 0.78125f, 0.46875f, 1.0f }, m_rendering_layer + 1, true);
		}
		m_text.render(0, 0, m_rendering_layer + 2, true, &x_offset);
	}
	else if (m_interger_only && !m_is_integer_only)
	{
		Renderer::draw_rectangle_color(m_location, m_size, { 1.0f, 1.0f, 1.0f, 1.0f }, m_rendering_layer, true);
		if (m_over)
		{
			Renderer::draw_box(m_location, { m_size.x + 12.0f, m_size.y + 12.0f }, 6.0f, { 1.0f, 0.0f, 0.0f, 1.0f }, m_rendering_layer + 1, true);
		}
		else
		{
			Renderer::draw_box(m_location, { m_size.x + 8.0f, m_size.y + 8.0f }, 4.0f, { 1.0f, 0.0f, 0.0f, 1.0f }, m_rendering_layer + 1, true);
		}
		m_text.render(0, 0, m_rendering_layer + 2, true, &x_offset);
	}
}

void Text_Box::event_callback(Events::Event& e)
{
	Events::Event_Dispatcher dispatcher(e);
	switch (e.Get_Event_Type())
	{
	case Events::Event_Type::Mouse_Moved:
		dispatcher.Dispatch<Events::Mouse_Moved_Event>(BIND_EVENT_FN(Text_Box::on_mouse_move));
	case Events::Event_Type::Mouse_Button_Pressed:
		dispatcher.Dispatch<Events::Mouse_Button_Pressed_Event>(BIND_EVENT_FN(Text_Box::on_mouse_click));
	case Events::Event_Type::Key_Pressed:
		dispatcher.Dispatch<Events::Key_Pressed_Event>(BIND_EVENT_FN(Text_Box::on_keyboard_press));
	default:
		break;
	}
}

void Text_Box::update_position(const float& zoom, const glm::vec2& camera_pos, const glm::mat4& camera_mat)
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

bool Text_Box::on_mouse_move(Events::Mouse_Moved_Event& e)
{
	std::pair<float, float> loc = std::make_pair(e.GetX(), e.GetY());

	float x_scaler = e.get_width() / 1280;
	float y_scaler = e.get_height() / 720;
	
	float x = loc.first - (e.get_width() / 2.0f);
	float y = (e.get_height() / 2.0f - loc.second);
	glm::vec4 mouse_loc = glm::vec4(x, y, 1.0f, 1.0f);
	glm::vec4 mouse_loc_new = (mouse_loc * m_camera_matrix) / glm::vec4(x_scaler, y_scaler, 1.0f, 1.0f);

	glm::vec4 vec1 = glm::vec4(m_collison_box.lower_bound, 1.0f, 1.0f) * m_camera_matrix;
	glm::vec4 vec2 = glm::vec4(m_collison_box.upper_bound, 1.0f, 1.0f) * m_camera_matrix;

	if (mouse_loc_new.x >= vec1.x && mouse_loc_new.x <= vec2.x)
	{
		if (mouse_loc_new.y >= vec1.y && mouse_loc_new.y <= vec2.y)
		{
			m_over = true;
			return false;
		}
	}

	m_over = false;
	return false;
}

bool Text_Box::on_mouse_click(Events::Mouse_Button_Pressed_Event& e)
{
	if (m_over == true)
	{
		m_selected = true;
		return true;
	}
	m_selected = false;
	return false;
}

bool Text_Box::on_keyboard_press(Events::Key_Pressed_Event& e)
{
	if (m_selected == true)
	{
		if (x_offset < m_size.x - 30.0f)
		{
			key_to_char(e.Get_Key_Code());
		}
		else if (e.Get_Key_Code() == CS_KEY_BACKSPACE)
		{
			if (!text.empty())
			{
				text.pop_back();
			}
		}
		else if (e.Get_Key_Code() == CS_KEY_ENTER)
		{
			m_selected = false;
		}
		m_text.update_string(text);
		return true;
	}
    return false;
}

void Text_Box::key_to_char(uint32_t key)
{
	switch (key)
	{
	case CS_KEY_SPACE:
		text.append(" ");
		break;
	case CS_KEY_APOSTROPHE:
		text.append("'");
		break;
	case CS_KEY_COMMA:
		text.append(",");
		break;
	case CS_KEY_MINUS:
		text.append("-");
		break;
	case CS_KEY_PERIOD:
		text.append(".");
		break;
	case CS_KEY_SLASH:
		text.append("/");
		break;
	case CS_KEY_0:
		text.append("0");
		break;
	case CS_KEY_1:
		text.append("1");
		break;
	case CS_KEY_2:
		text.append("2");
		break;
	case CS_KEY_3:
		text.append("3");
		break;
	case CS_KEY_4:
		text.append("4");
		break;
	case CS_KEY_5:
		text.append("5");
		break;
	case CS_KEY_6:
		text.append("6");
		break;
	case CS_KEY_7:
		text.append("7");
		break;
	case CS_KEY_8:
		text.append("8");
		break;
	case CS_KEY_9:
		text.append("9");
		break;
	case CS_KEY_SEMICOLON:
		text.append(";");
		break;
	case CS_KEY_EQUAL:
		text.append("=");
		break;
	case CS_KEY_A:
		if (caps)
		{
			text.append("A");
			break;
		}
		text.append("a");
		break;
	case CS_KEY_B:
		if (caps)
		{
			text.append("B");
			break;
		}
		text.append("b");
		break;
	case CS_KEY_C:
		if (caps)
		{
			text.append("C");
			break;
		}
		text.append("c");
		break;
	case CS_KEY_D:
		if (caps)
		{
			text.append("D");
			break;
		}
		text.append("d");
		break;
	case CS_KEY_E:
		if (caps)
		{
			text.append("E");
			break;
		}
		text.append("e");
		break;
	case CS_KEY_F:
		if (caps)
		{
			text.append("F");
			break;
		}
		text.append("f");
		break;
	case CS_KEY_G:
		if (caps)
		{
			text.append("G");
			break;
		}
		text.append("g");
		break;
	case CS_KEY_H:
		if (caps)
		{
			text.append("H");
			break;
		}
		text.append("h");
		break;
	case CS_KEY_I:
		if (caps)
		{
			text.append("I");
			break;
		}
		text.append("i");
		break;
	case CS_KEY_J:
		if (caps)
		{
			text.append("J");
			break;
		}
		text.append("j");
		break;
	case CS_KEY_K:
		if (caps)
		{
			text.append("K");
			break;
		}
		text.append("k");
		break;
	case CS_KEY_L:
		if (caps)
		{
			text.append("L");
			break;
		}
		text.append("l");
		break;
	case CS_KEY_M:
		if (caps)
		{
			text.append("M");
			break;
		}
		text.append("m");
		break;
	case CS_KEY_N:
		if (caps)
		{
			text.append("N");
			break;
		}
		text.append("n");
		break;
	case CS_KEY_O:
		if (caps)
		{
			text.append("O");
			break;
		}
		text.append("o");
		break;
	case CS_KEY_P:
		if (caps)
		{
			text.append("P");
			break;
		}
		text.append("p");
		break;
	case CS_KEY_Q:
		if (caps)
		{
			text.append("Q");
			break;
		}
		text.append("q");
		break;
	case CS_KEY_R:
		if (caps)
		{
			text.append("R");
			break;
		}
		text.append("r");
		break;
	case CS_KEY_S:
		if (caps)
		{
			text.append("S");
			break;
		}
		text.append("s");
		break;
	case CS_KEY_T:
		if (caps)
		{
			text.append("T");
			break;
		}
		text.append("t");
		break;
	case CS_KEY_U:
		if (caps)
		{
			text.append("U");
			break;
		}
		text.append("u");
		break;
	case CS_KEY_V:
		if (caps)
		{
			text.append("V");
			break;
		}
		text.append("v");
		break;
	case CS_KEY_W:
		if (caps)
		{
			text.append("W");
			break;
		}
		text.append("w");
		break;
	case CS_KEY_X:
		if (caps)
		{
			text.append("X");
			break;
		}
		text.append("x");
		break;
	case CS_KEY_Y:
		if (caps)
		{
			text.append("Y");
			break;
		}
		text.append("y");
		break;
	case CS_KEY_Z:
		if (caps)
		{
			text.append("Z");
			break;
		}
		text.append("z");
		break;
	case CS_KEY_LEFT_BRACKET:
		text.append("[");
		break;
	case CS_KEY_BACKSLASH:
		text.append("\\");
		break;
	case CS_KEY_RIGHT_BRACKET:
		text.append("]");
		break;
	case CS_KEY_ENTER:
		m_selected = false;
		break;
	case CS_KEY_BACKSPACE:
		if (!text.empty())
		{
			text.pop_back();
		}
		break;
	case CS_KEY_CAPS_LOCK:
		caps = !caps;
		break;
	default:
		break;
	}

}
