#pragma once

#include "scriptable_object.h"
#include "Button.h"


class colour_palette : public scriptable_object
{
public:
	colour_palette(glm::vec2 pos, Layer* layer, unsigned int render_layer, int id);
	~colour_palette();

	virtual void render() override;
	virtual void update() override;
	virtual void event_callback(Events::Event& e) override;

	virtual void update_position(const float& zoom, const glm::vec2& camera_pos, const glm::mat4& camera_mat) override;

	glm::vec4 get_colour() { return { (red != 0) ? red / 256.0f : 0.0f, (green != 0) ? green / 256.0f : 0.0f, (blue != 0) ? blue / 256.0f : 0.0f, 1.0f }; }

private:
	unsigned int m_rendering_layer;
	
	float red, green, blue = 0.0f;

	Button m_main;
	Button m_secondary;

private:
	void get_blue();
	void get_rgb();
};

