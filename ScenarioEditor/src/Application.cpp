#include "Application.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

//static std::array<std::unique_ptr<Texture>, Texture::MAX_TEXTURE_SLOTS> texture_slot;

app* app::s_instance = nullptr;

void app::On_Event(Events::Event& e)
{
    Events::Event_Dispatcher dispatch(e);
    dispatch.Dispatch<Events::Window_Close_Event>(BIND_EVENT_FN(app::OnWindowClose));

    for (auto it = stack.r_begin(); it < stack.rend(); ++it)
    {
        if (e.Handled)
        {
            break;
        }
        (*it)->On_Event(e);
    }

}
bool app::OnWindowClose(Events::Window_Close_Event& e)
{
    m_running = false;
    return true;
}
void app::Camera(Events::Event& e)
{
    m_camera.On_Event(e);
    m_render.update_view(m_camera.get_camera().Get_View_Projection_Matrix());
}


int app::loop()
{
    //GLFWwindow* window;

    /* Initialize the library */
    //if (!glfwInit())
       // return -1;

    //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    //window = glfwCreateWindow(1280, 960, "Hello World", NULL, NULL);
    //if (!window)
    //{
        //glfwTerminate();
        //return -1;
    //}

    /* Make the window's context current */
    //glfwMakeContextCurrent(window);
    //glfwSwapInterval(1);

    //if (glewInit() != GLEW_OK)
    {
        //Log::error("GLEW NOT INITLIZED", __FILE__, __LINE__);
    //};
        m_window = Window::Create();
        m_window->Set_Event_Callback(BIND_EVENT_FN(app::On_Event));

        m_render.init({ "res/textures/ork.jpg" });

        //m_camera.get_camera().Set_Position({ 0.0f,0.0f,0.0f });
        //m_camera.Set_Resolution({ m_window->Get_Width(),m_window->Get_Height() });
        //m_render.update_view(m_camera.get_camera().Get_View_Projection_Matrix());

        stack.Push_Layer(new Scenario_Editor());
        stack[0]->On_Attach({});

        s_instance = this;

        //Log::info((char*)glGetString(GL_VERSION));
        {

            float positions[] = {
                 100.0f, 100.0f, 0.0f, 0.0f,
                 340.0f, 100.0f, 1.0f, 0.0f,
                 340.0f, 420.0f, 1.0f, 1.0f,
                 100.0f, 420.0f, 0.0f, 1.0f
            };

            unsigned int indices[] = {
                0,1,2,
                2,3,0
            };

            //GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR));
            //GlCall(glEnable(GL_BLEND));

            //Vertex_Array va;
            {
                //Vertex_Buffer vb(positions, 4 * 4 * sizeof(float));
                //Vertex_Buffer_Layout layout;
                //layout.Push<float>(2);
                //layout.Push<float>(2);
                //layout.Push<float>(2);
                //layout.Push<float>(1);
                //va.add_buffer(vb, layout);
            }

            //Index_Buffer ib(6);

            //int width, height;
            //GlCall(glfwGetWindowSize(window, &width, &height));
            //glm::mat4 proj = glm::ortho(0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f); 
            //glm::mat4 view = glm::translate(glm::mat4(1.0f),glm::vec3(0, 0, 0)); //moving camera to left 
            //glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)); //moving model to the right and up

            //glm::mat4 mvp = proj * model;
            //glm::mat4 mvp = model;

            //shader shader("res/shaders/Texture.shader");
            //shader.bind();
            //shader.set_uniform_4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

            {
                //Texture tex("res/textures/ork.jpg");
                //tex.bind(1);
                //unsigned int color = 0xffffffff;
                //Texture test(1, 1, &color);
                //texture_slot[0] = std::make_unique<Texture>(tex);
                //texture_slot[1] = std::make_unique<Texture>(test);
            }
            //test.bind(1);
            //shader.set_uniform_1i("u_texture", 0);
           //shader.set_uniform_mat_4f("u_MVP", mvp);

            //va.unbind();
            //shader.unbind();
            //vb.unbind();
            //ib.unbind();

           
            //m_render.init({ "res/textures/ork.jpg" });



            //ImGui::CreateContext();
            //ImGui_ImplGlfwGL3_Init(m_window->Get_Native_Window(), true);
            //ImGui::StyleColorsDark();

            float r = 0.0f;
            float increment = 0.05f;

            bool show_demo_window = true;
            bool show_another_window = false;
            ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

            //while (!glfwWindowShouldClose(window))
           
            while (m_running)
            {
                m_render.clear();

                //ImGui_ImplGlfwGL3_NewFrame();

                {
                //    static float f = 0.0f;
                //    static int counter = 0;
                    //ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
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
                   //::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                }
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

                //shader.bind();
                //shader.set_uniform_4f("u_color", r, 0.3f, 0.5f, 1.0f);

                //va.bind();
                //ib.bind();

                //render.draw(va, ib);
                //GlCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
                float c_time = glfwGetTime();
                Timestep time = c_time - m_frame_time;
                m_frame_time = c_time;
                
                //m_camera.On_Update(time);
                //m_render.draw();

                for (auto it = stack.begin(); it < stack.end(); it++)
                {
                    (*it)->On_Update(time);
                }

                m_window->On_Update();

                if (r > 1.0f)
                {
                    increment = -0.05f;
                }
                else if (r < 0.0f)
                {
                    increment = 0.05f;
                }

                r = r + increment;

                //ImGui::Render();
                //ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

                //GlCall(glfwSwapBuffers(window));

                //GlCall(glfwPollEvents());
            }
            m_render.shutdown();
        }
        //ImGui_ImplGlfwGL3_Shutdown();
        //ImGui::DestroyContext();
        glfwTerminate();
        return 0;
    }
}

