#pragma once

#include <glm/glm.hpp>

#include "Orthographic_camera.h"

#include "../Events/Application_Event.h"
#include "../Events/Mouse_Event.h"

#include "../Input.h"

class Camera_Controller
{
public:
	Camera_Controller(float aspect_ratio, bool rotation = false);

	void On_Update();
	void On_Event(Events::Event& e);

	Orthographic_camera& get_camera() { return m_camera; }
	const Orthographic_camera& get_camera() const { return m_camera; }

	float Get_Zoom_Level() const { return m_zoom_level; }
	void Set_Zoom_Level(float level) { m_zoom_level = level; }
private:
	bool on_mouse_scroll(Events::Mouse_Scrolled_Event& e);
	bool on_window_resize(Events::Window_Resize_Event& e);
private:
	float m_aspect_ratio;
	float m_zoom_level = 1.0f;

	bool m_rotation;

	Orthographic_camera m_camera;

	glm::vec3 m_camera_position = { 0.0f, 0.0f, 0.0f };
	float m_camera_rotation = 0.0f;
	float m_camera_translation_speed = 5.0f;
	float m_rotation_speed = 180.0f;
};

