#include "Camera_Controller.h"


Camera_Controller::Camera_Controller(bool rotation)
	:m_camera(m_zoom_level, m_zoom_level, -m_zoom_level, m_zoom_level)
{
	m_aspect_ratio = 1.0f;
	m_rotation = rotation;
	m_camera_translation_speed = 0.005f;
}

void Camera_Controller::On_Update(Timestep ts)
{
	m_camera_translation_speed = 100.0f;
	m_camera_translation_speed = m_zoom_level * m_camera_translation_speed;
	if (Input::Is_Key_Pressed(CS_KEY_A))
	{
		m_camera_position.x -= cos(glm::radians(m_camera_rotation)) * m_camera_translation_speed * ts;
		m_camera_position.y -= sin(glm::radians(m_camera_rotation)) * m_camera_translation_speed * ts;
	}
	else if (Input::Is_Key_Pressed(CS_KEY_D))
	{
		m_camera_position.x += cos(glm::radians(m_camera_rotation)) * m_camera_translation_speed * ts;
		m_camera_position.y += sin(glm::radians(m_camera_rotation)) * m_camera_translation_speed * ts;
	}

	if (Input::Is_Key_Pressed(CS_KEY_W))
	{
		m_camera_position.x += -sin(glm::radians(m_camera_rotation)) * m_camera_translation_speed * ts;
		m_camera_position.y += cos(glm::radians(m_camera_rotation)) * m_camera_translation_speed * ts;
	}
	else if (Input::Is_Key_Pressed(CS_KEY_S))
	{
		m_camera_position.x -= -sin(glm::radians(m_camera_rotation)) * m_camera_translation_speed * ts;
		m_camera_position.y -= cos(glm::radians(m_camera_rotation)) * m_camera_translation_speed * ts;
	}

	if (m_rotation)
	{
		if (Input::Is_Key_Pressed(CS_KEY_Q))
			m_camera_rotation += m_rotation_speed;
		if (Input::Is_Key_Pressed(CS_KEY_E))
			m_camera_rotation -= m_rotation_speed;

		if (m_camera_rotation > 180.0f)
			m_camera_rotation -= 360.0f;
		else if (m_camera_rotation <= -180.0f)
			m_camera_rotation += 360.0f;

	}

	m_camera.Set_Position(m_camera_position);

	
}

void Camera_Controller::On_Event(Events::Event& e)
{
	Events::Event_Dispatcher dispatcher(e);
	dispatcher.Dispatch<Events::Mouse_Scrolled_Event>(std::bind(&Camera_Controller::on_mouse_scroll, this, std::placeholders::_1));
	dispatcher.Dispatch<Events::Window_Resize_Event>(std::bind(&Camera_Controller::on_window_resize, this, std::placeholders::_1));
}

bool Camera_Controller::on_mouse_scroll(Events::Mouse_Scrolled_Event& e)
{
	m_zoom_level -= e.GetYOffset() * 0.125f;
	m_zoom_level = std::max(m_zoom_level, 0.25f);
	m_zoom_level = (m_zoom_level <= 3) ? m_zoom_level : 3;
	m_camera.set_projection(-m_resolution.x * m_zoom_level, m_resolution.x * m_zoom_level, -m_resolution.y * m_zoom_level, m_resolution.y * m_zoom_level);
	return false;
}

bool Camera_Controller::on_window_resize(Events::Window_Resize_Event& e)
{
	//m_aspect_ratio = (float)e.GetWidth() / (float)e.GetHeight();
	m_resolution = { (float)e.GetWidth() , (float)e.GetHeight() };
	m_camera.set_projection(-m_resolution.x * m_zoom_level, m_resolution.x * m_zoom_level, -m_resolution.y * m_zoom_level, m_resolution.y * m_zoom_level);
	return false;
}
