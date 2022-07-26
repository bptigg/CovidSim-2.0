#pragma once

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Window.h"

#include "Log.h"
#include "gl_debug.h"

#include "Vertex_Buffer.h"
#include "Index_Buffer.h"
#include "Vertex_Array.h"
#include "Shader.h"
#include "Renderer.h"
#include "Texture.h"

#include "Camera/Camera_Controller.h"

#include "Layer_Stack.h"
#include "Scenario_Editor.h"
#include "Layers/GUI_Layer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "buildings_data.h"
#include "Timestep.h"

struct file_data
{
    bool file_exists;
    std::fstream scenario_file;
    std::filesystem::path m_filepath;

    struct scenario_data
    {
        std::string name;
        unsigned int max_counts;

        std::string population_pyramid;
        std::string population_race_dist;

        std::string adult_medical_data;
        std::string child_medical_data;
    };

    struct buildings
    {
        std::vector<education_buildings> schools;
        std::vector<public_buildings> pub_buildings;
        std::vector<transport_buildings> transport;
    };
};

class app
{
public:
    app()
        :m_camera()
    {
    }

    ~app()
    {

    }

    void On_Event(Events::Event& e);
private:
    bool m_running = true;
    Renderer m_render;
    Window* m_window;
    Camera_Controller m_camera;

    static app* s_instance;
    static file_data* f_instance;

    Layer_Stack m_stack;

    float m_frame_time = 0.0f;
public:

    static app& get() { return *s_instance; }
    static file_data& get_file() { return *f_instance; }

    //void On_Event(Events::Event& e);
    bool OnWindowClose(Events::Window_Close_Event& e);
    bool OnWindowResize(Events::Window_Resize_Event& e);
    void Camera(Events::Event& e);
    int loop();

    Window& GetWindow() { return *m_window; }
    const Window& GetWindow() const { return *m_window; }

};