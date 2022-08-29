#pragma once

#include "scriptable_object.h"
#include <algorithm>

//basically a container for a load of objects

class Scrollable_Menu : public scriptable_object
{
private:
	std::vector<scriptable_object*> m_menu_objects;

	float m_step = 10.0f;
	float m_position = 0;
	glm::vec2 bounds = {};

	bool full;
public:
	Scrollable_Menu(glm::vec2 pos, glm::vec2 size, Layer* layer, unsigned int render_layer, int id);
	~Scrollable_Menu();

	virtual void render() override;
	virtual void update() override;
	virtual void event_callback(Events::Event& e) override;

	virtual void update_position(const float& zoom, const glm::vec2& camera_pos, const glm::mat4& camera_mat) override;

	void add_object(scriptable_object* obj);
	void remove_object(scriptable_object* obj);

	inline float get_top() { return m_menu_objects[0]->get_position().y; }
	inline float get_bottom() { return m_menu_objects[m_menu_objects.size() - 1]->get_position().y; }

	inline bool check_valid(float pos) {
		full = true;
		return pos >= m_location.y - m_size.y / 2.0f && pos <= m_location.y + m_size.y / 2.0f;
	}

private:
	bool on_mouse_scroll(Events::Mouse_Scrolled_Event& e);
};

