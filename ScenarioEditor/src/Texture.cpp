#include "Texture.h"

#include "gl_debug.h"

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image\stb_image.h"


Texture::Texture(const std::string& path)
	:m_renderer_id(0), m_filepath(path), m_local_buffer(nullptr), m_width(0), m_height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1);
	m_local_buffer = stbi_load(path.c_str(), &m_width, &m_height, &m_BPP, 4);

	GlCall(glGenTextures(1, &m_renderer_id));
	GlCall(glBindTexture(GL_TEXTURE_2D, m_renderer_id));

	GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_local_buffer));

	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GlCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_local_buffer)
	{
		stbi_image_free(m_local_buffer);
	}
}

Texture::~Texture()
{
	GlCall(glDeleteTextures(1, &m_renderer_id));
}

void Texture::bind(unsigned int slot) const
{
	if (slot > 31)
	{
		Log::crit("Trying to bind more than 32 textures!", __FILE__, __LINE__);
	}

	GlCall(glActiveTexture(GL_TEXTURE0 + slot));
	GlCall(glBindTexture(GL_TEXTURE_2D, m_renderer_id));
}

void Texture::unbind() const
{
	GlCall(glBindTexture(GL_TEXTURE_2D, 0));
}
