#pragma once

#include <glm/glm.hpp>
#include <string>

class Text
{
public:
	glm::vec2 pos;

	std::string m_text;
	float m_scale;
	glm::vec4 m_colour;

	bool is_centred;

	Text(std::string& text, const glm::vec2 position, float scale, glm::vec4 colour, bool centred = true);
	~Text();

	void render(float x_offset, float y_offset, unsigned int layer);
	void update_string(std::string new_string);
};
