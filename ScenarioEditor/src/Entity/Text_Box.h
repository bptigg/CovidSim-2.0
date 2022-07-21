#pragma once

#include "scriptable_object.h"
#include "Text.h"

#include "../Events/Mouse_Event.h"
#include "../Events/Key_Event.h"

class Text_Box : public scriptable_object
{
protected:
	Text m_text;
	bool m_over;
	bool m_selected;
public:
	Text_Box(const glm::vec2& position, const glm::vec2& size, Layer* layer, Text text);
	~Text_Box();

	virtual void update() override;
	virtual void render() override;

	virtual void event_callback(Events::Event& e) override;

private:
	
	bool on_mouse_move(Events::Mouse_Moved_Event& e);
	bool on_mouse_click(Events::Mouse_Button_Pressed_Event& e);
	bool on_keyboard_press(Events::Key_Pressed_Event& e);
};

