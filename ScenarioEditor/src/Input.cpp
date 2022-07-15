#include "Input.h"

#include "Application.h" //will need to change to applicaton.h

bool Input::Is_Key_Pressed(const int key)
{
    auto* window = static_cast<GLFWwindow*>(app::get().GetWindow().Get_Native_Window()); //defiently change
    auto state = glfwGetKey(window, key);
    return state == GLFW_PRESS;
}

bool Input::Is_MouseButton_Pressed(int button)
{
    auto* window = static_cast<GLFWwindow*>(app::get().GetWindow().Get_Native_Window()); //defiently change
    auto state = glfwGetKey(window, button);
    return state == GLFW_PRESS;
}

glm::vec2 Input::Get_Mouse_Position()
{
    auto* window = static_cast<GLFWwindow*>(app::get().GetWindow().Get_Native_Window());
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    return { (float)xpos, (float)ypos };
}

float Input::Get_Mouse_X()
{
    return Get_Mouse_Position().x;
}

float Input::Get_Mouse_Y()
{
    return Get_Mouse_Position().y;
}
