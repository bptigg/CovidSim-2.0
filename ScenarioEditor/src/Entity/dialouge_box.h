#pragma once

#include "scriptable_object.h"
#include "Button.h"
#include "Text.h"

class dialouge_box : public scriptable_object
{
private:
	Text m_error_text;
	Text m_error;
	Button m_close;

	unsigned int m_rendering_layer;
	float m_scale;
public:
	dialouge_box(std::string text, const glm::vec2& pos, const glm::vec2& size, Layer* layer, unsigned int rendering_layer);
	~dialouge_box();

	virtual void update() override;
	virtual void render() override;

	virtual void event_callback(Events::Event& e) override;

	virtual void update_position(const float& zoom, const glm::vec2& camera_pos, const glm::mat4& camera_mat) override;

private:
	 
	bool on_mouse_move(Events::Mouse_Moved_Event& e);
	bool on_mouse_click(Events::Mouse_Button_Pressed_Event& e);
	void close();
};

