#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Log.h"



class Renderer
{
private:
	struct shader_source
	{
		std::string vertex_source;
		std::string fragment_source;
	};
public:
	int run();

	static unsigned int create_shader(const std::string& vertex_shader, const std::string& fragment_shader);
	static unsigned int compile_shader(const std::string& source_shader, unsigned int);
	static void remove_shader(const unsigned int& program, const unsigned int& shader);
	static shader_source parse_shader(const std::string& filepath);
};

