#pragma once

#include "scriptable_object.h"
#include "Text.h"

class Text_Menu_object : public scriptable_object
{
private:
	Text m_text;
	unsigned int m_rendering_layer;
public:
	Text_Menu_object(Text text, glm::vec2 pos, Layer* layer, unsigned int render_layer, int id = 0);

	virtual void render() override;
	virtual void update() override;
	virtual void event_callback(Events::Event& e) override;

	virtual void update_position(const float& zoom, const glm::vec2& camera_pos, const glm::mat4& camera_mat) override;

	Text* get_text() { return &m_text; }
};

