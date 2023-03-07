#include "Texture.h"

//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image\stb_image.h"


GLuint Texture::Load_Texture(std::string path)
{
	int width, height, BPP;
	unsigned int renderer_id;
	stbi_set_flip_vertically_on_load(1);
	unsigned char* local_buffer = stbi_load(path.c_str(), &width, &height, &BPP, 4);

	GlCall(glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id));
	GlCall(glBindTexture(GL_TEXTURE_2D, renderer_id));

	GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, local_buffer));

	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GlCall(glGenerateTextureMipmap(renderer_id));

	GlCall(glBindTexture(GL_TEXTURE_2D, 0));

	//GlCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (local_buffer != nullptr)
	{
		stbi_image_free(local_buffer);
	}

	Log::variable<const std::string&>("Texture initialised", path);
	return renderer_id;
}

GLuint Texture::Create_Texture(unsigned int width, unsigned int height, unsigned int color, unsigned int& renderer_id)
{
	GlCall(glCreateTextures(GL_TEXTURE_2D, 1, &renderer_id));
	GlCall(glBindTexture(GL_TEXTURE_2D, renderer_id));


	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color));

	GlCall(glGenerateTextureMipmap(renderer_id));

	GlCall(glBindTexture(GL_TEXTURE_2D, 0));

	return renderer_id;
}

GLuint Texture::Create_Texture(unsigned int width, unsigned height, unsigned char* buffer)
{
	GLuint renderer_id = 0;
	GlCall(glGenTextures(1, &renderer_id));
	GlCall(glBindTexture(GL_TEXTURE_2D, renderer_id));

	GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, buffer));

	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	if (width != 0 && height != 0)
	{
		GlCall(glGenerateTextureMipmap(renderer_id));
	}

	GlCall(glBindTexture(GL_TEXTURE_2D, 0));

	return renderer_id;
}

void Texture::delete_texture(unsigned int id)
{
	Log::variable<unsigned int>("Texture deleted: ", id);
	GlCall(glDeleteTextures(1, &id));
}

void Texture::bind(GLuint id,unsigned int slot) 
{
	if (slot > MAX_TEXTURE_SLOTS - 1)
	{
		Log::crit("Trying to bind more than 32 textures!", __FILE__, __LINE__);
	}

	GlCall(glActiveTexture(GL_TEXTURE0 + slot));
	GlCall(glBindTexture(GL_TEXTURE_2D, id));
}

void Texture::unbind(unsigned int slot) 
{
	GlCall(glActiveTexture(GL_TEXTURE0 + slot));
	GlCall(glBindTexture(GL_TEXTURE_2D, 0));
}

