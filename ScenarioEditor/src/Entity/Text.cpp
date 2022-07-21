#include "Text.h"

#include "../Renderer.h"

Text::Text(std::string text, const glm::vec2& position, float scale, const glm::vec4& colour, bool centred)
{
	pos = position;
	m_text = text;
	m_colour = colour;
	is_centred = centred;
	m_scale = scale;
}

Text::~Text()
{
}

void Text::render(float x_offset, float y_offset, unsigned int layer)
{
	Renderer::draw_text(m_text, { pos.x + x_offset, pos.y + y_offset }, m_colour, layer, m_scale, is_centred);
}

void Text::update_string(std::string new_string)
{
	m_text = new_string;
}
