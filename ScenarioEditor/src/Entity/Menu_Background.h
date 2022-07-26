#pragma once
#include "scriptable_object.h"
class Menu_Background : public scriptable_object
{
private:
	glm::vec4 m_background_color;
	unsigned int m_rendering_layer;

	std::function<void()> m_exit_func;
public:
	Menu_Background(glm::vec2 pos, glm::vec2 size, Layer* layer, glm::vec4 colour, const std::function<void()> exit_Func, unsigned int rendering_layer, int id = 0);
	virtual ~Menu_Background() override;

	virtual void render() override;
	virtual void update() override;
	virtual void event_callback(Events::Event& e) override;

	inline void Bind_function(const std::function<void()> func)
	{
		m_exit_func = func;
	}

	bool is_mouse_over(float width, float height, float x, float y);

	bool on_mouse_click(Events::Mouse_Button_Pressed_Event& e);
	bool on_keyboard_press(Events::Key_Pressed_Event& e);

	virtual void update_position(const float& zoom, const glm::vec2& camera_pos, const glm::mat4& camera_mat) override;
};

