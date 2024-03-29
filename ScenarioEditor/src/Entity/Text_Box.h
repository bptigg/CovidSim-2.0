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

	bool m_interger_only;
	bool m_is_integer_only;

	unsigned int m_rendering_layer;

	bool caps;

	float m_zoom;
	glm::vec2 m_camera_position;
	glm::mat4 m_camera_matrix;

	float x_offset;


public:
	Text_Box(const glm::vec2& position, const glm::vec2& size, Layer* layer, bool int_only, unsigned int rendering_layer, bool centred_text, int id = 0);
	~Text_Box();

	virtual void update() override;
	virtual void render() override;

	virtual void event_callback(Events::Event& e) override;

	virtual void update_position(const float& zoom, const glm::vec2& camera_pos, const glm::mat4& camera_mat) override;

	inline const bool get_selected() const { return m_selected; }

	inline unsigned int get_string_length() { return text.length(); }
	
	inline const std::string get_string() const { return text; }
	inline std::string get_string() { return text; }
	inline void set_string(std::string string) { text = string; m_text.update_string(text); }

	inline void clear_box() { text = ""; m_text.update_string(text); }

private:

	void key_to_char(uint32_t key);

	bool on_mouse_move(Events::Mouse_Moved_Event& e);
	bool on_mouse_click(Events::Mouse_Button_Pressed_Event& e);
	bool on_keyboard_press(Events::Key_Pressed_Event& e);
};

