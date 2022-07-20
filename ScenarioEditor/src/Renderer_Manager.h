#pragma once

#include "glm/glm.hpp"

#include <tuple>
#include <map>
#include <unordered_map>
#include <vector>

enum class render_type
{
	COLOURED_RECTANGLE = 0,
	TEXTURED_RECTANGLE = 1,
	COLOURED_BOX = 2,
	TEXT = 3
};


struct Character
{
	unsigned int tex_id = 0;
	glm::ivec2 size;
	glm::ivec2 bearing;
	uint16_t advance;
};


class render_queue_object
{
public:
	
	render_queue_object(render_type o_type, const glm::vec2& o_position, const glm::vec2 o_size)
		:type(o_type), position(o_position), size(o_size)
	{
		color = { 1.0f, 1.0f, 1.0f, 1.0f };
		texture_id = 0;
		border_width = 0.0f;
		
		rotation = 0.0;
		text = "";
		scale = 1.0f;
	}
	render_type type;
	
	glm::vec2 position;
	glm::vec2 size;
	
	glm::vec4 color;
	unsigned int texture_id;
	unsigned int layer;

	//text specific
	double rotation;
	std::string text;
	float scale;

	float border_width;

};


class Renderer_Manager
{
public:
	Renderer_Manager();
	~Renderer_Manager();

	//draw objects
	void draw_rectangle_texture(const glm::vec2& position, const glm::vec2& size, const unsigned int texture_id, unsigned int layer);
	void draw_rectangle_color(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, unsigned int layer);

	void draw_box(const glm::vec2& centre, const glm::vec2& size, const float border_width, const glm::vec4& color, unsigned int layer);

	void draw_text(std::string& text, const glm::vec2& centre, const glm::vec2& size, const glm::vec4& color, unsigned int layer, float scale);

	std::unordered_map<unsigned int, std::vector<render_queue_object*>>::iterator next_draw();
	void delete_queue();
private:
	void Push_Object(render_queue_object* object);
	void Pop_Object(render_queue_object* object);
public:
	bool finished = true;
	bool next_layer = true;
	bool empty = false;
private:
	std::map<unsigned int, std::unordered_map<unsigned int, std::vector<render_queue_object*>>> m_layer_queue;

	unsigned int m_map_position = 0;
	unsigned int m_layer_position = 0;

	unsigned int m_max_layers = 0;
	
	std::map<unsigned int, unsigned int> m_max_positions; 

};

