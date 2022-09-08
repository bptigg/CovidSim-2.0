#include "Scrollable_Menu.h"

Scrollable_Menu::Scrollable_Menu(glm::vec2 pos, glm::vec2 size, Layer* layer, unsigned int render_layer, int id)
	:scriptable_object(pos, size, layer, id)
{
	m_menu_object = true;
	full = false;
	m_type = entity_type::SCROLLABLE_MENU;
	no_bounds = false;

	bounds.y = m_location.y + m_size.y / 2.0f;
	bounds.x = m_location.y - m_size.y / 2.0f;
}

Scrollable_Menu::~Scrollable_Menu()
{
}

void Scrollable_Menu::render()
{
}

void Scrollable_Menu::update()
{
}

void Scrollable_Menu::event_callback(Events::Event& e)
{
	if (e.Get_Event_Type() == Events::Event_Type::Mouse_Scrolled)
	{
		Events::Event_Dispatcher dispatch(e);
		dispatch.Dispatch<Events::Mouse_Scrolled_Event>(BIND_EVENT_FN(Scrollable_Menu::on_mouse_scroll));
	}
}

void Scrollable_Menu::update_position(const float& zoom, const glm::vec2& camera_pos, const glm::mat4& camera_mat)
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

void Scrollable_Menu::add_object(scriptable_object* obj)
{
	m_menu_objects.push_back(obj);

	if (obj->get_position().y >= m_location.y + (m_size.y / 2.0f) || obj->get_position().y <= m_location.y - (m_size.y / 2.0f))
	{
		obj->render_obj(false);
	}

	auto comparison = [](scriptable_object* a, scriptable_object* b)
	{
		return a->get_position().y > b->get_position().y;
	};

	std::sort(m_menu_objects.begin(), m_menu_objects.end(), comparison);

	bounds.y = m_menu_objects[0]->get_position().y;
	bounds.x = m_menu_objects[m_menu_objects.size() - 1]->get_position().y;

	if (bounds.y >= m_location.y + m_size.y / 2.0f)
	{
		bounds.y = m_location.y + m_size.y / 2.0f;
	}
	if (bounds.x <= m_location.y - m_size.y / 2.0f)
	{
		bounds.x = m_location.y - m_size.y / 2.0f;
	}
}

void Scrollable_Menu::remove_object(scriptable_object* obj)
{
	m_menu_objects.erase(std::remove(m_menu_objects.begin(), m_menu_objects.end(), obj), m_menu_objects.end());
}

void Scrollable_Menu::insert_below(scriptable_object* obj)
{
	m_menu_objects.push_back(obj);

	if (m_menu_objects.size() == 1)
	{
		obj->change_position({ m_location.x, m_location.y + (m_size.y / 2.0f) - obj->get_size().y});
	}
	else
	{
		obj->change_position({ m_location.x, m_menu_objects[m_menu_objects.size() - 2]->get_position().y - 80.0f });
	}
	
	if (obj->get_position().y >= m_location.y + (m_size.y / 2.0f) || obj->get_position().y <= m_location.y - (m_size.y / 2.0f))
	{
		obj->render_obj(false);
	}

}

bool Scrollable_Menu::on_mouse_scroll(Events::Mouse_Scrolled_Event& e)
{
	if (!is_mouse_over())
	{
		return false;
	}

	float offset = e.GetYOffset() * m_step;

	m_position += offset;

	if (bounds.x > m_location.y - m_size.y / 2.0f && full == false)
	{
		return true;
	}

	if (!no_bounds)
	{
		if (m_menu_objects[0]->get_position().y + offset < bounds.y)
		{
			m_position = m_position - offset;
			offset = 0;
		}
		else if (m_menu_objects[m_menu_objects.size() - 1]->get_position().y + offset > bounds.x)
		{
			m_position = m_position - offset;
			offset = 0;
		}
	}

	for (scriptable_object* obj : m_menu_objects)
	{
		glm::vec2 new_pos = { obj->get_position().x, obj->get_position().y + offset };
		obj->change_position(new_pos);
		if (new_pos.y + obj->get_size().y / 2.0f > m_location.y + (m_size.y / 2.0f))
		{
			obj->render_obj(false);
		}
		else if (new_pos.y - obj->get_size().y / 2.0f < m_location.y - (m_size.y / 2.0f))
		{
			obj->render_obj(false);
		}
		else
		{
			obj->render_obj(true);
		}
	}

	return true;
}

bool Scrollable_Menu::is_mouse_over()
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
			return true;
		}
	}
	return false;
}
