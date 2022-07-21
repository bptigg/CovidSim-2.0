#include "Text_Box.h"

Text_Box::Text_Box(const glm::vec2& position, const glm::vec2& size, Layer* layer, Text text)
    :scriptable_object(position, size, layer), m_text(text)
{
    m_over = false;
    m_selected = false;
}

Text_Box::~Text_Box()
{
}

void Text_Box::update()
{
}

void Text_Box::render()
{
}

void Text_Box::event_callback(Events::Event& e)
{
}

bool Text_Box::on_mouse_move(Events::Mouse_Moved_Event& e)
{
    return false;
}

bool Text_Box::on_mouse_click(Events::Mouse_Button_Pressed_Event& e)
{
    return false;
}

bool Text_Box::on_keyboard_press(Events::Key_Pressed_Event& e)
{
    return false;
}

void update()
{
}

void render()
{
}

void event_callback(Events::Event& e)
{
}

bool on_mouse_click(Events::Mouse_Button_Pressed_Event& e)
{
    return false;
}

bool on_keyboard_press(Events::Key_Pressed_Event& e)
{
    return false;
}
