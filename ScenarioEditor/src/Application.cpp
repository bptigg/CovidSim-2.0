#include "Application.h"

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

#define FALSE 0
#define TRUE 1
 
app* app::s_instance = nullptr;

app::app()
    :m_file()
{
    m_camera = std::make_shared<Camera_Controller>(new Camera_Controller);

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
        //Camera(e);
        dispatch.Dispatch<Events::Window_Close_Event>(BIND_EVENT_FN(app::OnWindowClose));
        dispatch.Dispatch<Events::Window_Resize_Event>(BIND_EVENT_FN(app::OnWindowResize));
    }

    if (e.Get_Event_Type() == Events::Event_Type::GUI_Editor)
    {
        for (int i = 0; i < m_stack.size(); i++)
        {
            if (dynamic_cast<editor*>(m_stack[i]) != nullptr)
            {
                m_stack[i]->On_Attach({});
            }
        }
    }

    if (e.Get_Event_Type() == Events::Event_Type::GUI_Building_Select)
    {
        for (int i = 0; i < m_stack.size(); i++)
        {
            GUI_Layer* temp_layer = dynamic_cast<GUI_Layer*>(m_stack[i]);
            if (temp_layer != nullptr)
            {
               Events::GUI_Building_Select_Event* ev = dynamic_cast<Events::GUI_Building_Select_Event*>(&e);
               if (temp_layer->get_type() == GUI_Layer::Type::BuildingSelectMenu)
               {
                   m_stack[i]->On_Attach({});
                   temp_layer->set_caller(ev->get_caller());
                   m_stack[i]->render(true);
               }
            }
        }
    }

    if (e.Get_Event_Type() == Events::Event_Type::GUI_Size_Select)
    {
        for (int i = 0; i < m_stack.size(); i++)
        {
            GUI_Layer* temp_layer = dynamic_cast<GUI_Layer*>(m_stack[i]);
            if (temp_layer != nullptr)
            {
                Events::GUI_Building_Size_Event* ev = dynamic_cast<Events::GUI_Building_Size_Event*>(&e);
                if (temp_layer->get_type() == GUI_Layer::Type::BuildingSizeSubMenu)
                {
                    temp_layer->set_prev_menu(ev->get_menu_caller());
                    m_stack[i]->On_Attach({});
                    temp_layer->set_caller(ev->get_caller());
                    m_stack[i]->render(true);
                }
            }
        }
    }

    if (e.Get_Event_Type() == Events::Event_Type::GUI_Public_Select)
    {
        for (int i = 0; i < m_stack.size(); i++)
        {
            GUI_Layer* temp_layer = dynamic_cast<GUI_Layer*>(m_stack[i]);
            if (temp_layer != nullptr)
            {
                Events::GUI_Public_Building_Event* ev = dynamic_cast<Events::GUI_Public_Building_Event*>(&e);
                if (temp_layer->get_type() == GUI_Layer::Type::PublicBuildingSubMenu)
                {
                    m_stack[i]->On_Attach({});
                    temp_layer->set_menu(ev->get_menu());
                    temp_layer->set_caller(ev->get_caller());
                    m_stack[i]->render(true);
                }
            }
        }
    }

    if (e.Get_Event_Type() == Events::Event_Type::GUI_Dropdown)
    {
        for (int i = 0; i < m_stack.size(); i++)
        {
            GUI_Layer* temp_layer = dynamic_cast<GUI_Layer*>(m_stack[i]);
            if (temp_layer != nullptr)
            {
                Events::GUI_Dropdown_Event* ev = dynamic_cast<Events::GUI_Dropdown_Event*>(&e);
                if (temp_layer->get_type() == GUI_Layer::Type::ButtonDropDown)
                {
                    temp_layer->set_caller(ev->get_caller());
                    m_stack[i]->On_Attach({});
                    m_stack[i]->render(true);
                }
            }
        }
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
void app::push_layer(Layer* layer)
{
    m_stack.Push_Layer(layer);
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
    //m_camera.On_Event(e);
   // m_render.update_view(m_camera.get_camera().Get_View_Projection_Matrix());
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

    if (std::filesystem::exists("data") == FALSE)
    {
        std::cout << "data directory doesn't exist... creating one\n";
        std::filesystem::create_directory("data");
        if (std::filesystem::exists("data"))
            std::cout << "Created directory\n";
        std::cout << "Terminating Program" << std::endl;
        Log::info("TERMINATING PROGRAM DUE TO LACK OF DATA FOLDER");
        return;
    }

    m_camera->get_camera().Set_Position({ 0.0f,0.0f,0.0f });
    m_camera->Set_Resolution({ 1280.0f, 720.0f });
    Renderer::update_view(m_camera->get_camera().Get_View_Projection_Matrix());

    m_stack.Push_Layer(new GUI_Layer(GUI_Layer::Type::SetupMenu, 3, m_camera)); //WILL BE REMOVED AT SOMEPOINT
    m_stack[0]->Set_Event_Callback(BIND_EVENT_FN(app::On_Event));
    m_stack[0]->On_Attach({});

    m_stack.Push_Layer(new editor(1, m_camera));
    m_stack[1]->Set_Event_Callback(BIND_EVENT_FN(app::On_Event));

    m_stack.Push_Layer(new GUI_Layer(GUI_Layer::Type::BuildingSelectMenu, 3, m_camera));
    m_stack[2]->Set_Event_Callback(BIND_EVENT_FN(app::On_Event));

    m_stack.Push_Layer(new GUI_Layer(GUI_Layer::Type::PublicBuildingSubMenu, 3, m_camera));
    m_stack[3]->Set_Event_Callback(BIND_EVENT_FN(app::On_Event));

    m_stack.Push_Layer(new GUI_Layer(GUI_Layer::Type::PublicTransportSubMenu, 3, m_camera));
    m_stack[4]->Set_Event_Callback(BIND_EVENT_FN(app::On_Event));

    m_stack.Push_Layer(new GUI_Layer(GUI_Layer::Type::BuildingSizeSubMenu, 3, m_camera));
    m_stack[4]->Set_Event_Callback(BIND_EVENT_FN(app::On_Event));

    m_stack.Push_Layer(new GUI_Layer(GUI_Layer::Type::ButtonDropDown, 10, m_camera));
    m_stack[5]->Set_Event_Callback(BIND_EVENT_FN(app::On_Event));

    
    m_running = true;
}

void app::run()
{
    while (m_running)
    {
        m_render.clear();
        Renderer::update_view(m_camera->get_camera().Get_View_Projection_Matrix());

        float c_time = glfwGetTime();
        Timestep time = c_time - m_frame_time;
        m_frame_time = c_time;

        if(!m_minimized)
        {
            for (int i = 0; i < m_stack.size(); i++)
            {
                auto it = m_stack[i];
                it->On_Update(time);
                if (it->delete_layer == true)
                {
                    if (i == m_stack.size() - 1)
                    {
                        m_stack.Pop_Layer(it);
                        break;
                    }
                    m_stack.Pop_Layer(it);
                }
            }
            Renderer::draw();
        }

        m_window->On_Update();
    }
}

