#include "Renderer.h"

struct render_data
{
	unsigned int White_Texture = 0;
	unsigned int White_Texture_Slot = 0;

	unsigned int Index_Count = 0;

	Vertex* Object_Buffer = nullptr;
	Vertex* Object_Buffer_Ptr = nullptr;

	std::array<GLuint, Texture::MAX_TEXTURE_SLOTS> texture_slots;
	unsigned int current_texture_slot = 1;
};

static render_data s_data;

Renderer::~Renderer()
{
	if (initlized)
	{
		shutdown();
	}
}

void Renderer::init(std::vector<std::string> textures)
{

	if (initlized)
	{
		Log::error("RENDERER ALREADY INITLIZED", __FILE__, __LINE__);
		return; 
	}

	s_data.Object_Buffer = new Vertex[m_MAX_VERTICIES];

	int samplers[Texture::MAX_TEXTURE_SLOTS];
	for (int i = 0; i < Texture::MAX_TEXTURE_SLOTS; i++)
	{
		samplers[i] = i;
	}

	m_shader = new shader("res/shaders/Texture.shader");
	m_Index_Buffer = new Index_Buffer(m_MAX_INDICIES);
	m_Vertex_Buffer = new Vertex_Buffer(m_MAX_VERTICIES * sizeof(Vertex));
	
	m_VA0 = new Vertex_Array();
	{
		Vertex_Buffer_Layout layout;
		layout.Push<float>(2);
		layout.Push<float>(4);
		layout.Push<float>(2);
		layout.Push<float>(1);
		m_VA0->add_buffer(*m_Vertex_Buffer, layout);
	}


	glm::mat4 mvp = glm::ortho(0.0f, 1280.0f, 0.0f, 960.f, -1.0f, 1.0f);
	m_shader->set_uniform_mat_4f("u_MVP", mvp);
	m_shader->set_uniform_1i("u_texture", 0);

	unsigned int color = 0xffffffff;

	s_data.texture_slots[1] = Texture::Load_Texture("res/textures/ork.jpg");
	s_data.texture_slots[0] = Texture::Create_Texture(1,1,color,s_data.White_Texture);

	for (int i = 2; i < 32; i++)
	{
		s_data.texture_slots[i] = 0;
	}

	initlized = true;
}

void Renderer::shutdown()
{
	Log::info("Renderer shutting down....");
	delete[] s_data.Object_Buffer;
	delete m_shader;
	delete m_VA0;
	delete m_Vertex_Buffer;
	delete m_Index_Buffer;

	Texture::delete_texture(s_data.White_Texture);

	initlized = false;
	Log::info("Renderer shut down");
}

void Renderer::clear()
{
	GlCall(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::draw(const Vertex_Array& vao, const Index_Buffer& ib, const shader& shader)
{
	shader.bind();
	vao.bind();
	ib.bind();
	GlCall(glDrawElements(GL_TRIANGLES, ib.get_count(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::draw()
{
	for (unsigned int i = 0; i < s_data.texture_slots.size(); i++)
	{
		if (s_data.texture_slots[i] != 0)
		{
			GlCall(glBindTextureUnit(i, s_data.texture_slots[i]));
		}
	}

	m_shader->bind();
	m_VA0->bind();
	m_Index_Buffer->bind();

	//GlCall(glDrawElements(GL_TRIANGLES, m_Index_Buffer->get_count(), GL_UNSIGNED_INT, nullptr));
	GlCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}

void Renderer::draw_rectangle_color(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
	if (s_data.Index_Count >= m_MAX_INDICIES)
	{
		//EndBatch();
		//Flush();
		//BeginBatch();
	}

	float texture_index = s_data.White_Texture_Slot;

	s_data.Object_Buffer_Ptr->position = { position.x, position.y };
	s_data.Object_Buffer_Ptr->color = color;
	s_data.Object_Buffer_Ptr->texture_coord = { 0.0f, 0.0f };
	s_data.Object_Buffer_Ptr->tex_id = texture_index;
	s_data.Object_Buffer_Ptr++;

	s_data.Object_Buffer_Ptr->position = { position.x + size.x, position.y };
	s_data.Object_Buffer_Ptr->color = color;
	s_data.Object_Buffer_Ptr->texture_coord = { 1.0f, 0.0f };
	s_data.Object_Buffer_Ptr->tex_id = texture_index;
	s_data.Object_Buffer_Ptr++;

	s_data.Object_Buffer_Ptr->position = { position.x + size.x, position.y + size.y };
	s_data.Object_Buffer_Ptr->color = color;
	s_data.Object_Buffer_Ptr->texture_coord = { 1.0f, 1.0f };
	s_data.Object_Buffer_Ptr->tex_id = texture_index;
	s_data.Object_Buffer_Ptr++;

	s_data.Object_Buffer_Ptr->position = { position.x, position.y + size.y };
	s_data.Object_Buffer_Ptr->color = color;
	s_data.Object_Buffer_Ptr->texture_coord = { 0.0f, 1.0f };
	s_data.Object_Buffer_Ptr->tex_id = texture_index;
	s_data.Object_Buffer_Ptr++;

	s_data.Index_Count += 6;

}

void Renderer::draw_rectangle_texture(const glm::vec2& position, const glm::vec2& size, const unsigned int index)
{
}


void Renderer::flush()
{
}
