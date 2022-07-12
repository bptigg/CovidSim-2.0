#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Log.h"
#include "gl_debug.h"

#include "Vertex_Buffer.h"
#include "Index_Buffer.h"
#include "Vertex_Array.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"

int main()
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
    {

        float positions[] = {
            -0.5f, -0.5f, 0.0f, 0.0f,
             0.5f, -0.5f, 1.0f, 0.0f,
             0.5f,  0.8f, 1.0f, 1.0f,
            -0.5f,  0.8f, 0.0f, 1.0f
        };

        unsigned int indices[] = {
            0,1,2,
            2,3,0
        };

        GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR));
        GlCall(glEnable(GL_BLEND));

        Vertex_Array va;
        Vertex_Buffer vb(positions, 4 * 4 * sizeof(float));
        Vertex_Buffer_Layout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.add_buffer(vb, layout);

        Index_Buffer ib(indices, 6);

        shader shader("res/shaders/Texture.shader");
        shader.bind();
        shader.set_uniform_4f("u_color", 0.8f, 0.3f, 0.8f, 1.0f);

        Texture tex("res/textures/ork.jpg");
        tex.bind();
        shader.set_uniform_1i("u_texture", 0);

        va.unbind();
        shader.unbind();
        vb.unbind();
        ib.unbind();

        Renderer render;

        float r = 0.0f;
        float increment = 0.05f;
        while (!glfwWindowShouldClose(window))
        {
            render.clear();

            shader.bind();
            shader.set_uniform_4f("u_Color", r, 0.3f, 0.5f, 1.0f);

            va.bind();
            ib.bind();

            render.draw(va, ib, shader);
            //GlCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

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

    }
    glfwTerminate();
    return 0;
}