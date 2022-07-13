#include "Index_Buffer.h"

#include "gl_debug.h"

Index_Buffer::Index_Buffer(const unsigned int* data, unsigned int count)
    :m_count(count)
{
    GlCall(glGenBuffers(1, &m_renderer_id));
    GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id));
    GlCall(glBufferData(GL_ARRAY_BUFFER, m_count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

Index_Buffer::Index_Buffer(unsigned int count)
    :m_count(count)
{
    unsigned int* data = new unsigned int[count];

    unsigned int quads = count / 6;
    for (unsigned int i = 0; i < quads; i++)
    {
        data[i * 6] = (i * 4);
        data[i * 6 + 1] = (i * 4) + 1;
        data[i * 6 + 2] = (i * 4) + 2;
        data[i * 6 + 3] = (i * 4) + 2;
        data[i * 6 + 4] = (i * 4) + 3;
        data[i * 6 + 5] = (i * 4);
    }

    GlCall(glGenBuffers(1, &m_renderer_id));
    GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id));
    GlCall(glBufferData(GL_ARRAY_BUFFER, m_count * sizeof(unsigned int), data, GL_STATIC_DRAW));

    delete[] data;
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
