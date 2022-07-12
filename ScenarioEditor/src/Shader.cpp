#include "Shader.h"

#include "gl_debug.h"

shader::shader(const std::string& filepath)
	:m_filepath(filepath), m_renderer_id(0)
{
    shader_source source = parse_shader(filepath);
    m_renderer_id = create_shader(source.vertex_source, source.fragment_source);
}

shader::~shader()
{
    GlCall(glDeleteProgram(m_renderer_id));
}

unsigned int shader::create_shader(const std::string& vertex_shader, const std::string& fragment_shader)
{
    GlCall(unsigned int program = glCreateProgram());
    unsigned int vs = compile_shader(vertex_shader, GL_VERTEX_SHADER);
    unsigned int fs = compile_shader(fragment_shader, GL_FRAGMENT_SHADER);

    GlCall(glAttachShader(program, vs));
    GlCall(glAttachShader(program, fs));
    GlCall(glLinkProgram(program));
    GlCall(glValidateProgram(program));

    unsigned int is_linked = 0;
    GlCall(glGetProgramiv(program, GL_LINK_STATUS, (int*)&is_linked));
    if (is_linked == GL_FALSE)
    {
        int max_length = 0;
        GlCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length));

        char* info_log = new char[max_length];
        GlCall(glGetProgramInfoLog(program, max_length, &max_length, &info_log[0]));

        GlCall(glDeleteProgram(program));

        GlCall(glDeleteShader(vs));
        GlCall(glDeleteShader(fs));

        Log::error(info_log, __FILE__, __LINE__);
        Log::crit("FAILED TO LOAD SHADERS", __FILE__, __LINE__);

        delete[] info_log;
    }


    remove_shader(program, vs);
    remove_shader(program, fs);

    return program;

}

unsigned int shader::compile_shader(const std::string& source, unsigned int type)
{
    GlCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GlCall(glShaderSource(id, 1, &src, nullptr));
    GlCall(glCompileShader(id));

    int result;
    GlCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int max_length;
        GlCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &max_length));

        char* info_log = new char[max_length];
        GlCall(glGetShaderInfoLog(id, max_length, &max_length, &info_log[0]));

        std::stringstream ss;
        ss << "Failed to compile"
            << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
            << "shader"
            << info_log << std::endl;

        Log::crit(ss.str().data(), __FILE__, __LINE__);
        delete[] info_log;

        GlCall(glDeleteShader(id));
        return 0;
    }

    return id;
}

void shader::remove_shader(const unsigned int& program, const unsigned int& shader)
{
    GlCall(glDetachShader(program, shader));
    GlCall(glDeleteShader(shader));
}

shader::shader_source shader::parse_shader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    if (!stream.good())
    {
        Log::error("Shader file not found", __FILE__, __LINE__);
        Log::variable("Shader filepath", filepath);

        return { "","" };
    }

    enum class shader_type
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    shader_type       type = shader_type::NONE;
    std::string       line = "";
    std::stringstream ss[2];

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = shader_type::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = shader_type::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() };
}

void shader::bind() const
{
    GlCall(glUseProgram(m_renderer_id));
}

void shader::unbind() const
{
    GlCall(glUseProgram(0));
}

void shader::set_uniform_1i(const std::string& name, int v0)
{
    bind();
    GlCall(glUniform1i(get_uniform_location(name), v0));
}

void shader::set_uniform_1f(const std::string& name, float v0)
{
    bind();
    GlCall(glUniform1f(get_uniform_location(name), v0));
}

void shader::set_uniform_4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    bind();
    GlCall(glUniform4f(get_uniform_location(name), v0, v1, v2, v3));
}

unsigned int shader::get_uniform_location(const std::string& name)
{
    if (m_uniform_cache.find(name) != m_uniform_cache.end())
    {
        return m_uniform_cache[name];
    }

    GlCall(int location = glGetUniformLocation(m_renderer_id, name.c_str()));
    if (location == -1)
    {
        std::stringstream ss;
        ss << "uniform " << name << " not found" << std::endl;
        Log::warning(ss.str().data());
    }
    else
    m_uniform_cache[name] = location;
    return location;
}
