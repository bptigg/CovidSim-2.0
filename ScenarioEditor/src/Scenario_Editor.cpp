#include "Scenario_Editor.h"

Scenario_Editor::Scenario_Editor()
	:Layer("Scenario Editor"), m_orthographic_controller(false)
{
}

void Scenario_Editor::On_Attach(std::vector<std::pair<std::string, std::string>> textures)
{
	m_orthographic_controller.get_camera().Set_Position({ 0.0f,0.0f,0.0f });
	m_orthographic_controller.Set_Resolution({ 1280.0f, 720.0f });
	Renderer::update_view(m_orthographic_controller.get_camera().Get_View_Projection_Matrix());

	for (int i = 0; i < textures.size(); i++)
	{
		m_textures[textures[i].first] = Texture::Load_Texture(textures[i].second);
	}
}

void Scenario_Editor::On_Detach()
{

}

void Scenario_Editor::On_Update(Timestep ts)
{
	m_orthographic_controller.On_Update(ts);

	Renderer::begin_batch();

	//ss << "Mouse click at: " << mouse_loc.x << " , " << mouse_loc.y << " : " << zoom << std::endl;

	if (present)
	{
		glm::vec4 color = { 0.8f, 0.4f,0.2f,1.0f };
		Renderer::draw_box({ -60.0f,60.0f }, { 120.0f, 120.0f }, 5.0f, color);
		//Renderer::draw_rectangle_color({ -115.0f, 60.0f }, { 10.0f, 100.0f }, color);
		//Renderer::draw_rectangle_color({ -60.0f, 115.0f }, { 120.0f, 10.0f }, color);
		//Renderer::draw_rectangle_color({ -5.0f, 60.0f }, { 10.0f, 100.0f }, color);
		//Renderer::draw_rectangle_color({ -60.0f, 5.0f }, { 120.0f, 10.0f }, color);
	}

	Renderer::draw();
}

void Scenario_Editor::On_ImGui_Render()
{
}

void Scenario_Editor::On_Event(Events::Event& e)
{
	m_orthographic_controller.On_Event(e);
	Renderer::update_view(m_orthographic_controller.get_camera().Get_View_Projection_Matrix());
	Events::Event_Dispatcher dispatcher(e);
	dispatcher.Dispatch<Events::Key_Pressed_Event>(BIND_EVENT_FN(Scenario_Editor::On_Key_Pressed));
	dispatcher.Dispatch<Events::Mouse_Moved_Event>(BIND_EVENT_FN(Scenario_Editor::On_Mouse_Press));
}

bool Scenario_Editor::On_Key_Pressed(Events::Key_Pressed_Event& e)
{
	if (e.Get_Key_Code() == CS_KEY_LEFT)
	{
		Log::info("LEFT KEY PRESSED");
	}
	if (e.Get_Key_Code() == CS_KEY_RIGHT)
	{
		Log::info("RIGHT KEY PRESSED");
	}
	if (e.Get_Key_Code() == CS_KEY_UP)
	{
		Log::info("UP KEY PRESSED");
	}
	if (e.Get_Key_Code() == CS_KEY_DOWN)
	{
		Log::info("DOWN KEY PRESSED");
	}
	return false;
}

bool Scenario_Editor::On_Mouse_Press(Events::Mouse_Moved_Event& e)
{
	std::pair<float, float> loc = std::make_pair(e.GetX(), e.GetY());

	float zoom = m_orthographic_controller.Get_Zoom_Level();
	if (zoom == 1)
	{
		glm::vec4 mouse_loc = glm::vec4((loc.first - 640.0f) * zoom + m_orthographic_controller.get_position().x, (360.0f - loc.second) * zoom + m_orthographic_controller.get_position().y, 1.0f, 1.0f);
		mouse_loc = mouse_loc * m_orthographic_controller.get_camera().Get_View_Projection_Matrix();
		mouse_loc = mouse_loc * zoom * zoom;

		std::stringstream ss;
		glm::vec2 position = { -60.0f, 60.f };
		present = colision(mouse_loc, position, 120.0f);
		if (colision(mouse_loc, position, 120.0f))
		{
			ss << "Mouse click at: " << mouse_loc.x << " , " << mouse_loc.y << " : " << zoom << std::endl;
			Log::info(ss.str().data());
		}
	}
	else
	{
		present = false;
	}
	return false;
}

//testing

bool Scenario_Editor::colision(glm::vec4& pos, glm::vec2& box_pos, float size)
{
	glm::vec4 vec1 = { box_pos.x - (size / 2.0f), box_pos.y - (size / 2.0f), 1.0f, 1.0f };
	glm::vec4 vec2 = { box_pos.x + (size / 2.0f), box_pos.y + (size / 2.0f), 1.0f, 1.0f };

	vec1 = vec1 * m_orthographic_controller.get_camera().Get_View_Projection_Matrix();
	vec2 = vec2 * m_orthographic_controller.get_camera().Get_View_Projection_Matrix();

	if (pos.x > vec1.x)
	{
		if (pos.x < vec2.x)
		{
			if (pos.y > vec1.y)
			{
				if (pos.y < vec2.y)
				{
					return true;
				}
			}
			return false;
		}
		return false;
	}
	return false;
}
