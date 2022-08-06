#include "editor.h"

editor::editor(unsigned int base_layer, std::shared_ptr<Camera_Controller> ortho_control)
{
	m_base_layer = base_layer;
	m_render = false;
	m_dialog_box = false;

	m_orthographic_controller = std::move(ortho_control);
}

editor::~editor()
{
	On_Detach();
}

void editor::On_Attach(std::vector<std::pair<std::string, std::string>> textures)
{
	m_render = true;
	
	if (m_attached)
	{
		return;
	}
	
	for (std::pair<std::string, std::string> path : textures)
	{
		m_textures[path.first] = Texture::Load_Texture(path.second);
	}

	file_data* temp_file = &file_data::get_file();
	m_size_of_grid = temp_file->world_buildings.num_of_tiles;
	draw_buttons(m_size_of_grid);

	m_orthographic_controller->get_camera().Set_Position({ 0.0f,0.0f,0.0f });
	m_orthographic_controller->set_position();
	Renderer::update_view(m_orthographic_controller->get_camera().Get_View_Projection_Matrix());

	m_attached = true;
	
}

void editor::On_Detach()
{
	for (scriptable_object* obj : m_objects)
	{
		delete obj;
	}
	m_objects.clear(); 
}

void editor::On_Update(Timestep ts)
{
	m_orthographic_controller->On_Update(ts);
	//m_orthographic_controller->set_position();
	//Renderer::update_view(m_orthographic_controller->get_camera().Get_Projection_Matrix());

	for (scriptable_object* obj : m_objects)
	{
		obj->update_position(m_orthographic_controller->Get_Zoom_Level(), m_orthographic_controller->get_position(), m_orthographic_controller->get_camera().Get_View_Projection_Matrix());
		obj->update();
	}

	//Renderer::draw();
}

void editor::On_ImGui_Render()
{
}

void editor::On_Event(Events::Event& e)
{
	m_orthographic_controller->On_Event(e);
	Renderer::update_view(m_orthographic_controller->get_camera().Get_View_Projection_Matrix());

	for (scriptable_object* obj : m_objects)
	{
		if (e.Handled != true)
		{
			obj->event_callback(e);
		}
	}
}

void editor::add_scriptable_object(scriptable_object* object)
{
	m_objects.push_back(object);
}

void editor::draw_buttons(int amount)
{
	float x = 0.0f;
	float y = 0.0f;
	float size = 60.0f;

	float width = amount * size;
	x = -width / 2.0f;
	y = x;

	m_button_num = 0;

	for (int i = 0; i < amount; i++)
	{
		for (int e = 0; e < amount; e++)
		{
			Button* tile = new Button({ x,y }, { size, size }, this, false, m_button_num);
			tile->base_colour = { 0.3f, 0.3f, 0.3f, 1.0f };
			tile->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
			tile->selected_colour = { 0.3f, 0.3f, 0.3f,1.0f };
			tile->rendering_layer = m_base_layer;
			tile->persist_hover(true);
			tile->Bind_function(BIND_BUTTON_FN(editor::open_zone_selector));
			tile->Bind_right_function(BIND_BUTTON_FN(editor::open_drop_down));

			std::shared_ptr<button_data> tile_data(new button_data);
			m_world_data[m_button_num] = tile_data;
			m_world_data[m_button_num]->button_id = m_button_num;
			m_world_data[m_button_num]->building_type = -1;
			add_scriptable_object(tile);

			x = x + size;
			m_button_num++;
		}

		y = y + size;
		x = -width / 2.0f;
	}

}

void editor::open_zone_selector()
{
	Events::GUI_Building_Select_Event event(m_objects[m_selected]);
	Event_Call_back(event);
}

void editor::open_drop_down()
{
	Events::GUI_Dropdown_Event event(m_objects[m_selected]);
	Event_Call_back(event);
}


