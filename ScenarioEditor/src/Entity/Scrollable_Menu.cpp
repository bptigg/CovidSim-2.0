#include "Scrollable_Menu.h"

Scrollable_Menu::Scrollable_Menu(glm::vec2 pos, glm::vec2 size, Layer* layer, unsigned int render_layer, int id)
	:scriptable_object(pos, size, layer, id)
{
	m_menu_object = true;
	full = false;
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

bool Scrollable_Menu::on_mouse_scroll(Events::Mouse_Scrolled_Event& e)
{
	float offset = e.GetYOffset() * m_step;

	m_position += offset;

	if (bounds.x > m_location.y - m_size.y / 2.0f && full == false)
	{
		return true;
	}

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
