#include "Entity.h"

bool Point_Intersection(glm::vec2 point, glm::vec2 pos, Collision_Box box)
{
	float lowerX = pos.x + box.lower_bound.x;
	float lowerY = pos.y + box.lower_bound.y;
	float upperX = pos.x + box.upper_bound.x;
	float upperY = pos.y + box.upper_bound.y;

	return point.x > lowerX && point.y > lowerY && point.x < upperX&& point.y < upperY;
}

void draw_collision_box(Collision_Box box, glm::vec2 pos)
{
	Renderer::begin_batch(); //will change the way the renderer draws to having a buffer seperate from the batch so it can deal with textures
	Renderer::draw_box(pos, { box.upper_bound.x - box.lower_bound.x, box.upper_bound.y - box.lower_bound.x }, 1.0f, {0.0f,0.0f,0.0f,1.0f});
}
