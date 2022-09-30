#include "editor.h"

#include "../Events/Transport_Events.h"

static editor* e_instance;

editor::editor(unsigned int base_layer, std::shared_ptr<Camera_Controller> ortho_control)
{
	m_base_layer = base_layer;
	m_render = false;
	m_dialog_box = false;

	m_orthographic_controller = std::move(ortho_control);
	m_disable_non_transport_events = false;
	m_cached_transport_overlay = false;

	e_instance = this;
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
		
		if (m_world_data[obj->get_id()]->render)
		{
			if (!m_disable_non_transport_events)
			{
				obj->update();
			}
			else
			{
				if (m_world_data[obj->get_id()]->transport_building)
				{
					obj->update();
				}

			}

			auto button = dynamic_cast<Button*>(obj);
			if (button != nullptr)
			{
				button->action_needed = m_world_data[button->get_id()]->action_needed;
			}
		}
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

	if (e.Get_Event_Type() == Events::Event_Type::Key_Pressed)
	{
		if (Input::Is_Key_Pressed(CS_KEY_TAB))
		{
			Events::Event_Dispatcher dispatch(e);
			dispatch.Dispatch<Events::Key_Pressed_Event>(BIND_EVENT_FN(editor::open_settings_panel));
		}
		else if (dynamic_cast<Events::Key_Pressed_Event*>(&e)->Get_Key_Code() == CS_KEY_ESCAPE)
		{
			Events::Event_Dispatcher dispatch(e);
			dispatch.Dispatch<Events::Key_Pressed_Event>(BIND_EVENT_FN(editor::export_scenario));

		}
		
	}

	for (scriptable_object* obj : m_objects)
	{
		if (e.Handled != true)
		{
			if (!m_disable_non_transport_events)
			{
				obj->event_callback(e);
			}
			else
			{
				if (m_world_data[obj->get_id()]->transport_building)
				{
					obj->event_callback(e);
				}

			}
		}
		if (e.Handled)
		{
			break;
		}
	}
}

editor* editor::get()
{
	return e_instance;
}

glm::vec4 editor::get_grid()
{
	std::pair<float, float> min = { 0.0f, 0.0f };
	std::pair<float, float> max = { 0.0f, 0.0f };

	min.first  = m_objects[0]->get_position().x - (m_objects[0]->get_size().x / 2.0f);
	min.second = m_objects[0]->get_position().y - (m_objects[0]->get_size().y / 2.0f);

	max.first = m_objects[m_button_num - 1]->get_position().x + (m_objects[m_button_num - 1]->get_size().x / 2.0f);
	max.second = m_objects[m_button_num - 1]->get_position().y + (m_objects[m_button_num - 1]->get_size().y / 2.0f);

	return { min.first, min.second, max.first, max.second };
}

std::vector<glm::vec4> editor::get_overlay()
{
	if (!m_cached_transport_overlay)
	{
		glm::vec4 grid = get_grid();

		std::vector<glm::vec4> overlay = {};
		float min_x = grid.x;
		float min_y = grid.y;

		float max_x = grid.z;
		float max_y = grid.w;

		float x = min_x;
		float y = min_y;
		for (int i = 0; i < m_objects.size();)
		{
			glm::vec4 current_rectangle(0.0f);
			if (m_world_data[i]->types.size() > 0 && m_world_data[i]->transport_building)
			{
				x = m_objects[i]->get_position().x - (m_objects[i]->get_size().x / 2.0f);
				y = m_objects[i]->get_position().y - (m_objects[i]->get_size().y / 2.0f);

				if (y != min_y - 60.0f)
				{
					current_rectangle.z = max_x;
					current_rectangle.x = min_x;

					current_rectangle.w = y;
					current_rectangle.y = min_y;
					float old_min_y = min_y;
					min_y = y + 60.0f;

					if (y != old_min_y)
					{
						overlay.push_back(current_rectangle);
					}
					overlay.push_back({ min_x, y, x, (m_objects[i]->get_position().y + (m_objects[i]->get_size().y / 2.0f)) });

					float diff = y - grid.y;
					int row = diff / 60.0f;

					int offset = row * m_size_of_grid;
					int index = i - offset;

					float x_pos = m_objects[i]->get_position().x + (m_objects[i]->get_size().x / 2.0f);

					for (int e = index + 1; e < m_size_of_grid; e++)
					{
						if (m_world_data[e + offset]->transport_building)
						{
							if (m_objects[e + offset]->get_position().x - (m_objects[e + offset]->get_size().x / 2.0f) != x_pos)
							{
								overlay.push_back({ x_pos, y, m_objects[e + offset]->get_position().x - (m_objects[e + offset]->get_size().x / 2.0f) , y + 60.0f });
							}
							x_pos = m_objects[e + offset]->get_position().x + (m_objects[e + offset]->get_size().x / 2.0f);
						}
					}

					if (x_pos != max_x)
					{
						overlay.push_back({ x_pos, y, max_x, y + 60 });
					}

					i = ((row + 1) * m_size_of_grid) - 1;
				}
			}
			i++;
		}

		if (y + 60.0f != max_y)
		{
			overlay.push_back({ min_x, (y == grid.y) ? y : y + 60.0f, max_x, max_y });
		}
		m_transport_overlay = overlay;
		m_cached_transport_overlay = true;
	}
	
	return m_transport_overlay;
}

void editor::only_transport(bool arg)
{
	m_disable_non_transport_events = arg;

	if (arg == false)
	{
		for (auto obj : m_objects)
		{
			if (m_world_data[obj->get_id()]->transport_building != true) {
				m_world_data[obj->get_id()]->render = true;
			}
		}
	}
	else
	{
		for (auto obj : m_objects)
		{
			if (m_world_data[obj->get_id()]->transport_building != true) {
				m_world_data[obj->get_id()]->render = false;
			}
		}
	}
}

void editor::bind_transport_select(bool arg)
{
	if (arg)
	{
		for (auto obj : m_objects)
		{
			if (m_world_data[obj->get_id()]->render == true && m_world_data[obj->get_id()]->transport_building == true)
			{
				auto button = dynamic_cast<Button*>(obj);
				button->Bind_function(BIND_BUTTON_FN(editor::stop_selected));
			};
		}
	}
	else
	{
		for (auto obj : m_objects)
		{
			if (m_world_data[obj->get_id()]->render == true && m_world_data[obj->get_id()]->transport_building == true)
			{
				auto button = dynamic_cast<Button*>(obj);
				button->Bind_function(BIND_BUTTON_FN(editor::open_zone_selector));
			};
		}
	}
}

void editor::bind_transport_remove(bool arg)
{
	if (arg)
	{
		for (auto obj : m_objects)
		{
			if (m_world_data[obj->get_id()]->render == true && m_world_data[obj->get_id()]->transport_building == true)
			{
				auto button = dynamic_cast<Button*>(obj);
				button->Bind_function(BIND_BUTTON_FN(editor::stop_removed));
			};
		}
	}
	else
	{
		for (auto obj : m_objects)
		{
			if (m_world_data[obj->get_id()]->render == true && m_world_data[obj->get_id()]->transport_building == true)
			{
				auto button = dynamic_cast<Button*>(obj);
				button->Bind_function(BIND_BUTTON_FN(editor::open_zone_selector));
			};
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
			m_world_data[m_button_num]->transport_building = false;
			m_world_data[m_button_num]->type = Transport_Type::NONE;
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

void editor::stop_selected()
{
	Events::Transport_stop_select event(m_objects[m_selected]);
	Event_Call_back(event);
	bind_transport_select(false);
}

void editor::stop_removed()
{
	Events::Transport_stop_select event(m_objects[m_selected], true);
	Event_Call_back(event);
	bind_transport_select(false);
}

bool editor::open_settings_panel(Events::Key_Pressed_Event& e)
{
	Events::GUI_Settings_Event event(m_objects[m_selected]);
	Event_Call_back(event);
	return true;
}

bool editor::export_scenario(Events::Key_Pressed_Event& e)
{
	Events::AppExportEvent event;
	Event_Call_back(event);
	return true;
}


