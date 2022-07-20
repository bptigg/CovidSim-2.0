#pragma once
#include "scriptable_object.h"
class Menu_Background : public scriptable_object
{
private:
	glm::vec4 m_background_color;

	std::function<void()> m_exit_func;
public:
	Menu_Background(glm::vec2 pos, glm::vec2 size, Layer* layer, glm::vec4 colour, const std::function<void()> exit_Func);
	virtual ~Menu_Background() override;

	virtual void render() override;
	virtual void update() override;
	virtual void event_callback(Events::Event& e) override;

	bool is_mouse_over(); 

	bool on_mouse_click(Events::Mouse_Button_Pressed_Event& e);
	bool on_keyboard_press(Events::Key_Pressed_Event& e);
};

