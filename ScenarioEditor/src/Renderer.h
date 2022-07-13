#pragma once

#include "gl_debug.h"
#include <GL/glew.h>

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include "Vertex_Array.h"
#include "Index_Buffer.h"
#include "Shader.h"
#include "Texture.h"

#include <memory>
#include <array>


class Renderer
{
private:
	const unsigned int m_MAX_QUADS = 10000;
	const unsigned int m_MAX_VERTICIES = m_MAX_QUADS * 4;
	const unsigned int m_MAX_INDICIES = m_MAX_QUADS * 6;

	std::unique_ptr<Index_Buffer> m_Index_Buffer;
	std::unique_ptr<Vertex_Buffer> m_Vertex_Buffer;
	std::unique_ptr<Vertex_Array> m_VA0;
	std::unique_ptr<shader> m_shader;

	bool initlized;
	
public:

	void init(std::vector<std::string> textures);
	void shutdown();
	
	void clear();
	void draw(const Vertex_Array& vao, const Index_Buffer& ib, const shader& shader);
	void draw(const Vertex_Array& vao, const Index_Buffer& ib);

	void flush();

};

