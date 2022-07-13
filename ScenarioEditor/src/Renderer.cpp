#include "Renderer.h"

struct render_data
{
	unsigned int texture_slot = 1;
	
	unsigned int White_Texture = 0;
	unsigned int White_Texture_Slot = 0;

	std::array<GLuint, Texture::MAX_TEXTURE_SLOTS> texture_slots;
};

static render_data s_data;

void Renderer::init(std::vector<std::string> textures)
{
	//shader setup
	int samplers[Texture::MAX_TEXTURE_SLOTS];
	for (int i = 0; i < Texture::MAX_TEXTURE_SLOTS; i++)
	{
		samplers[i] = i;
	}

	m_shader = std::make_unique<shader>("res/shaders/Texture.shader");
	//m_shader->set_uniform_4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
	glm::mat4 mvp = glm::ortho(0.0f, 1280.0f, 0.0f, 960.f, -1.0f, 1.0f);
	m_shader->set_uniform_mat_4f("u_MVP", mvp);
	m_shader->set_uniform_1i("u_texture", 1);

	//Texture tex("res/textures/ork.jpg");
	//s_data.texture_slots[1] = tex.get_id();
	//s_data.textures.push_back(std::make_unique<Texture>(tex));

	//s_data.textures.push_back(std::unique_ptr<Texture>(new Texture("res/shaders/Texture.shader")));
	//s_data.texture_slots[1] = s_data.textures[0]->get_id();

	unsigned int color = 0xffffffff;
	//Texture test(1, 1, color);
	//s_data.texture_slots[0] = test.get_id();
	//s_data.textures.push_back(std::unique_ptr<Texture>(new Texture(1, 1, color)));
	//s_data.texture_slots[0] = s_data.textures[1]->get_id();

	s_data.texture_slots[1] = Texture::Load_Texture("res/textures/ork.jpg");
	s_data.texture_slots[0] = Texture::Create_Texture(1,1,color,s_data.White_Texture);

	for (int i = 2; i < 32; i++)
	{
		s_data.texture_slots[i] = 0;
	}


	//Texture test(1, 1, color);
	//glCreateTextures(GL_TEXTURE_2D, 1, &s_data.White_Texture);
	//glBindTexture(GL_TEXTURE_2D, s_data.White_Texture);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);

	//s_data.texture_slots[0] = s_data.White_Texture;
	
	
	//s_data.texture_slots[1]->bind();
	//tex.bind();

	
}

void Renderer::shutdown()
{
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

void Renderer::draw(const Vertex_Array& vao, const Index_Buffer& ib)
{
	for (unsigned int i = 0; i < s_data.texture_slots.size(); i++)
	{
		if (s_data.texture_slots[i] != 0)
		{
			GlCall(glBindTextureUnit(i, s_data.texture_slots[i]));
		}
	}

	//GlCall(glBindTextureUnit(0, s_data.texture_slots[0]));
	//GlCall(glBindTextureUnit(1, s_data.texture_slots[1]));

	m_shader->bind();
	vao.bind();
	ib.bind();

	GlCall(glDrawElements(GL_TRIANGLES, ib.get_count(), GL_UNSIGNED_INT, nullptr));
}


void Renderer::flush()
{
}
