#pragma once

#include "Vertex_Buffer.h"
#include "Vertex_Buffer_Layout.h"

class Vertex_Array
{
private:
	unsigned int m_renderer_id;
public:
	Vertex_Array();
	~Vertex_Array();

	void add_buffer(const Vertex_Buffer& vb, const Vertex_Buffer_Layout& layout);

	void bind() const;
	void unbind() const;
};

