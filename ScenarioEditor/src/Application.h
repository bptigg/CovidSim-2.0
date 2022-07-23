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

#include "Timestep.h"

class app
{
private:
    bool m_running = true;
    Renderer m_render;
    Window* m_window;
    Camera_Controller m_camera;

    static app* s_instance;

    Layer_Stack stack;

    float m_frame_time = 0.0f;
public:
    app()
        :m_camera()
    {

    }

    static app& get() { return *s_instance; }

    void On_Event(Events::Event& e);
    bool OnWindowClose(Events::Window_Close_Event& e);
    void Camera(Events::Event& e);
    int loop();

    void mouse_click(Events::Mouse_Button_Pressed_Event& e);

    Window& GetWindow() { return *m_window; }

};