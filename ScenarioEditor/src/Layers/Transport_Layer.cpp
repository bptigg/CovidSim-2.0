#include "Transport_Layer.h"

#include "editor.h"

Transport_Layer::Transport_Layer(unsigned int base_layer, std::shared_ptr<Camera_Controller> ortho_controll)
{
	m_base_layer = base_layer;
	m_orthographic_controller = std::move(ortho_controll);

	m_render = false;
	m_dialog_box = false;
}

Transport_Layer::~Transport_Layer()
{
	On_Detach();
}

void Transport_Layer::On_Attach(std::vector<std::pair<std::string, std::string>> textures)
{
	m_overlay = editor::get()->get_overlay();

	if (m_attached)
	{
		return;
	}

	for (std::pair<std::string, std::string> path : textures)
	{
		m_textures[path.first] = Texture::Load_Texture(path.second);
	}

	editor::get()->only_transport(true);
	m_render = false;
	m_attached = true;
}

void Transport_Layer::On_Detach()
{
	for (scriptable_object* obj : m_objects)
	{
		delete obj;
	}
	m_objects.clear();
	m_overlay.clear();
}

void Transport_Layer::On_Update(Timestep ts)
{
	if (m_render)
	{
		for (glm::vec4 rec : m_overlay)
		{
			float x_pos = (rec.z + rec.x) / 2.0f;
			float y_pos = (rec.w + rec.y) / 2.0f;
			Renderer::draw_rectangle_color({ x_pos, y_pos }, { rec.z - rec.x, rec.w - rec.y }, { 0.8f, 0.8f, 0.8f, 1.0f }, m_base_layer, false);
		}
	}

	//Renderer::draw_rectangle_color({ 600, 360 }, { 60,60 }, { 0.8f, 0.8f, 0.8f, 1.0f }, m_base_layer, false);
}

void Transport_Layer::On_ImGui_Render()
{
}

void Transport_Layer::On_Event(Events::Event& e)
{
	if (m_render)
	{
		if (e.Get_Event_Type() == Events::Event_Type::Key_Pressed)
		{
			if (Input::Is_Key_Pressed(CS_KEY_L))
			{
				Events::Event_Dispatcher dispatch(e);
				dispatch.Dispatch<Events::Key_Pressed_Event>(BIND_EVENT_FN(Transport_Layer::open_line_manager));
			}
		}
		if (e.Get_Event_Type() == Events::Event_Type::Transport_Line_Editor)
		{
			open_line_editor();
			e.Handled = true; 
		}

		//m_scroll.event_callback(e);
	}
}

void Transport_Layer::disable_overlay()
{
	editor::get()->only_transport(false);
	m_render = false;
}

void Transport_Layer::enable_overlay()
{
	m_overlay = editor::get()->get_overlay();
	editor::get()->only_transport(true);
	m_render = true;
}

std::shared_ptr<Line> Transport_Layer::update_line(std::string key)
{
	if (m_lines.find(key) == m_lines.end())
	{
		m_lines[key] = std::shared_ptr<Line>(new Line);
	}

	return m_lines[key];
}

bool Transport_Layer::open_line_manager(Events::Key_Pressed_Event& e)
{
	Events::GUI_Line_Manager_Event event(this);
	Event_Call_back(event);
	return true;
}

void Transport_Layer::open_line_editor()
{
	Events::GUI_Line_Editor_Event event(true);
	event.add_layer(this);
	Event_Call_back(event);
}
