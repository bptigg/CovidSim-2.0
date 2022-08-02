#pragma once

class Vertex_Buffer
{
private:
	unsigned int m_renderer_id;
	bool is_dynamic;
	unsigned int m_offset = 0;
	unsigned int m_buffer_size;
public:
	Vertex_Buffer(const void* data, unsigned int size);
	Vertex_Buffer(unsigned int size);
	~Vertex_Buffer();

	void bind() const;
	void unbind() const;

	void clear_buffer();
	bool add_to_buffer(const void* vertices, unsigned int size);

	inline unsigned int get_buffer_size() { return m_buffer_size; }
	inline bool can_store(unsigned int size) { return m_offset + size <= m_buffer_size; }
	inline bool is_empty() { return m_offset == 0; }
};

