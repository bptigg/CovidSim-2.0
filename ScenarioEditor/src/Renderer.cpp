#include "Renderer.h"

#include <ft2build.h>
#include FT_FREETYPE_H

static const unsigned int m_MAX_QUADS = 10000;
static const unsigned int m_MAX_VERTICIES = m_MAX_QUADS * 4;
static const unsigned int m_MAX_INDICIES = m_MAX_QUADS * 6;

struct render_data
{
	unsigned int White_Texture = 0;
	unsigned int White_Texture_Slot = 0;

	shader* quad_shader = nullptr;
	shader* text_shader = nullptr;

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

	std::unordered_map<char, Character> characters;
	bool base_uniforms = false;
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

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
	{
		Log::crit("FREETYPE: Could not initlized free", __FILE__, __LINE__);
	}

	FT_Face face;
	if (FT_New_Face(ft, "res/fonts/HPSimplified_Rg.ttf", 0, &face))
	{
		Log::crit("FREETYPE: Failed to load font", __FILE__, __LINE__);
	}

	FT_Set_Pixel_Sizes(face, 0, 48);
	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
	{
		Log::crit("FREETYPE: Failed to load glyph", __FILE__, __LINE__);
	}
	GlCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

	for (unsigned char c = 0; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			Log::warning("FREETYPE: Failed to load glyph");
			continue;
		}

		Character character = {
			0,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(uint16_t)face->glyph->advance.x,
		};

		s_data.characters[c] = character;
		s_data.characters[c].tex_id = Texture::Create_Texture(face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.buffer);
	}

	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	Log::info("Text initialised");

	s_data.Object_Buffer = new Vertex[m_MAX_VERTICIES];

	int samplers[Texture::MAX_TEXTURE_SLOTS];
	for (int i = 0; i < Texture::MAX_TEXTURE_SLOTS; i++)
	{
		samplers[i] = i;
	}

	GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	s_data.quad_shader = new shader("res/shaders/simple.shader");
	s_data.quad_shader->set_uniform_1iv("u_Textures", 32, samplers);
	s_data.IndexBuffer = new Index_Buffer(m_MAX_INDICIES);

	s_data.text_shader = new shader("res/shaders/Text.shader");
	s_data.text_shader->set_uniform_1iv("u_Textures", 32, samplers);
	
	s_data.VertexBuffer = new Vertex_Buffer(m_MAX_VERTICIES * sizeof(Vertex));
	s_data.VA0 = new Vertex_Array();
	{
		Vertex_Buffer_Layout layout;
		layout.Push<float>(2);
		layout.Push<float>(4);
		layout.Push<float>(2);
		layout.Push<float>(1);
		layout.Push<float>(1);
		s_data.VA0->add_buffer(*s_data.VertexBuffer, layout);
	}

	glm::mat4 mvp = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	//glm::vec4 zoom = glm::vec4(1.0f);
	
	s_data.quad_shader->set_uniform_mat_4f("u_View_Proj", mvp);
	s_data.quad_shader->set_uniform_mat_4f("u_Transform", transform);
	s_data.quad_shader->set_uniform_mat_4f("u_og_View_Proj", mvp);
	s_data.quad_shader->set_uniform_mat_4f("u_og_Transform", transform);

	s_data.text_shader->set_uniform_mat_4f("u_MVP", mvp);
	s_data.text_shader->set_uniform_4f("u_Zoom", 1.0f, 1.0f, 1.0f, 1.0f);
	s_data.text_shader->set_uniform_mat_4f("u_og_MVP", mvp);
	

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
	delete s_data.text_shader;
	
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
		if (!manager.empty)
		{
			begin_batch();
			for (int i = 0; i < draw->second.size(); i++)
			{
				switch (draw->second[i]->type)
				{
				case render_type::COLOURED_RECTANGLE:
					m_draw_rectangle_color(draw->second[i]->position, draw->second[i]->size, draw->second[i]->color, draw->second[i]->static_obj);
					break;
				case render_type::TEXTURED_RECTANGLE:
					m_draw_rectangle_texture(draw->second[i]->position, draw->second[i]->size, draw->second[i]->texture_id, draw->second[i]->static_obj);
					break;
				case render_type::COLOURED_BOX:
					m_draw_box(draw->second[i]->position, draw->second[i]->size, draw->second[i]->border_width, draw->second[i]->color, draw->second[i]->static_obj);
					break;
				case render_type::TEXT:
					end_batch();
					s_data.quad_shader->bind();
					flush();
					m_draw_text(draw->second[i]->text, draw->second[i]->position, draw->second[i]->size, draw->second[i]->color, draw->second[i]->scale, draw->second[i]->static_obj, draw->second[i]->centred, draw->second[i]->text_width);
					break;
				default:
					Log::warning("UNKOWN DRAW TYPE");
					break;
				}
			}
			if (manager.next_layer)
			{
				end_batch();
				s_data.quad_shader->bind();
				flush();
			}
		}
	}

	manager.delete_queue();
	s_data.current_texture_slot = 1;

	//end_batch();

	//flush();


}

void Renderer::update_view(const glm::mat4& projection_view_matrix)
{
	s_data.quad_shader->set_uniform_mat_4f("u_View_Proj", projection_view_matrix);
	s_data.text_shader->set_uniform_mat_4f("u_MVP", projection_view_matrix);

	if (!s_data.base_uniforms)
	{
		s_data.quad_shader->set_uniform_mat_4f("u_og_View_Proj", projection_view_matrix);
		s_data.text_shader->set_uniform_mat_4f("u_og_MVP", projection_view_matrix);
		s_data.base_uniforms = true;
	}
}

void Renderer::draw_rectangle_color(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, unsigned int layer, bool static_obj)
{
	manager.draw_rectangle_color(position, size, color, layer, static_obj);
}

void Renderer::draw_rectangle_texture(const glm::vec2& position, const glm::vec2& size, const unsigned int index, unsigned int layer, bool static_obj)
{
	manager.draw_rectangle_texture(position, size, index, layer, static_obj);
}

void Renderer::draw_box(const glm::vec2& centre, const glm::vec2& size, const float border_width, const glm::vec4 color, unsigned int layer, bool static_obj)
{
	manager.draw_box(centre, size, border_width, color, layer, static_obj);
}

void Renderer::draw_text(std::string& text, const glm::vec2 centre, const glm::vec4& color, unsigned int layer, float scale, bool static_obj, bool centred, float* width)
{
	float text_width	= 0.0f;
	float text_height	= 0.0f;
	float min_y			= 0.0f;
	float new_scale		= scale / 100;
	for (std::string::const_iterator c = text.begin(); c != text.end(); c++)
	{
		Character* ch = &s_data.characters[*c];
		text_width += (ch->advance >> 6) * new_scale;
		float h = ch->bearing.y * new_scale;
		if (h > text_height)
		{
			text_height = h;
		}
		float y = (ch->bearing.y - ch->size.y) * new_scale;
		if (y < min_y)
		{
			min_y = y;
		}
	}

	manager.draw_text(text, centre, { text_width, text_height - min_y }, color, layer, scale, static_obj, centred, width);
}

void Renderer::m_draw_rectangle_color(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, float static_obj)
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
	s_data.Object_Buffer_Ptr->static_obj = static_obj;
	s_data.Object_Buffer_Ptr++;

	s_data.Object_Buffer_Ptr->position = { position.x + (size.x / 2.0f), position.y - (size.y / 2.0f) };
	s_data.Object_Buffer_Ptr->color = color;
	s_data.Object_Buffer_Ptr->texture_coord = { 1.0f, 0.0f};
	s_data.Object_Buffer_Ptr->tex_id = texture_index;
	s_data.Object_Buffer_Ptr->static_obj = static_obj;
	s_data.Object_Buffer_Ptr++;

	s_data.Object_Buffer_Ptr->position = { position.x + (size.x / 2.0f), position.y + (size.y / 2.0f) };
	s_data.Object_Buffer_Ptr->color = color;
	s_data.Object_Buffer_Ptr->texture_coord = { 1.0f, 1.0f };
	s_data.Object_Buffer_Ptr->tex_id = texture_index;
	s_data.Object_Buffer_Ptr->static_obj = static_obj;
	s_data.Object_Buffer_Ptr++;

	s_data.Object_Buffer_Ptr->position = { position.x - (size.x/ 2.0f), position.y + (size.y / 2.0f) };
	s_data.Object_Buffer_Ptr->color = color;
	s_data.Object_Buffer_Ptr->texture_coord = { 0.0f, 1.0f };
	s_data.Object_Buffer_Ptr->tex_id = texture_index;
	s_data.Object_Buffer_Ptr->static_obj = static_obj;
	s_data.Object_Buffer_Ptr++;

	s_data.Index_Count += 6;
}

void Renderer::m_draw_rectangle_texture(const glm::vec2& position, const glm::vec2& size, const unsigned int texture_id, float static_obj)
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
	s_data.Object_Buffer_Ptr->static_obj = static_obj;
	s_data.Object_Buffer_Ptr++;

	s_data.Object_Buffer_Ptr->position = { position.x + (size.x / 2.0f), position.y - (size.y / 2.0f) };
	s_data.Object_Buffer_Ptr->color = color;
	s_data.Object_Buffer_Ptr->texture_coord = { 1.0f, 0.0f };
	s_data.Object_Buffer_Ptr->tex_id = texture_index;
	s_data.Object_Buffer_Ptr->static_obj = static_obj;
	s_data.Object_Buffer_Ptr++;

	s_data.Object_Buffer_Ptr->position = { position.x + (size.x / 2.0f), position.y + (size.y / 2.0f) };
	s_data.Object_Buffer_Ptr->color = color;
	s_data.Object_Buffer_Ptr->texture_coord = { 1.0f, 1.0f };
	s_data.Object_Buffer_Ptr->tex_id = texture_index;
	s_data.Object_Buffer_Ptr->static_obj = static_obj;
	s_data.Object_Buffer_Ptr++;

	s_data.Object_Buffer_Ptr->position = { position.x - (size.x / 2.0f), position.y + (size.y / 2.0f) };
	s_data.Object_Buffer_Ptr->color = color;
	s_data.Object_Buffer_Ptr->texture_coord = { 0.0f, 1.0f };
	s_data.Object_Buffer_Ptr->tex_id = texture_index;
	s_data.Object_Buffer_Ptr->static_obj = static_obj;
	s_data.Object_Buffer_Ptr++;

	s_data.Index_Count += 6;
}

void Renderer::m_draw_box(const glm::vec2& centre, const glm::vec2& size, const float border_width, const glm::vec4 color, float static_obj)
{
	if (s_data.Index_Count + 4 >= m_MAX_INDICIES)
	{
		end_batch();
		flush();
		begin_batch();
	}

	m_draw_rectangle_color({ centre.x - (size.x / 2.0f) + (border_width / 2.0f), centre.y }, { border_width, size.y }, color, static_obj);
	m_draw_rectangle_color({ centre.x + (size.x / 2.0f) - (border_width / 2.0f), centre.y }, { border_width, size.y }, color, static_obj);
	m_draw_rectangle_color({ centre.x, centre.y + (size.y / 2.0f) - (border_width / 2.0f) }, { size.x - 2 * border_width, border_width }, color, static_obj);
	m_draw_rectangle_color({ centre.x, centre.y - (size.y / 2.0f) + (border_width / 2.0f) }, { size.x - 2 * border_width, border_width }, color, static_obj);
}

void Renderer::m_draw_text(std::string& text, const glm::vec2& position, const glm::vec2& size, glm::vec4& color, float scale, float static_obj, bool centred, float* width)
{
	float x_offset = 0.0f;
	for (std::string::const_iterator c = text.begin(); c != text.end(); c++)
	{
		begin_batch();
		Character* ch = &s_data.characters[*c];

		if (s_data.Index_Count >= m_MAX_INDICIES || s_data.current_texture_slot > 31)
		{
			end_batch();
			flush();
			begin_batch();
		}

		float texture_id = ch->tex_id;

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

		std::array<Vertex, 4> vertecies = m_convert_character_to_vertices(ch, x_offset, texture_index, scale, text, position, size, color, centred);

		s_data.Object_Buffer_Ptr->position = vertecies[0].position;
		s_data.Object_Buffer_Ptr->color = vertecies[0].color;
		s_data.Object_Buffer_Ptr->texture_coord = vertecies[0].texture_coord;
		s_data.Object_Buffer_Ptr->tex_id = vertecies[0].tex_id;
		s_data.Object_Buffer_Ptr->static_obj = static_obj;
		s_data.Object_Buffer_Ptr++;

		s_data.Object_Buffer_Ptr->position = vertecies[1].position;
		s_data.Object_Buffer_Ptr->color = vertecies[1].color;
		s_data.Object_Buffer_Ptr->texture_coord = vertecies[1].texture_coord;
		s_data.Object_Buffer_Ptr->tex_id = vertecies[1].tex_id;
		s_data.Object_Buffer_Ptr->static_obj = static_obj;
		s_data.Object_Buffer_Ptr++;

		s_data.Object_Buffer_Ptr->position = vertecies[2].position;
		s_data.Object_Buffer_Ptr->color = vertecies[2].color;
		s_data.Object_Buffer_Ptr->texture_coord = vertecies[2].texture_coord;
		s_data.Object_Buffer_Ptr->tex_id = vertecies[2].tex_id;
		s_data.Object_Buffer_Ptr->static_obj = static_obj;
		s_data.Object_Buffer_Ptr++;

		s_data.Object_Buffer_Ptr->position = vertecies[3].position;
		s_data.Object_Buffer_Ptr->color = vertecies[3].color;
		s_data.Object_Buffer_Ptr->texture_coord = vertecies[3].texture_coord;
		s_data.Object_Buffer_Ptr->tex_id = vertecies[3].tex_id;
		s_data.Object_Buffer_Ptr->static_obj = static_obj;
		s_data.Object_Buffer_Ptr++;

		s_data.Index_Count += 6;
		
		float new_scale = scale / 100;
		x_offset += (ch->advance >> 6) * new_scale;
		if (width != nullptr)
		{
			*width = x_offset;
		}

		end_batch();
		s_data.text_shader->bind();
		GlCall(glEnable(GL_BLEND));
		flush();
		GlCall(glDisable(GL_BLEND));

	}
}

std::array<Vertex, 4> Renderer::m_convert_character_to_vertices(Character* ch, float x_offset, float tex_slot, float scale, std::string& text, const glm::vec2& position, const glm::vec2& size, glm::vec4& color, bool centred)
{
	float new_scale = scale / 100;

	float x_pos = position.x + ch->bearing.x * new_scale + x_offset;
	float y_pos = position.y - (ch->size.y - ch->bearing.y) * new_scale;

	float w = ch->size.x * new_scale;
	float h = ch->size.y * new_scale;

	glm::vec2 bottom, top;

	if (centred)
	{
		bottom.x = -size.x / 2.0f;
		bottom.y = -size.y / 2.0f;

		top.x = (-size.x / 2.0f) + w;
		top.y = (-size.y / 2.0f) + h;
	}
	else
	{
		bottom.x = 0;
		bottom.y = 0;

		top.x = w;
		top.y = h;
	}

	Vertex v0;
	v0.position = { x_pos + bottom.x, y_pos + top.y };
	v0.texture_coord = { 0.0f, 0.0f };
	v0.color = color;
	v0.tex_id = tex_slot;

	Vertex v1;
	v1.position = { x_pos + top.x, y_pos + top.y };
	v1.texture_coord = { 1.0f, 0.0f };
	v1.color = color;
	v1.tex_id = tex_slot;

	Vertex v2;
	v2.position = { x_pos + top.x, y_pos + bottom.y };
	v2.texture_coord = { 1.0f, 1.0f };
	v2.color = color;
	v2.tex_id = tex_slot;

	Vertex v3;
	v3.position = { x_pos + bottom.x, y_pos + bottom.y };
	v3.texture_coord = { 0.0f, 1.0f };
	v3.color = color;
	v3.tex_id = tex_slot;

	return { v0,v1,v2,v3 };
}


void Renderer::flush()
{
	for (unsigned int i = 0; i < s_data.texture_slots.size(); i++)
	{
		GlCall(glBindTextureUnit(i, s_data.texture_slots[i]));
	}

	
	
	//s_data.quad_shader->bind();

	s_data.VA0->bind();
	s_data.IndexBuffer->bind();

	GlCall(glDrawElements(GL_TRIANGLES, s_data.Index_Count, GL_UNSIGNED_INT, nullptr));

	s_data.Index_Count = 0;

}
