#include "GUI_Layer.h"

GUI_Layer::GUI_Layer(Type menu_type)
{
	m_type = menu_type;
}

GUI_Layer::~GUI_Layer()
{
	On_Detach();
}

void GUI_Layer::On_Attach(std::vector<std::pair<std::string, std::string>> textures)
{
	for (auto path : textures)
	{
		m_textures[path.first] = Texture::Load_Texture(path.second);
	}

	switch (m_type)
	{
	default:
		break;
	}
}

void GUI_Layer::On_Detach()
{
	for (scriptable_object* obj : m_objects)
	{
		delete obj;
	}
	m_objects.clear();
}

void GUI_Layer::On_Update(Timestep ts)
{
	for (scriptable_object* obj : m_objects)
	{
		obj->update();
	}
	Renderer::draw();
}

void GUI_Layer::On_ImGui_Render()
{
}

void GUI_Layer::On_Event(Events::Event& e)
{
	for (scriptable_object* obj : m_objects)
	{
		obj->event_callback(e);
	}
}

void GUI_Layer::add_scriptable_object(scriptable_object* obj)
{
	m_objects.push_back(obj);
}
