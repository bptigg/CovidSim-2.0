#include "Text_Menu_object.h"

Text_Menu_object::Text_Menu_object(Text text, glm::vec2 pos, Layer* layer, unsigned int render_layer)
	:scriptable_object(pos, { 0.0f, 0.0f }, layer), m_text(text)
{
	m_set_up = false;
	m_menu_object = true;
	m_rendering_layer = render_layer;
	m_type = entity_type::TEXT_MENU_OBJECT;
}

void Text_Menu_object::render()
{
	m_text.render(0.0f, 0.0f, m_rendering_layer, true);
}

void Text_Menu_object::update()
{
	render();
}

void Text_Menu_object::event_callback(Events::Event& e)
{
	return;
}

void Text_Menu_object::update_position(const float& zoom, const glm::vec2& camera_pos, const glm::mat4& camera_mat)
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
