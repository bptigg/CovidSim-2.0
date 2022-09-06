#pragma once

#include "scriptable_object.h"
#include "../Events/Mouse_Event.h"
#include "../Entity/Text.h"

class Button : public scriptable_object
{
public:
	enum class State
	{
		None = 0,
		Hover,
		Press
	};

	glm::vec4 base_colour;
	glm::vec4 box_colour;
	glm::vec4 selected_colour;
	glm::vec2 relative_position;

	bool textured;

	GLuint base_texture;
	GLuint selected_texture;

	unsigned int rendering_layer;

	//editor specfic
	bool action_needed;

protected:
	std::function<void()> button_func;
	std::function<void()> right_button_func;
	bool m_default_func;

	State m_state;
	Text m_text;

	bool m_persist;
	bool m_hover;

	bool m_render;

	float m_zoom;
	glm::vec2 m_camera_position;
	glm::mat4 m_camera_matrix;


public:
	Button(const glm::vec2& location, const glm::vec2& size, Layer* layer, bool menu, int id = 0);
	Button(const std::string text, const glm::vec2& location, const glm::vec2& size, Layer* layer, bool menu, float scale, int id = 0);
	virtual ~Button() override;

	inline void Bind_function(const std::function<void()> func)
	{
		button_func = func;
	}

	inline void Bind_right_function(const std::function<void()> func)
	{
		right_button_func = func;
	}

	inline void render_obj(bool obj) override { m_render = obj; } 
	inline bool get_render() override { return m_render; }

	virtual void update() override;
	virtual void render() override;
	virtual void event_callback(Events::Event& e) override;

	virtual void update_position(const float& zoom, const glm::vec2& camera_pos, const glm::mat4& camera_mat) override;
	virtual void change_position(const glm::vec2& pos) override;

	inline void change_text(std::string new_text) { m_text.update_string(new_text); }

	void change_state(bool state) { m_hover = state; m_state = (!m_hover) ? State::None : State::Press; }
	void persist_hover(bool persist) { m_persist = persist; }
	const std::string get_text() const { return m_text.m_text; }

	void p_is_mouse_over() { is_mouse_over(true); }

private:
	void m_default_function();

	bool on_mouse_move(Events::Mouse_Moved_Event& e);
	bool on_mouse_click(Events::Mouse_Button_Pressed_Event& e);

	bool is_mouse_over(bool position = false);
};

