#include "Vertex_Buffer.h"
#include "gl_debug.h"

Vertex_Buffer::Vertex_Buffer(const void* data, unsigned int size)
{
    GlCall(glGenBuffers(1, &m_renderer_id));
    GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id));
    GlCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

Vertex_Buffer::Vertex_Buffer(unsigned int size)
{
    GlCall(glGenBuffers(1, &m_renderer_id));
    GlCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id));
    GlCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW));
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

void Vertex_Buffer::clear_buffer()
{
    GlCall(glClearNamedBufferData(m_renderer_id, GL_RGBA16, GL_RGBA, GL_UNSIGNED_BYTE, nullptr));
    m_offset = 0;
}

bool Vertex_Buffer::add_to_buffer(const void* vertices, unsigned int size)
{
    if (!can_store(size))
    {
        Log::error("RENDER BUFFER FULL", __FILE__, __LINE__);
        return false;
    }
    else
    {
        GlCall(glNamedBufferSubData(m_renderer_id, m_offset, size, vertices));
        m_offset += size;
        return true;
    }
    return false;
}
