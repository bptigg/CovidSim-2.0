#include "Vertex_Buffer.h"
#include "gl_debug.h"

Vertex_Buffer::Vertex_Buffer(const void* data, unsigned int size)
{
    GlCall(glGenBuffers(1, &m_renderer_id));
    GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id));
    GlCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

Vertex_Buffer::~Vertex_Buffer()
{
    GlCall(glDeleteBuffers(1, &m_renderer_id));
}

void Vertex_Buffer::bind() const 
{
    GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id));
}

void Vertex_Buffer::unbind() const
{
    GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
