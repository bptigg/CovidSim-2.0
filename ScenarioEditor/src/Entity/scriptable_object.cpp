#include "scriptable_object.h"

scriptable_object::scriptable_object(const glm::vec2& location, const glm::vec2& size, Layer* layer, int id)
	:m_location(location), m_size(size), m_Layer(layer), m_object_id(id)
{
	m_collison_box.lower_bound = { location.x - (size.x / 2.0f), location.y - (size.y / 2.0f) };
	m_collison_box.upper_bound = { location.x + (size.x / 2.0f), location.y + (size.y / 2.0f) };
	m_collison_box.width = 2.0f;

	m_delete_object = false;
	//m_render = true;
}

scriptable_object::~scriptable_object()
{
}

