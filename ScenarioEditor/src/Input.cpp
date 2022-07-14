#include "Input.h"

bool Input::Is_Key_Pressed(int key)
{
    return false;
}

bool Input::Is_MouseButton_Pressed(int button)
{
    return false;
}

glm::vec2 Input::Get_Mouse_Position()
{
    return glm::vec2();
}

float Input::Get_Mouse_X()
{
    return Get_Mouse_Position().x;
}

float Input::Get_Mouse_Y()
{
    return Get_Mouse_Position().y;
}
