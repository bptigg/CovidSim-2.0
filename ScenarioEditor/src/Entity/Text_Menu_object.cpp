#include "Text_Menu_object.h"

#define TEXT_RENDERING_LAYER 3

Text_Menu_object::Text_Menu_object(Text text, glm::vec2 pos, Layer* layer)
	:scriptable_object(pos, { 0.0f, 0.0f }, layer), m_text(text)
{
}

void Text_Menu_object::render()
{
	m_text.render(m_location.x, m_location.y, TEXT_RENDERING_LAYER);
}

void Text_Menu_object::update()
{
}
