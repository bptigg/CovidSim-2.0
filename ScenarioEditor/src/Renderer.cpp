#include "Renderer.h"

int Renderer::run()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 640, "Hello World", NULL, NULL);
    if (!window) 
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        Log::error("GLEW NOT INITLIZED", __FILE__, __LINE__);
    };

    Log::info((char*)glGetString(GL_VERSION));


    float positions[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f,
    };

    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };

    unsigned int vao;
    GlCall(glGenVertexArrays(1, &vao));
    GlCall(glBindVertexArray(vao));

    unsigned int buffer;
    GlCall(glGenBuffers(1, &buffer));
    GlCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GlCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW));

    GlCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
    GlCall(glEnableVertexAttribArray(0));

    unsigned int ibo;
    GlCall(glGenBuffers(1, &ibo));
    GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

    shader_source source = parse_shader("res/shaders/Basic.shader");

    unsigned int shader = create_shader(source.vertex_source, source.fragment_source);
    GlCall(glUseProgram(shader));

    GlCall(int location = glGetUniformLocation(shader, "u_Color"));
    ASSERT(location != -1);

    GlCall(glBindVertexArray(0));
    GlCall(glUseProgram(0));
    GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));


    float r = 0.0f;
    float increment = 0.05f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        
        GlCall(glClear(GL_COLOR_BUFFER_BIT));
        
        GlCall(glUseProgram(shader));
        GlCall(glUniform4f(location, r, 0.3f, 0.5f, 1.0f));
        
        GlCall(glBindVertexArray(vao));
        GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

        GlCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        if (r > 1.0f)
        {
            increment = -0.05f;
        }
        else if (r < 0.0f)
        {
            increment = 0.05f;
        }
        
        r = r + increment;
        
        GlCall(glfwSwapBuffers(window));

        GlCall(glfwPollEvents());
    }

    GlCall(glDeleteProgram(shader));

    glfwTerminate();
    return 0;
}

unsigned int Renderer::create_shader(const std::string& vertex_shader, const std::string& fragment_shader)
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

unsigned int Renderer::compile_shader(const std::string& source, unsigned int type)
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

void Renderer::remove_shader(const unsigned int& program, const unsigned int& shader)
{
    GlCall(glDetachShader(program, shader));
    GlCall(glDeleteShader(shader));
}

Renderer::shader_source Renderer::parse_shader(const std::string& filepath)
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
