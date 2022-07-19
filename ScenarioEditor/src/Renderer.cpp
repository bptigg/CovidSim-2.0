#include "Renderer.h"

#include "Renderer_Manager.h"

static const unsigned int m_MAX_QUADS = 10000;
static const unsigned int m_MAX_VERTICIES = m_MAX_QUADS * 4;
static const unsigned int m_MAX_INDICIES = m_MAX_QUADS * 6;

struct render_data
{
	unsigned int White_Texture = 0;
	unsigned int White_Texture_Slot = 0;

	shader* quad_shader = nullptr;
	
	Vertex_Array* VA0 = nullptr;
	Vertex_Buffer* VertexBuffer = nullptr;
	Index_Buffer* IndexBuffer = nullptr;


	bool initlized = false;

	unsigned int Index_Count = 0;
	unsigned int line_index_count = 0;

	float line_width = 5.0f;

	Vertex* Object_Buffer = nullptr;
	Vertex* Object_Buffer_Ptr = nullptr;

	std::array<GLuint, Texture::MAX_TEXTURE_SLOTS> texture_slots;
	unsigned int current_texture_slot = 1;
};

static render_data s_data;
static Renderer_Manager manager;

Renderer::~Renderer()
{
	if (s_data.initlized)
	{
		shutdown();
	}
}

void Renderer::init(std::vector<std::string> textures)
{

	if (s_data.initlized)
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

	s_data.quad_shader = new shader("res/shaders/simple.shader");
	s_data.quad_shader->set_uniform_1iv("u_Textures", 32, samplers);
	s_data.IndexBuffer = new Index_Buffer(m_MAX_INDICIES);
	
	s_data.VertexBuffer = new Vertex_Buffer(m_MAX_VERTICIES * sizeof(Vertex));
	s_data.VA0 = new Vertex_Array();
	{
		Vertex_Buffer_Layout layout;
		layout.Push<float>(2);
		layout.Push<float>(4);
		layout.Push<float>(2);
		layout.Push<float>(1);
		s_data.VA0->add_buffer(*s_data.VertexBuffer, layout);
	}

	glm::mat4 mvp = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	
	s_data.quad_shader->set_uniform_mat_4f("u_View_Proj", mvp);
	s_data.quad_shader->set_uniform_mat_4f("u_Transform", transform);
	

	unsigned int color = 0xffffffff;

	s_data.texture_slots[0] = Texture::Create_Texture(1,1,color,s_data.White_Texture);
	s_data.White_Texture_Slot = 0;

	for (int i = 1; i < Texture::MAX_TEXTURE_SLOTS; i++)
	{
		s_data.texture_slots[i] = 0;
	}

	Texture::Load_Texture("res/textures/ork.jpg");

	s_data.initlized = true;
}

void Renderer::shutdown()
{
	Log::info("Renderer shutting down....");
	delete[] s_data.Object_Buffer;
	
	delete s_data.quad_shader;
	
	delete s_data.VA0;
	delete s_data.VertexBuffer;
	delete s_data.IndexBuffer;

	manager.delete_queue();

	Texture::delete_texture(s_data.White_Texture);

	s_data.initlized = false;
	Log::info("Renderer shut down");
}

void Renderer::begin_batch()
{
	s_data.Object_Buffer_Ptr = s_data.Object_Buffer;
}

void Renderer::end_batch()
{
	GLsizeiptr buffer_size = (uint8_t*)s_data.Object_Buffer_Ptr - (uint8_t*)s_data.Object_Buffer;
	s_data.VertexBuffer->add_to_buffer(s_data.Object_Buffer, buffer_size);

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
	//for (unsigned int i = 0; i < s_data.texture_slots.size(); i++)
	//{
	//	if (s_data.texture_slots[i] != 0)
	//	{
	//		GlCall(glBindTextureUnit(i, s_data.texture_slots[i]));
	//	}
	//}

	//begin_batch();
	//draw_rectangle_texture({ -60.0f, -60.0f }, { 120.0f, 120.0f }, 2);
	//draw_rectangle_texture({ 60.0f,  60.0f }, { 120.0f, 120.0f }, 2);
	//end_batch();

	//Do one draw call per texture stops the texutre overlap stuff

	//flush();
	//begin_batch();
	//draw_rectangle_color({  60.0f, -60.0f }, { 120.0f, 120.0f }, { 0.8f, 0.3f, 0.8f, 1.0f });
	//draw_rectangle_color({ -60.0f,  60.0f }, { 120.0f, 120.0f }, { 0.5f, 0.2f, 0.1f, 1.0f });

	while (!manager.finished)
	{
		auto draw = manager.next_draw();
		begin_batch();
		for (int i = 0; i < draw->second.size(); i++)
		{
			switch (draw->second[i]->type)
			{
			case render_type::COLOURED_RECTANGLE:
				m_draw_rectangle_color(draw->second[i]->position, draw->second[i]->size, draw->second[i]->color);
				break;
			case render_type::TEXTURED_RECTANGLE:
				m_draw_rectangle_texture(draw->second[i]->position, draw->second[i]->size, draw->second[i]->texture_id);
				break;
			case render_type::COLOURED_BOX:
				m_draw_box(draw->second[i]->position, draw->second[i]->size, draw->second[i]->border_width, draw->second[i]->color);
				break;
			default:
				Log::warning("UNKOWN DRAW TYPE");
				break;
			}
		}
		end_batch();
		flush();
	}

	manager.delete_queue();
	s_data.current_texture_slot = 1;

	//end_batch();

	//flush();


}

void Renderer::update_view(const glm::mat4& projection_view_matrix)
{
	s_data.quad_shader->set_uniform_mat_4f("u_View_Proj", projection_view_matrix);
}

void Renderer::draw_rectangle_color(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
	manager.draw_rectangle_color(position, size, color);
}

void Renderer::draw_rectangle_texture(const glm::vec2& position, const glm::vec2& size, const unsigned int index)
{
	manager.draw_rectangle_texture(position, size, index);
}

void Renderer::draw_box(const glm::vec2& centre, const glm::vec2& size, const float border_width, const glm::vec4 color)
{
	manager.draw_box(centre, size, border_width, color);
}

void Renderer::m_draw_rectangle_color(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
	if (s_data.Index_Count >= m_MAX_INDICIES)
	{
		end_batch();
		flush();
		begin_batch();
	}

	float texture_index = (float)s_data.White_Texture_Slot;

	s_data.Object_Buffer_Ptr->position = { position.x - (size.x / 2.0f), position.y - (size.y / 2.0f) };
	s_data.Object_Buffer_Ptr->color = color;
	s_data.Object_Buffer_Ptr->texture_coord = { 0.0f, 0.0f};
	s_data.Object_Buffer_Ptr->tex_id = texture_index;
	s_data.Object_Buffer_Ptr++;

	s_data.Object_Buffer_Ptr->position = { position.x + (size.x / 2.0f), position.y - (size.y / 2.0f) };
	s_data.Object_Buffer_Ptr->color = color;
	s_data.Object_Buffer_Ptr->texture_coord = { 1.0f, 0.0f};
	s_data.Object_Buffer_Ptr->tex_id = texture_index;
	s_data.Object_Buffer_Ptr++;

	s_data.Object_Buffer_Ptr->position = { position.x + (size.x / 2.0f), position.y + (size.y / 2.0f) };
	s_data.Object_Buffer_Ptr->color = color;
	s_data.Object_Buffer_Ptr->texture_coord = { 1.0f, 1.0f };
	s_data.Object_Buffer_Ptr->tex_id = texture_index;
	s_data.Object_Buffer_Ptr++;

	s_data.Object_Buffer_Ptr->position = { position.x - (size.x/ 2.0f), position.y + (size.y / 2.0f) };
	s_data.Object_Buffer_Ptr->color = color;
	s_data.Object_Buffer_Ptr->texture_coord = { 0.0f, 1.0f };
	s_data.Object_Buffer_Ptr->tex_id = texture_index;
	s_data.Object_Buffer_Ptr++;

	s_data.Index_Count += 6;
}

void Renderer::m_draw_rectangle_texture(const glm::vec2& position, const glm::vec2& size, const unsigned int texture_id)
{
	if (s_data.Index_Count >= m_MAX_INDICIES || s_data.current_texture_slot > 31)
	{
		end_batch();
		flush();
		begin_batch();
	}

	constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

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


	s_data.Object_Buffer_Ptr->position = { position.x - (size.x / 2.0f), position.y - (size.y / 2.0f) };
	s_data.Object_Buffer_Ptr->color = color;
	s_data.Object_Buffer_Ptr->texture_coord = { 0.0f, 0.0f };
	s_data.Object_Buffer_Ptr->tex_id = texture_index;
	s_data.Object_Buffer_Ptr++;

	s_data.Object_Buffer_Ptr->position = { position.x + (size.x / 2.0f), position.y - (size.y / 2.0f) };
	s_data.Object_Buffer_Ptr->color = color;
	s_data.Object_Buffer_Ptr->texture_coord = { 1.0f, 0.0f };
	s_data.Object_Buffer_Ptr->tex_id = texture_index;
	s_data.Object_Buffer_Ptr++;

	s_data.Object_Buffer_Ptr->position = { position.x + (size.x / 2.0f), position.y + (size.y / 2.0f) };
	s_data.Object_Buffer_Ptr->color = color;
	s_data.Object_Buffer_Ptr->texture_coord = { 1.0f, 1.0f };
	s_data.Object_Buffer_Ptr->tex_id = texture_index;
	s_data.Object_Buffer_Ptr++;

	s_data.Object_Buffer_Ptr->position = { position.x - (size.x / 2.0f), position.y + (size.y / 2.0f) };
	s_data.Object_Buffer_Ptr->color = color;
	s_data.Object_Buffer_Ptr->texture_coord = { 0.0f, 1.0f };
	s_data.Object_Buffer_Ptr->tex_id = texture_index;
	s_data.Object_Buffer_Ptr++;

	s_data.Index_Count += 6;
}

void Renderer::m_draw_box(const glm::vec2& centre, const glm::vec2& size, const float border_width, const glm::vec4 color)
{
	if (s_data.Index_Count + 4 >= m_MAX_INDICIES)
	{
		end_batch();
		flush();
		begin_batch();
	}

	m_draw_rectangle_color({ centre.x - (size.x / 2.0f) + (border_width / 2.0f), centre.y }, { border_width, size.y }, color);
	m_draw_rectangle_color({ centre.x + (size.x / 2.0f) - (border_width / 2.0f), centre.y }, { border_width, size.y }, color);
	m_draw_rectangle_color({ centre.x, centre.y + (size.y / 2.0f) - (border_width / 2.0f) }, { size.x - 2 * border_width, border_width }, color);
	m_draw_rectangle_color({ centre.x, centre.y - (size.y / 2.0f) + (border_width / 2.0f) }, { size.x - 2 * border_width, border_width }, color);
}





void Renderer::flush()
{
	for (unsigned int i = 0; i < s_data.texture_slots.size(); i++)
	{
		GlCall(glBindTextureUnit(i, s_data.texture_slots[i]));
	}

	
	
	s_data.quad_shader->bind();

	s_data.VA0->bind();
	s_data.IndexBuffer->bind();

	GlCall(glDrawElements(GL_TRIANGLES, s_data.Index_Count, GL_UNSIGNED_INT, nullptr));

	s_data.Index_Count = 0;

}
