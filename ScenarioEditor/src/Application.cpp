#include "Application.h"

#include <Windows.h>

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
    bool skip_gui = false;

    if (e.Get_Event_Type() == Events::Event_Type::App_Export)
    {
        dispatch.Dispatch<Events::AppExportEvent>(BIND_EVENT_FN(app::export_scenario));
    }

    for (auto it = m_stack.r_begin(); it < m_stack.rend(); ++it)
    {
        if ((*it)->dialog_box == true)
        {
            skip = true;
        }
        if ((*it)->text_box == true)
        {
            if (dynamic_cast<Events::Key_Pressed_Event*>(&e) != nullptr)
            {
                skip_gui = true;
            }
        }
    }

    if (!skip)
    {
        //Camera(e);
        dispatch.Dispatch<Events::Window_Close_Event>(BIND_EVENT_FN(app::OnWindowClose));
        dispatch.Dispatch<Events::Window_Resize_Event>(BIND_EVENT_FN(app::OnWindowResize));
    }

    if (!skip_gui)
    {
        if (e.Get_Event_Type() == Events::Event_Type::GUI_Editor)
        {
            for (int i = 0; i < m_stack.size(); i++)
            {
                if (dynamic_cast<editor*>(m_stack[i]) != nullptr)
                {
                    m_stack[i]->On_Attach({});
                    return;
                }
            }
        }

        if (e.Get_Event_Type() == Events::Event_Type::Transport_Overlay_Select)
        {
            for (int i = 0; i < m_stack.size(); i++)
            {
                auto overlay = dynamic_cast<Transport_Layer*>(m_stack[i]);
                if (overlay != nullptr)
                {
                    auto event = dynamic_cast<Events::Transport_Overlay_Event*>(&e);
                    if (event->get_enable())
                    {
                        if (overlay->get_attached())
                        {
                            overlay->enable_overlay();
                            return;
                        }
                        else
                        {
                            m_stack[i]->On_Attach({});
                            m_stack[i]->render(true);
                            return;
                        }
                    }
                    else
                    {
                        overlay->disable_overlay();
                        return;
                    }
                }
            }
        }

        if (e.Get_Event_Type() == Events::Event_Type::Education_Overlay_Select)
        {
            for (int i = 0; i < m_stack.size(); i++)
            {
                auto overlay = dynamic_cast<Education_Layer*>(m_stack[i]);
                if (overlay != nullptr)
                {
                    auto event = dynamic_cast<Events::Education_overlay_select*>(&e);
                    if (event->get_enable())
                    {
                        if (overlay->get_attached())
                        {
                            overlay->enable_overlay();
                            return;
                        }
                        else
                        {
                            m_stack[i]->On_Attach({});
                            m_stack[i]->render(true);
                            return;
                        }
                    }
                    else
                    {
                        overlay->disable_overlay();
                        return;
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
                        return;
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
                        return;
                    }
                }
            }
        }

        if (e.Get_Event_Type() == Events::Event_Type::GUI_Transport_Select)
        {
            for (int i = 0; i < m_stack.size(); i++)
            {
                GUI_Layer* temp_layer = dynamic_cast<GUI_Layer*>(m_stack[i]);
                if (temp_layer != nullptr)
                {
                    Events::GUI_Transport_Building_Event* ev = dynamic_cast<Events::GUI_Transport_Building_Event*>(&e);
                    if (temp_layer->get_type() == GUI_Layer::Type::PublicTransportSubMenu)
                    {
                        m_stack[i]->On_Attach({});
                        temp_layer->set_menu(ev->get_menu());
                        temp_layer->set_caller(ev->get_caller());
                        m_stack[i]->render(true);
                        return;
                    }
                }
            }
        }

        GUI_event<Events::GUI_Building_Select_Event>(e, GUI_Layer::Type::BuildingSelectMenu, Events::Event_Type::GUI_Building_Select);
        GUI_event<Events::GUI_Dropdown_Event>(e, GUI_Layer::Type::ButtonDropDown, Events::Event_Type::GUI_Dropdown);
        GUI_event<Events::Popup_Capacity_Event>(e, GUI_Layer::Type::CapacityPopup, Events::Event_Type::Popup_Capacity);
        GUI_event<Events::Popup_Opening_Event>(e, GUI_Layer::Type::OpeningPopup, Events::Event_Type::Popup_Opening);
        GUI_event<Events::Popup_Staff_Event>(e, GUI_Layer::Type::StaffPopup, Events::Event_Type::Popup_Staff);
        GUI_event<Events::GUI_Colour_Event>(e, GUI_Layer::Type::ColourSelect, Events::Event_Type::GUI_Colour_Select);

        GUI_event<Events::GUI_Settings_Event>(e, GUI_Layer::Type::SettingsMenu, Events::Event_Type::GUI_Settings_Select);
       
        if (e.Get_Event_Type() == Events::Event_Type::Transport_Line_Manager)
        {
            for (int i = 0; i < m_stack.size(); i++)
            {
                GUI_Layer* temp_layer = dynamic_cast<GUI_Layer*>(m_stack[i]);
                if (temp_layer != nullptr)
                {
                    Events::GUI_Line_Manager_Event* ev = dynamic_cast<Events::GUI_Line_Manager_Event*>(&e);
                    if (temp_layer->get_type() == GUI_Layer::Type::LineManager)
                    {
                        m_stack[i]->On_Attach({});
                        m_stack[i]->render(true);
                        temp_layer->set_call_layer(ev->get_layer());
                        temp_layer->update_line_manager();
                        return;
                    }
                }
            }
        }

        if (e.Get_Event_Type() == Events::Event_Type::Transport_Line_Editor)
        {
            Events::GUI_Line_Editor_Event* ev = dynamic_cast<Events::GUI_Line_Editor_Event*>(&e);
            if (ev->get_layer() != nullptr)
            {
                for (int i = 0; i < m_stack.size(); i++)
                {
                    GUI_Layer* temp_layer = dynamic_cast<GUI_Layer*>(m_stack[i]);
                    if (temp_layer != nullptr)
                    {
                        Events::GUI_Line_Editor_Event* ev = dynamic_cast<Events::GUI_Line_Editor_Event*>(&e);
                        if (temp_layer->get_type() == GUI_Layer::Type::LineEditor)
                        {
                            m_stack[i]->On_Attach({});
                            m_stack[i]->render(true);
                            temp_layer->set_call_layer(ev->get_layer());

                            if (ev->get_key() != "")
                            {
                                temp_layer->pload_line(ev->get_key());
                            }
                            return;
                        }
                    }
                }
            }
        }
    }

    //if (e.Get_Event_Type() == Events::Event_Type::GUI_Building_Select)
    //{
    //    for (int i = 0; i < m_stack.size(); i++)
    //    {
    //        GUI_Layer* temp_layer = dynamic_cast<GUI_Layer*>(m_stack[i]);
    //        if (temp_layer != nullptr)
    //        {
    //            Events::GUI_Building_Select_Event* ev = dynamic_cast<Events::GUI_Building_Select_Event*>(&e);
    //            if (temp_layer->get_type() == GUI_Layer::Type::BuildingSelectMenu)
    //            {
    //                m_stack[i]->On_Attach({});
    //                temp_layer->set_caller(ev->get_caller());
    //                m_stack[i]->render(true);
    //            }
    //        }
    //    }
    //}

    //if (e.Get_Event_Type() == Events::Event_Type::GUI_Dropdown)
    //{
    //    for (int i = 0; i < m_stack.size(); i++)
    //    {
    //        GUI_Layer* temp_layer = dynamic_cast<GUI_Layer*>(m_stack[i]);
    //        if (temp_layer != nullptr)
    //        {
    //            Events::GUI_Dropdown_Event* ev = dynamic_cast<Events::GUI_Dropdown_Event*>(&e);
    //            if (temp_layer->get_type() == GUI_Layer::Type::ButtonDropDown)
    //            {
    //                temp_layer->set_caller(ev->get_caller());
    //                m_stack[i]->On_Attach({});
    //                m_stack[i]->render(true);
    //            }
    //        }
    //    }
    //}

    //if (e.Get_Event_Type() == Events::Event_Type::Popup_Capacity)
    //{
    //    for (int i = 0; i < m_stack.size(); i++)
    //    {
    //        GUI_Layer* temp_layer = dynamic_cast<GUI_Layer*>(m_stack[i]);
    //        if (temp_layer != nullptr)
    //        {
    //            Events::Popup_Capacity_Event* ev = dynamic_cast<Events::Popup_Capacity_Event*>(&e);
    //            if (temp_layer->get_type() == GUI_Layer::Type::CapacityPopup)
    //            {
    //                temp_layer->set_caller(ev->get_caller());
    //                m_stack[i]->On_Attach({});
    //                m_stack[i]->render(true);
    //            }
    //        }
    //    }
    //}

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

bool app::export_scenario(Events::AppExportEvent& e)
{
    file_data* scenario_file = &file_data::get_file();
    auto editor_overlay = editor::get();
    
    int grid_size = scenario_file->world_buildings.num_of_tiles;

    //scenario_file->world_buildings.transport

    for (auto tb : editor_overlay->get_world_data_list())
    {
        if (tb.second->building_type > 0)
        {
            std::pair<int, int> location;
            location.first = tb.first % grid_size;
            location.second = (tb.first - location.first) / grid_size;
            if (tb.second->transport_building)
            {
                std::vector<TRANSPORT_BUILDINGS> transport_types;
                for (auto type : tb.second->types)
                {
                    transport_types.push_back((TRANSPORT_BUILDINGS)type);
                }

                scenario_file->world_buildings.transport.push_back({ transport_types, location, (int)tb.second->staff, (int)tb.second->capacity, (transport_types.size() > 1) ? true : false });
            }
            if (tb.second->education_building)
            {

            }
            else
            {
                scenario_file->world_buildings.pub_buildings.push_back({ tb.second->building_type, location, (int)tb.second->staff, (int)tb.second->capacity });
            }
        }
    }

    return true;
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
        else
        {
            std::cout << "Terminating Program" << std::endl;
            Log::info("TERMINATING PROGRAM DUE TO LACK OF DATA FOLDER");
            return;
        }
    }

    if (std::filesystem::exists("user modifiable files") == FALSE)
    {
        std::cout << "user modifiable files directory doesn't exist... creating one\n";
        std::filesystem::create_directory("user modifiable files");
        if (std::filesystem::exists("user modifiable files"))
        {
            std::cout << "Created directory\n";

            //const wchar_t* src_url = L"https://raw.githubusercontent.com/bptigg/CovidSim/master/Scenarios/EMC.cvsn";
            //const wchar_t* dest_file = L"test.cvsn";

            //if (S_OK == URLDownloadToFile(NULL, src_url, dest_file, 0, NULL))
            //{
            //
            //    printf("Saved to 'myfile.txt'");
            //
            //}

        }
        else
        {
            std::cout << "Terminating Program" << std::endl;
            Log::info("TERMINATING PROGRAM DUE TO LACK OF USER MODIFIABLE FILES FOLDER");
            return;
        }
    }

    m_camera->get_camera().Set_Position({ 0.0f,0.0f,0.0f });
    m_camera->Set_Resolution({ 1280.0f, 720.0f });
    Renderer::update_view(m_camera->get_camera().Get_View_Projection_Matrix());

    m_stack.Push_Layer(new editor(1, m_camera));
    m_stack[0]->Set_Event_Callback(BIND_EVENT_FN(app::On_Event));

    m_stack.Push_Layer(new GUI_Layer(GUI_Layer::Type::SetupMenu, 3, m_camera)); //WILL BE REMOVED AT SOMEPOINT
    m_stack[1]->Set_Event_Callback(BIND_EVENT_FN(app::On_Event));
    m_stack[1]->On_Attach({});

    m_stack.Push_Layer(new GUI_Layer(GUI_Layer::Type::BuildingSelectMenu, 3, m_camera));
    m_stack[2]->Set_Event_Callback(BIND_EVENT_FN(app::On_Event));

    m_stack.Push_Layer(new GUI_Layer(GUI_Layer::Type::PublicBuildingSubMenu, 3, m_camera));
    m_stack[3]->Set_Event_Callback(BIND_EVENT_FN(app::On_Event));

    m_stack.Push_Layer(new GUI_Layer(GUI_Layer::Type::PublicTransportSubMenu, 3, m_camera));
    m_stack[4]->Set_Event_Callback(BIND_EVENT_FN(app::On_Event));

    m_stack.Push_Layer(new GUI_Layer(GUI_Layer::Type::BuildingSizeSubMenu, 3, m_camera));
    m_stack[5]->Set_Event_Callback(BIND_EVENT_FN(app::On_Event));

    m_stack.Push_Layer(new GUI_Layer(GUI_Layer::Type::ButtonDropDown, 10, m_camera));
    m_stack[6]->Set_Event_Callback(BIND_EVENT_FN(app::On_Event));

    m_stack.Push_Layer(new GUI_Layer(GUI_Layer::Type::CapacityPopup, 13, m_camera));
    m_stack[7]->Set_Event_Callback(BIND_EVENT_FN(app::On_Event));

    m_stack.Push_Layer(new GUI_Layer(GUI_Layer::Type::StaffPopup, 13, m_camera));
    m_stack[8]->Set_Event_Callback(BIND_EVENT_FN(app::On_Event));

    m_stack.Push_Layer(new GUI_Layer(GUI_Layer::Type::OpeningPopup, 13, m_camera));
    m_stack[9]->Set_Event_Callback(BIND_EVENT_FN(app::On_Event));

    m_stack.Push_Layer(new GUI_Layer(GUI_Layer::Type::SettingsMenu, 18, m_camera));
    m_stack[10]->Set_Event_Callback(BIND_EVENT_FN(app::On_Event));

    m_stack.Push_Layer(new Transport_Layer(16, m_camera));
    m_stack[11]->Set_Event_Callback(BIND_EVENT_FN(app::On_Event));

    m_stack.Push_Layer(new GUI_Layer(GUI_Layer::Type::LineManager, 18, m_camera));
    m_stack[12]->Set_Event_Callback(BIND_EVENT_FN(app::On_Event));

    m_stack.Push_Layer(new GUI_Layer(GUI_Layer::Type::LineEditor, 18, m_camera));
    m_stack[13]->Set_Event_Callback(BIND_EVENT_FN(app::On_Event));
    
    m_stack.Push_Layer(new GUI_Layer(GUI_Layer::Type::ColourSelect, 18, m_camera));
    m_stack[14]->Set_Event_Callback(BIND_EVENT_FN(app::On_Event));

    m_stack.Push_Layer(new Education_Layer(16, m_camera));
    m_stack[15]->Set_Event_Callback(BIND_EVENT_FN(app::On_Event));

    
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

