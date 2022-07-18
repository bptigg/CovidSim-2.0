#pragma once

#include "../Renderer.h"

struct Collision_Box
{
	glm::vec2 lower_bound, upper_bound;
	float width = 1.0f;
};

bool Point_Intersection(glm::vec2 point, glm::vec2 pos, Collision_Box box);

#if _DEBUG
void draw_collision_box(Collision_Box box, glm::vec2 pos);
#endif


class Entity
{
};

