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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

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
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
             -50.0f, -50.0f, 0.0f, 0.0f,
              50.0f, -50.0f, 1.0f, 0.0f,
              50.0f,  50.0f, 1.0f, 1.0f,
             -50.0f,  50.0f, 0.0f, 1.0f
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

        int width, height;
        GlCall(glfwGetWindowSize(window, &width, &height));
        glm::mat4 proj = glm::ortho(0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f); 
        glm::mat4 view = glm::translate(glm::mat4(1.0f),glm::vec3(0, 0, 0)); //moving camera to left 
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); //moving model to the right and up

        glm::mat4 mvp = proj * view * model;

        shader shader("res/shaders/Texture.shader");
        shader.bind();
        shader.set_uniform_4f("u_color", 0.8f, 0.3f, 0.8f, 1.0f);

        Texture tex("res/textures/ork.jpg");
        tex.bind();
        shader.set_uniform_1i("u_texture", 0);
        shader.set_uniform_mat_4f("u_MVP", mvp);

        va.unbind();
        shader.unbind();
        vb.unbind();
        ib.unbind();

        Renderer render;
        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();

        float r = 0.0f;
        float increment = 0.05f;

        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        while (!glfwWindowShouldClose(window))
        {
            render.clear();

            ImGui_ImplGlfwGL3_NewFrame();

            //{
            //    static float f = 0.0f;
            //    static int counter = 0;
            //    ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
            //    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
            //    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
            //
            //    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
            //    ImGui::Checkbox("Another Window", &show_another_window);
            //
            //    if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
            //        counter++;
            //    ImGui::SameLine();
            //    ImGui::Text("counter = %d", counter);
            //
            //    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            //}
            //
            //// 2. Show another simple window. In most cases you will use an explicit Begin/End pair to name your windows.
            //if (show_another_window)
            //{
            //    ImGui::Begin("Another Window", &show_another_window);
            //    ImGui::Text("Hello from another window!");
            //    if (ImGui::Button("Close Me"))
            //        show_another_window = false;
            //    ImGui::End();
            //}
            //
            //// 3. Show the ImGui demo window. Most of the sample code is in ImGui::ShowDemoWindow(). Read its code to learn more about Dear ImGui!
            //if (show_demo_window)
            //{
            //    ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver); // Normally user code doesn't need/want to call this because positions are saved in .ini file anyway. Here we just want to make the demo initial state a bit more friendly!
            //    ImGui::ShowDemoWindow(&show_demo_window);
            //}

            shader.bind();
            shader.set_uniform_4f("u_color", r, 0.3f, 0.5f, 1.0f);

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

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            GlCall(glfwSwapBuffers(window));

            GlCall(glfwPollEvents());
        }

    }
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}