#include "Education_Layer.h"

#include "editor.h"


static Education_Layer* e_instance = nullptr;

Education_Layer::Education_Layer(unsigned int base_layer, std::shared_ptr<Camera_Controller> ortho_controll)
{
	m_base_layer = base_layer;
	m_orthographic_controller = std::move(ortho_controll);

	m_render = false;
	m_dialog_box = false;
	m_attached = false;

	e_instance = this;
}

Education_Layer::~Education_Layer()
{
	On_Detach();
}

void Education_Layer::On_Attach(std::vector<std::pair<std::string, std::string>> textures)
{
	if (m_attached)
	{
		return;
	}

	for (std::pair<std::string, std::string> path : textures)
	{
		m_textures[path.first] = Texture::Load_Texture(path.second);
	}

	editor::get()->only_education(true);
	m_render = false;
	m_attached = true;
}

void Education_Layer::On_Detach()
{
	for (scriptable_object* obj : m_objects)
	{
		delete obj;
	}
	m_objects.clear();
}

void Education_Layer::On_Update(Timestep ts)
{
}

void Education_Layer::On_ImGui_Render()
{
}

void Education_Layer::On_Event(Events::Event& event)
{
}

void Education_Layer::enable_overlay()
{
	editor::get()->only_education(true);
	m_render = true;
}

void Education_Layer::disable_overlay()
{
	editor::get()->only_education(false);
	m_render = false;
}
