#pragma once

#include "gl_debug.h"
#include <GL/glew.h>

#include "Vertex_Array.h"
#include "Index_Buffer.h"
#include "Shader.h"


class Renderer
{
private:
public:
	void clear();
	void draw(const Vertex_Array& vao, const Index_Buffer& ib, const shader& shader);
};

