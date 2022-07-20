#include "Menu_Background.h"

Menu_Background::Menu_Background(glm::vec2 pos, glm::vec2 size, Layer* layer, glm::vec4 colour, const std::function<void()> exit_Func)
	:scriptable_object(pos,size,layer), m_background_color(colour), m_exit_func(exit_Func)
{
}

Menu_Background::~Menu_Background()
{
}

void Menu_Background::render()
{
	Renderer::draw_rectangle_color(m_location, m_size, m_background_color,3);
}

void Menu_Background::update()
{
}

void Menu_Background::event_callback(Events::Event& e)
{
	Events::Event_Dispatcher dispatcher(e);
	switch (e.Get_Event_Type())
	{
	case Events::Event_Type::Key_Pressed:
		dispatcher.Dispatch<Events::Key_Pressed_Event>(BIND_EVENT_FN(Menu_Background::on_keyboard_press));
		break;
	case Events::Event_Type::Mouse_Button_Pressed:
		dispatcher.Dispatch<Events::Mouse_Button_Pressed_Event>(BIND_EVENT_FN(Menu_Background::on_mouse_click));
		break;
	default:
		break;
	}
}

bool Menu_Background::is_mouse_over()
{
	glm::vec2 loc = { Input::Get_Mouse_X(), Input::Get_Mouse_Y() };
	return !(loc.x < m_location.x - m_size.x / 2.0f || loc.x > m_location.x + m_size.x / 2.0f || loc.y < m_location.y - m_size.y / 2.0f || loc.y > m_location.y + m_size.y / 2.0f);
}

bool Menu_Background::on_mouse_click(Events::Mouse_Button_Pressed_Event& e)
{
	if (Input::Is_Key_Pressed(CS_KEY_ESCAPE))
	{
		m_exit_func();
		return true;
	}
	return false;
}

bool Menu_Background::on_keyboard_press(Events::Key_Pressed_Event& e)
{
	if (Input::Is_MouseButton_Pressed(CS_MOUSE_BUTTON_LEFT) && !is_mouse_over())
	{
		m_exit_func();
		return true;
	}
	return false;
}
