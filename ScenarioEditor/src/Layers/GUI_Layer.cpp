#include "GUI_Layer.h"

GUI_Layer::GUI_Layer(Type menu_type, unsigned int base_layer, std::shared_ptr<Camera_Controller> ortho_controll)
{
	m_type = menu_type;
	m_base_layer = base_layer;

	m_orthographic_controller = std::move(ortho_controll);
}

GUI_Layer::~GUI_Layer()
{
	On_Detach();
}

void GUI_Layer::On_Attach(std::vector<std::pair<std::string, std::string>> textures)
{
	for (std::pair<std::string, std::string> path : textures)
	{
		m_textures[path.first] = Texture::Load_Texture(path.second);
	}

	create_menu();

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
	//m_orthographic_controller->block = box->get_selected();
	m_orthographic_controller->On_Update(ts);

	for (scriptable_object* obj : m_objects)
	{
		obj->update_position(m_orthographic_controller->Get_Zoom_Level(), m_orthographic_controller->get_position(), m_orthographic_controller->get_camera().Get_View_Projection_Matrix());
		obj->update();
	}

	//Text title_text("Settings", { 0  , 250   }, 70.0f, { 0.0f,0.0f,0.0f,1.0f }, true);
	//title_text.render(0, 0, 3, true);

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

void GUI_Layer::create_menu()
{
	Menu_Background* settings = new Menu_Background({ 0,0 }, { 500, 600 }, this, { 0.09375f, 0.09375f, 0.09375f, 1.0f }, nullptr, m_base_layer);
	m_objects.push_back(settings);

	Text title_text("Settings", { 0 + settings->get_position().x , 250 + settings->get_position().y }, 70.0f, { (float)220 / (float)256, (float)220 / (float)256, (float)220 / (float)256, 1.0f }, true);
	Text_Menu_object* title = new Text_Menu_object(title_text, { 0 + settings->get_position().x, 250 + settings->get_position().y }, this, m_base_layer + 2);
	m_objects.push_back(title);

	Text num_name_text("Scenario name", { 0 + settings->get_position().x , 175 + settings->get_position().y }, 50.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* num_name = new Text_Menu_object(num_name_text, { 0 + settings->get_position().x, 110 + settings->get_position().y }, this, m_base_layer + 2);
	m_objects.push_back(num_name);

	Text_Box* num_name_box = new Text_Box({ 0 + settings->get_position().x, 140 + settings->get_position().y }, { 400, 30 }, this, false, m_base_layer, true);
	m_objects.push_back(num_name_box);

	Text num_tiles_text("Number of Tiles", { 0 + settings->get_position().x , 100 + settings->get_position().y }, 50.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* num_tiles = new Text_Menu_object(num_tiles_text, { 0 + settings->get_position().x, 100 + settings->get_position().y }, this, m_base_layer + 2);
	m_objects.push_back(num_tiles);

	Text_Box* num_tiles_box = new Text_Box({ 0 + settings->get_position().x, 65 + settings->get_position().y }, { 200, 30 }, this, true, m_base_layer, true);
	m_objects.push_back(num_tiles_box);

	Text num_agents_text("Population size", { 0 + settings->get_position().x , 25 + settings->get_position().y }, 50.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* num_agents = new Text_Menu_object(num_agents_text, { 0 + settings->get_position().x, 25 + settings->get_position().y }, this, m_base_layer + 2);
	m_objects.push_back(num_agents);

	Text_Box* num_agents_box = new Text_Box({ 0 + settings->get_position().x, -10 + settings->get_position().y }, { 200, 30 }, this, true, m_base_layer, true);
	m_objects.push_back(num_agents_box);
}
