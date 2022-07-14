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

	m_shader = new shader("res/shaders/simple.shader");
	m_shader->set_uniform_1iv("u_Textures", 32, samplers);
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
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	m_shader->set_uniform_mat_4f("u_View_Proj", mvp);
	m_shader->set_uniform_mat_4f("u_Transform", transform);

	unsigned int color = 0xffffffff;

	s_data.texture_slots[0] = Texture::Create_Texture(1,1,color,s_data.White_Texture);
	s_data.White_Texture_Slot = 0;

	for (int i = 1; i < Texture::MAX_TEXTURE_SLOTS; i++)
	{
		s_data.texture_slots[i] = 0;
	}

	Texture::Load_Texture("res/textures/ork.jpg");

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

void Renderer::begin_batch()
{
	s_data.Object_Buffer_Ptr = s_data.Object_Buffer;
}

void Renderer::end_batch()
{
	GLsizeiptr buffer_size = (uint8_t*)s_data.Object_Buffer_Ptr - (uint8_t*)s_data.Object_Buffer;
	m_Vertex_Buffer->add_to_buffer(s_data.Object_Buffer, buffer_size);
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

	begin_batch();
	draw_rectangle_texture({ 200.0f, 200.0f }, { 120.0f, 160.0f }, 1);
	end_batch();

	flush();

}

void Renderer::draw_rectangle_color(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
	if (s_data.Index_Count >= m_MAX_INDICIES)
	{
		end_batch();
		flush();
		begin_batch();
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

void Renderer::draw_rectangle_texture(const glm::vec2& position, const glm::vec2& size, const unsigned int texture_id)
{
	if (s_data.Index_Count >= m_MAX_INDICIES || s_data.current_texture_slot > 31)
	{
		end_batch();
		flush();
		begin_batch();
	}


	float texture_index = 0.0f;
	for (unsigned int i = 1; i < s_data.current_texture_slot; i++)
	{
		if (s_data.texture_slots[i] == texture_id)
		{
			texture_index = (float)i;
			break;
		}
	}

	if (texture_index == 0.0f)
	{
		texture_index = (float)s_data.current_texture_slot;
		s_data.texture_slots[s_data.current_texture_slot] = texture_id;
		s_data.current_texture_slot++;
	}

	constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

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


void Renderer::flush()
{
	for (unsigned int i = 0; i < s_data.texture_slots.size(); i++)
	{
		GlCall(glBindTextureUnit(i, s_data.texture_slots[i]));
	}

	m_shader->bind();
	m_VA0->bind();
	m_Index_Buffer->bind();

	GlCall(glDrawElements(GL_TRIANGLES, s_data.Index_Count, GL_UNSIGNED_INT, nullptr));

	s_data.Index_Count = 0;
	s_data.current_texture_slot = 1;
}
