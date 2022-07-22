#pragma once

#include "scriptable_object.h"
#include "Text.h"

#include "../Events/Mouse_Event.h"
#include "../Events/Key_Event.h"

class Text_Box : public scriptable_object
{
protected:
	Text m_text;
	std::string text;
	bool m_over;
	bool m_selected;

	bool caps;

	float m_zoom;
	glm::vec2 m_camera_position;
	glm::mat4 m_camera_matrix;

	float x_offset;

public:
	Text_Box(const glm::vec2& position, const glm::vec2& size, Layer* layer);
	~Text_Box();

	virtual void update() override;
	virtual void render() override;

	virtual void event_callback(Events::Event& e) override;

	void update_position(const float& zoom, const glm::vec2& camera_pos, const glm::mat4& camera_mat);

	const bool get_selected() const { return m_selected; }

private:

	void key_to_char(uint32_t key);

	bool on_mouse_move(Events::Mouse_Moved_Event& e);
	bool on_mouse_click(Events::Mouse_Button_Pressed_Event& e);
	bool on_keyboard_press(Events::Key_Pressed_Event& e);
};

