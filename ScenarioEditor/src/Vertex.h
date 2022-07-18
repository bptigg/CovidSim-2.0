#pragma once

#include <glm/glm.hpp>

struct Vertex
{
	glm::vec2 position;
	glm::vec4 color;
	glm::vec2 texture_coord;
	float tex_id;
};

struct Line_Vertex
{
	glm::vec2 position;
	glm::vec4 color;
	float tex_id;
};