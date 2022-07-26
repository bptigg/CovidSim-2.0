#include "Application.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)


app* app::s_instance = nullptr;
file_data* app::f_instance = nullptr;

void app::On_Event(Events::Event& e)
{
    Events::Event_Dispatcher dispatch(e);

    bool skip = false;

    for (auto it = m_stack.r_begin(); it < m_stack.rend(); ++it)
    {
        if ((*it)->dialog_box == true)
        {
            skip = true;
        }

    }

    if (!skip)
    {
        dispatch.Dispatch<Events::Window_Close_Event>(BIND_EVENT_FN(app::OnWindowClose));
        dispatch.Dispatch<Events::Window_Resize_Event>(BIND_EVENT_FN(app::OnWindowResize));
    }

    for (auto it = m_stack.r_begin(); it < m_stack.rend(); ++it)
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

bool app::OnWindowResize(Events::Window_Resize_Event& e)
{
    GlCall(glViewport(0, 0, e.GetWidth(), e.GetHeight()));

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, e.GetWidth(), 0.0f, e.GetHeight(), -1.0f, 1.0f);

    return false;
}
void app::Camera(Events::Event& e)
{
    m_camera.On_Event(e);
    m_render.update_view(m_camera.get_camera().Get_View_Projection_Matrix());
}


int app::loop()
{
    {
        m_window = Window::Create();
        m_window->Set_Event_Callback(BIND_EVENT_FN(app::On_Event));

        m_render.init({ "res/textures/ork.jpg" });

        m_camera.get_camera().Set_Position({ 0.0f,0.0f,0.0f });
        m_camera.Set_Resolution({ 1280.0f, 720.0f });
        Renderer::update_view(m_camera.get_camera().Get_View_Projection_Matrix());

        m_stack.Push_Layer(new GUI_Layer(GUI_Layer::Type::SetupMenu, 3, std::make_shared<Camera_Controller>(m_camera)));
        m_stack[0]->Set_Event_Callback(BIND_EVENT_FN(app::On_Event));
        m_stack[0]->On_Attach({});

        s_instance = this;
        f_instance = &m_file;

        {
            while (m_running)
            {
                m_render.clear();

                float c_time = glfwGetTime();
                Timestep time = c_time - m_frame_time;
                m_frame_time = c_time;
                

                for (auto it = m_stack.begin(); it < m_stack.end(); it++)
                {
                    (*it)->On_Update(time);
                }

                m_window->On_Update();
                
            }
            m_render.shutdown();
        }
        glfwTerminate();
        return 0;
    }
}

