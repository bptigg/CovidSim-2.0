#include "Application.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

#define FALSE 0
#define TRUE 1

app* app::s_instance = nullptr;

app::app()
    :m_camera(), m_file()
{
    if (s_instance != nullptr)
    {
        Log::crit("Applicaiton already exists", __FILE__, __LINE__);
    }
    
    s_instance = this;

    m_window = Window::Create();
    m_window->Set_Event_Callback(BIND_EVENT_FN(app::On_Event));

    Renderer::init();
}

app::~app()
{
    Renderer::shutdown();
    glfwTerminate();
}

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

    if (e.GetWidth() == 0 || e.GetHeight() == 0)
    {
        m_minimized = true;
        return false;
    }

    m_minimized = false;

    GlCall(glViewport(0, 0, e.GetWidth(), e.GetHeight()));

   //GlCall(glMatrixMode(GL_PROJECTION));
    //GlCall(glLoadIdentity());
    //GlCall(glOrtho(0.0f, e.GetWidth(), 0.0f, e.GetHeight(), -1.0f, 1.0f));

    return false;
}
void app::Camera(Events::Event& e)
{
    m_camera.On_Event(e);
    m_render.update_view(m_camera.get_camera().Get_View_Projection_Matrix());
}

void app::init()
{
    if (std::filesystem::exists("scenarios") == FALSE)
    {
        std::cout << "scenarios directory doesn't exist... creating one\n";
        std::filesystem::create_directory("scenarios");
        if (std::filesystem::exists("scenarios"))
            std::cout << "Created directory\n";
    }

    m_camera.get_camera().Set_Position({ 0.0f,0.0f,0.0f });
    m_camera.Set_Resolution({ 1280.0f, 720.0f });
    Renderer::update_view(m_camera.get_camera().Get_View_Projection_Matrix());

    m_stack.Push_Layer(new GUI_Layer(GUI_Layer::Type::SetupMenu, 3, std::make_shared<Camera_Controller>(m_camera))); //WILL BE REMOVED AT SOMEPOINT
    m_stack[0]->Set_Event_Callback(BIND_EVENT_FN(app::On_Event));
    m_stack[0]->On_Attach({});

    m_running = true;
}

void app::run()
{
    while (m_running)
    {
        m_render.clear();

        float c_time = glfwGetTime();
        Timestep time = c_time - m_frame_time;
        m_frame_time = c_time;

        if(!m_minimized)
        {
            for (auto it = m_stack.begin(); it < m_stack.end(); it++)
            {
                (*it)->On_Update(time);
            }
        }

        m_window->On_Update();
    }
}

