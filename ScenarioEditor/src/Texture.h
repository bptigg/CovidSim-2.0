#pragma once

#include <string>

#include "gl_debug.h"


class Texture
{
public:
	static const unsigned int MAX_TEXTURE_SLOTS = 32;
public:

	static GLuint Load_Texture(std::string path);
	static GLuint Create_Texture(unsigned int width, unsigned int height, unsigned int color, unsigned int& renderer_id);
	
	static void delete_texture(unsigned int id);

	static void bind(GLuint id, unsigned int slot = 0) ;
	static void unbind(unsigned int slot) ;

};

