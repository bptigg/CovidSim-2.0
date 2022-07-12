#include "Index_Buffer.h"

#include "gl_debug.h"

Index_Buffer::Index_Buffer(const unsigned int* data, unsigned int count)
    :m_count(count)
{
    GlCall(glGenBuffers(1, &m_renderer_id));
    GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id));
    GlCall(glBufferData(GL_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

Index_Buffer::~Index_Buffer()
{
    GlCall(glDeleteBuffers(1, &m_renderer_id));
}

void Index_Buffer::bind() const 
{
    GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id));
}

void Index_Buffer::unbind() const 
{
    GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
