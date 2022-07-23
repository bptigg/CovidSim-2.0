#pragma once

#include "scriptable_object.h"
#include "../Events/Mouse_Event.h"

class Button : public scriptable_object
{
public:
	enum class State
	{
		None = 0,
		Hover,
		Press
	};

protected:
	std::function<void()> button_func;
	bool m_default_func;

	State m_state;

	float m_zoom;
	glm::vec2 m_camera_position;
	glm::mat4 m_camera_matrix;

public:
	Button(const glm::vec2& location, const glm::vec2& size, Layer* layer, bool menu);
	virtual ~Button() override;

	inline void Bind_function(const std::function<void()> func)
	{
		button_func = func;
	}

	virtual void update() override;
	virtual void render() override;
	virtual void event_callback(Events::Event& e) override;

	virtual void update_position(const float& zoom, const glm::vec2& camera_pos, const glm::mat4& camera_mat) override;

private:
	void m_default_function();

	bool on_mouse_move(Events::Mouse_Moved_Event& e);
	bool on_mouse_click(Events::Mouse_Button_Pressed_Event& e);
};

