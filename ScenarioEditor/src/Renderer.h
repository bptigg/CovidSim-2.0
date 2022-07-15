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

#include <memory>
#include <array>


class Renderer
{
private:
	const unsigned int m_MAX_QUADS = 10000;
	const unsigned int m_MAX_VERTICIES = m_MAX_QUADS * 4;
	const unsigned int m_MAX_INDICIES = m_MAX_QUADS * 6;

	//std::unique_ptr<Index_Buffer> m_Index_Buffer;
	//std::unique_ptr<Vertex_Buffer> m_Vertex_Buffer;
	//std::unique_ptr<Vertex_Array> m_VA0;
	//std::unique_ptr<shader> m_shader;

	shader* m_shader = nullptr;
	Vertex_Array* m_VA0 = nullptr;
	Vertex_Buffer* m_Vertex_Buffer = nullptr;
	Index_Buffer* m_Index_Buffer = nullptr;

	bool initlized;
	
public:

	~Renderer();
	
	void init(std::vector<std::string> textures);
	void shutdown();

	void begin_batch();
	void end_batch();
	
	void clear();
	void draw(const Vertex_Array& vao, const Index_Buffer& ib, const shader& shader);
	void draw();

	void update_view(const glm::mat4& projection_view_matrix);

#pragma region draw_shapes
	void draw_rectangle_color(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
	void draw_rectangle_texture(const glm::vec2& position, const glm::vec2& size, const unsigned int index);

	void flush();

};

