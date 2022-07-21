#pragma once

#include "gl_debug.h"
#include <GL/glew.h>

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include "Vertex_Array.h"
#include "Index_Buffer.h"
#include "Shader.h"
#include "Texture.h"

#include "Vertex.h"
#include "Renderer_Manager.h"

#include <memory>
#include <array>


class Renderer
{
private:
	//const unsigned int m_MAX_QUADS = 10000;
	//const unsigned int m_MAX_VERTICIES = m_MAX_QUADS * 4;
	//const unsigned int m_MAX_INDICIES = m_MAX_QUADS * 6;

	//std::unique_ptr<Index_Buffer> m_Index_Buffer;
	//std::unique_ptr<Vertex_Buffer> m_Vertex_Buffer;
	//std::unique_ptr<Vertex_Array> m_VA0;
	//std::unique_ptr<shader> m_shader;

	//shader* m_shader = nullptr;
	//Vertex_Array* m_VA0 = nullptr;
	//Vertex_Buffer* m_Vertex_Buffer = nullptr;
	//Index_Buffer* m_Index_Buffer = nullptr;

	//bool initlized;
	
public:

	~Renderer();
	
	static void init(std::vector<std::string> textures);
	static void shutdown();
	
	static void begin_batch();
	static void end_batch();
	
	static void clear();
	static void draw(const Vertex_Array& vao, const Index_Buffer& ib, const shader& shader);
	static void draw();
	
	static void update_view(const glm::mat4& projection_view_matrix);

#pragma region draw_shapes
	static void draw_rectangle_color(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, unsigned int layer);
	static void draw_rectangle_texture(const glm::vec2& position, const glm::vec2& size, const unsigned int index, unsigned int layer);

	static void draw_box(const glm::vec2& centre, const glm::vec2& size, const float border_width, const glm::vec4 color, unsigned int layer);

	static void draw_text(std::string& text, const glm::vec2 centre, const glm::vec4& color, unsigned int layer, float scale, bool centred);

	static void flush();

private:
	static void m_draw_rectangle_color(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
	static void m_draw_rectangle_texture(const glm::vec2& position, const glm::vec2& size, const unsigned int index);

	static void m_draw_box(const glm::vec2& centre, const glm::vec2& size, const float border_width, const glm::vec4 color);
	static void m_draw_text(std::string& text, const glm::vec2& position, const glm::vec2& size, glm::vec4& color, float scale, bool centred);

	static std::array<Vertex, 4> m_convert_character_to_vertices(Character* ch, float x_offset, float tex_slot, float scale, std::string& text, const glm::vec2& position, const glm::vec2& size, glm::vec4& color, bool centred);

};

