#include "Renderer.h"

void Renderer::clear()
{
	GlCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::draw(const Vertex_Array& vao, const Index_Buffer& ib, const shader& shader)
{
	shader.bind();
	vao.bind();
	ib.bind();
	GlCall(glDrawElements(GL_TRIANGLES, ib.get_count(), GL_UNSIGNED_INT, nullptr))
}
