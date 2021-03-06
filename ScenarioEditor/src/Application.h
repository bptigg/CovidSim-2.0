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
#include "Layers/editor.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include "file.h"
#include "Timestep.h"



class app
{
public:

    app();
    ~app();

    void On_Event(Events::Event& e);
private:
    bool m_running;
    bool m_minimized;
    
    Layer_Stack m_stack;

    float m_frame_time = 0.0f;
   
    Renderer m_render;
    Window* m_window;
    
    std::shared_ptr<Camera_Controller> m_camera;
    file_data m_file;

    static app* s_instance;


public:

    static app& get() { return *s_instance; }

    bool OnWindowClose(Events::Window_Close_Event& e);
    bool OnWindowResize(Events::Window_Resize_Event& e);
    
    void Camera(Events::Event& e);
    
    void init();
    void run();

    Window& GetWindow() { return *m_window; }
    const Window& GetWindow() const { return *m_window; }

    void push_layer(Layer* layer);

};