#include "GUI_Layer.h"

#define BIND_FUNCTION(x) std::bind(&GUI_Layer::x, this)

#if _DEBUG
#define BYPASS 1
#else
#define BYPASS 0
#endif

GUI_Layer::GUI_Layer(Type menu_type, unsigned int base_layer, std::shared_ptr<Camera_Controller> ortho_controll)
{
	m_type = menu_type;
	m_base_layer = base_layer;
	m_render = true;
	m_dialog_box = false;
	
	m_orthographic_controller = std::move(ortho_controll);
}

GUI_Layer::~GUI_Layer()
{
	On_Detach();
}

void GUI_Layer::On_Attach(std::vector<std::pair<std::string, std::string>> textures)
{

	if (m_attached)
	{
		return;
	}

	for (std::pair<std::string, std::string> path : textures)
	{
		m_textures[path.first] = Texture::Load_Texture(path.second);
	}

	switch (m_type)
	{
	case Type::SetupMenu:
		create_settings_menu(0);
		break;
	case Type::BuildingSelectMenu:
		create_building_menu();
		break;
	default:
		break;
	}

	m_attached = true;

}

void GUI_Layer::On_Detach()
{
	for (scriptable_object* obj : m_objects)
	{
		delete obj;
	}
	m_objects.clear();
};

void GUI_Layer::On_Update(Timestep ts)
{
	//m_orthographic_controller->On_Update(ts);
	//Renderer::update_view(m_orthographic_controller->get_camera().Get_Projection_Matrix());

	int removed = 0;
	for (int i = 0; i < m_objects.size() + removed; i++)
	{
		scriptable_object* obj = m_objects[i - removed];
		if (obj->delete_object)
		{
			obj->~scriptable_object();
			m_objects.erase(m_objects.begin() + i - removed);
			m_dialog_box = false;
			removed++;
			continue;
		}

		obj->update_position(m_orthographic_controller->Get_Zoom_Level(), m_orthographic_controller->get_position(), m_orthographic_controller->get_camera().Get_View_Projection_Matrix());
			
		if (m_render)
		{
			obj->update();
		}

	}

}

void GUI_Layer::On_ImGui_Render()
{
}

void GUI_Layer::On_Event(Events::Event& e)
{
	if (m_render)
	{
		m_orthographic_controller->block = true;
		if (m_dialog_box)
		{
			for (scriptable_object* obj : m_objects)
			{
				if (obj->get_type() == entity_type::DIALOUGE_BOX)
				{
					if (e.Handled != true)
					{
						obj->event_callback(e);
					}
				}
			}
		}
		else
		{
			for (scriptable_object* obj : m_objects)
			{
				if (e.Handled != true)
				{
					obj->event_callback(e);
				}
			}
		}
		m_orthographic_controller->block = false;
	}
}

void GUI_Layer::set_caller(scriptable_object* caller)
{
	m_caller = caller;
}

void GUI_Layer::change_box_colour()
{
	for (scriptable_object* obj : m_objects)
	{
		Button* temp_button = dynamic_cast<Button*>(obj);
		if (temp_button != nullptr && temp_button->get_id() == m_selected)
		{
			Button* caller_button = dynamic_cast<Button*>(m_caller);
			if (caller_button != nullptr)
			{
				caller_button->base_colour = temp_button->base_colour;
				caller_button->selected_colour = temp_button->selected_colour;
				caller_button->change_state(false);
				m_caller = nullptr;
				break;
			}
		}
	}
}

void GUI_Layer::change_box_texture()
{
}

void GUI_Layer::add_scriptable_object(scriptable_object* obj)
{
	m_objects.push_back(obj);
}

void GUI_Layer::create_settings_menu(unsigned int menu)
{
	switch (menu)
	{
	case 0:
		page_one();
		break;
	case 1:
		page_two();
		break;
	default:
		Log::warning("NO KNOWN MENU PAGE");
		break;
	};

}

void GUI_Layer::create_building_menu()
{
	int button_id = 0;
	m_render = false;
	Menu_Background* settings = new Menu_Background({ 430,0 }, { 420, 640 }, this, { 0.09375f, 0.09375f, 0.09375f, 1.0f }, nullptr, m_base_layer);
	settings->Bind_function(BIND_FUNCTION(GUI_Layer::close_menu));
	add_scriptable_object(settings);

	Text title_text("Zone selection", { 0 + settings->get_position().x , 280 + settings->get_position().y }, 60.0f, { (float)220 / (float)256, (float)220 / (float)256, (float)220 / (float)256, 1.0f }, true);
	Text_Menu_object* title = new Text_Menu_object(title_text, { 0 + settings->get_position().x, 280 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(title);

	Button* reset = new Button({ -105.0f + settings->get_position().x, 230.0f + settings->get_position().y }, { 50.0f, 50.f }, this, true, button_id);
	reset->base_colour = { 0.3f, 0.3f, 0.3f, 1.0f };
	reset->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	reset->selected_colour = { 0.3f, 0.3f, 0.3f,1.0f };
	reset->rendering_layer = m_base_layer;
	reset->Bind_function(BIND_BUTTON_FN(GUI_Layer::change_box_colour));
	add_scriptable_object(reset);
	button_id++;

	Text reset_text("reset zone", { 50 + settings->get_position().x , 230 + settings->get_position().y }, 40.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* reset_holder = new Text_Menu_object(reset_text, { 50 + settings->get_position().x, 250 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(reset_holder);

	Button* walking_zone = new Button({ -105.0f + settings->get_position().x, 170.0f + settings->get_position().y }, { 50.0f, 50.f }, this, true, button_id);
	walking_zone->base_colour = { 0.5f, 0.5f, 0.5f, 1.0f };
	walking_zone->selected_colour = walking_zone->base_colour;
	walking_zone->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	walking_zone->rendering_layer = m_base_layer;
	walking_zone->Bind_function(BIND_BUTTON_FN(GUI_Layer::change_box_colour));
	add_scriptable_object(walking_zone);
	button_id++;

	Text walking_zone_text("undeveloped space", { 50 + settings->get_position().x , 170 + settings->get_position().y }, 40.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* walking_zone_holder = new Text_Menu_object(walking_zone_text, { 50 + settings->get_position().x, 170 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(walking_zone_holder);

	Button* housing_zone = new Button({ -105.0f + settings->get_position().x, 110.0f + settings->get_position().y }, { 50.0f, 50.f }, this, true, button_id);
	housing_zone->base_colour = { 86.0f / 255.0f, 125.0f / 255.0f, 70.0f / 255.0f, 1.0f };
	housing_zone->selected_colour = housing_zone->base_colour;
	housing_zone->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	housing_zone->rendering_layer = m_base_layer;
	housing_zone->Bind_function(BIND_BUTTON_FN(GUI_Layer::change_box_colour));
	add_scriptable_object(housing_zone);
	button_id++;

	Text house_zone_text("housing development", { 50 + settings->get_position().x , 110 + settings->get_position().y }, 40.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* housing_zone_holder = new Text_Menu_object(house_zone_text, { 50 + settings->get_position().x, 170 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(housing_zone_holder);

	Button* generic_zone = new Button({ -105.0f + settings->get_position().x, 50.0f + settings->get_position().y }, { 50.0f, 50.f }, this, true, button_id);
	generic_zone->base_colour = { 51.0f / 255.0f, 51.0f / 255.0f, 1.0f, 1.0f };
	generic_zone->selected_colour = generic_zone->base_colour;
	generic_zone->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	generic_zone->rendering_layer = m_base_layer;
	generic_zone->Bind_function(BIND_BUTTON_FN(GUI_Layer::change_box_colour));
	add_scriptable_object(generic_zone);
	button_id++;

	Text generic_zone_text("generic development", { 50 + settings->get_position().x , 50 + settings->get_position().y }, 40.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* generic_zone_holder = new Text_Menu_object(generic_zone_text, { 50 + settings->get_position().x, 170 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(generic_zone_holder);

}

void GUI_Layer::setting_exit_func()
{
	bool not_complete = false;
	for (scriptable_object* obj : m_objects)
	{
		if (obj->get_type() == entity_type::TEXT_BOX)
		{
			Text_Box* box = dynamic_cast<Text_Box*>(obj);
			if (box != nullptr)
			{
				if (box->get_string_length() == 0)
				{
					not_complete = true;
					break;
				}
			}
		}
	}

	if (not_complete)
	{
		dialouge_box* entry_fields = new dialouge_box("Incomplete entry fields remaining", { 0.0f, 0.0f }, { 600.0f, 200.0f }, this, m_base_layer + 4);
		m_objects.push_back(entry_fields);
		m_dialog_box = true;
	}
	else if(!sm_saved)
	{
		dialouge_box* saved = new dialouge_box("Settings not saved", { 0.0f, 0.0f }, {600.0f, 200.0f}, this, m_base_layer + 4);
		m_objects.push_back(saved);
		m_dialog_box = true;
	}

}

void GUI_Layer::save_exit_func()
{
	bool not_complete = false;
	for (scriptable_object* obj : m_objects)
	{
		if (obj->get_type() == entity_type::TEXT_BOX)
		{
			Text_Box* box = dynamic_cast<Text_Box*>(obj);
			if (box != nullptr)
			{
				if (box->get_string_length() == 0)
				{
					not_complete = true;
					break;
				}
			}
		}
	}

	if (not_complete)
	{
		dialouge_box* entry_fields = new dialouge_box("Incomplete entry fields remaining", { 0.0f, 0.0f }, { 600.0f, 200.0f }, this, m_base_layer + 4);
		m_objects.push_back(entry_fields);
		m_dialog_box = true;
		return;
	}
	else
	{
		
		{
			file_data* temp_file = &file_data::get_file();

			std::ifstream temp;

			std::vector<std::string> text_input;
			
			for (scriptable_object* obj : m_objects)
			{
				if (obj->get_type() == entity_type::TEXT_BOX)
				{
					Text_Box* box = dynamic_cast<Text_Box*>(obj);
					if (box != nullptr)
					{
						text_input.push_back(box->get_string());
					}
				}
			}

			temp.open("scenarios/" + text_input[0] + ".cvsn");

			if(temp.fail())
			{
				temp_file->file_exists = true;
			}
			else
			{
				dialouge_box* file_exists = new dialouge_box("file: " + text_input[0] + " already exists. Provide new filename", {0.0f, 0.0f}, {600.0f, 200.0f}, this, m_base_layer + 4);
				m_objects.push_back(file_exists);
				m_dialog_box = true;
				return;
			}

			std::string::const_iterator it;

			for (int i = 1; i < 4; i++)
			{
				it = text_input[i].begin();

				while (it != text_input[i].end() && std::isdigit(*it)) ++it;

				if (it != text_input[i].end())
				{
					dialouge_box* not_int = new dialouge_box(text_input[i] + " is not a valid number", { 0.0f, 0.0f }, { 600.0f, 200.0f }, this, m_base_layer + 4);
					m_objects.push_back(not_int);
					m_dialog_box = true;
					return;
				}
			}

			temp_file->scenario.name					= text_input[0];
			temp_file->world_buildings.num_of_tiles		= stoi(text_input[1]);
			temp_file->scenario.population				= stoi(text_input[2]);
			temp_file->scenario.max_counts				= stoi(text_input[3]);
		}
		
		for (scriptable_object* obj : m_objects)
		{
			obj->delete_obj(true);
		}
		page_two();
	}
}

void GUI_Layer::save_exit_func_2()
{
	bool not_complete = false;
	for (scriptable_object* obj : m_objects)
	{
		if (obj->get_type() == entity_type::TEXT_BOX)
		{
			Text_Box* box = dynamic_cast<Text_Box*>(obj);
			if (box != nullptr)
			{
				if (box->get_string_length() == 0)
				{
					not_complete = true;
					break;
				}
			}
		}
	}

	if (not_complete)
	{
		dialouge_box* entry_fields = new dialouge_box("Incomplete entry fields remaining", { 0.0f, 0.0f }, { 600.0f, 200.0f }, this, m_base_layer + 4);
		m_objects.push_back(entry_fields);
		m_dialog_box = true;
		return;
	}
	else
	{
		file_data* temp_file = &file_data::get_file();

		std::ifstream temp;

		std::vector<std::string> text_input;

		for (scriptable_object* obj : m_objects)
		{
			if (obj->get_type() == entity_type::TEXT_BOX)
			{
				Text_Box* box = dynamic_cast<Text_Box*>(obj);
				if (box != nullptr)
				{
					text_input.push_back(box->get_string());
				}
			}
		}

		if (!BYPASS)
		{
			for (int i = 0; i < text_input.size(); i++)
			{
				temp.open("data/" + text_input[0] + ".json");
				if (temp.fail())
				{
					dialouge_box* entry_fields = new dialouge_box("file: " + text_input[0] + " not present", { 0.0f, 0.0f }, { 600.0f, 200.0f }, this, m_base_layer + 4);
					m_objects.push_back(entry_fields);
					m_dialog_box = true;
					return;
				}
			}
		}

		temp_file->scenario.population_pyramid		= text_input[0];
		temp_file->scenario.population_race_dist	= text_input[1];
		temp_file->scenario.adult_medical_data		= text_input[2];
		temp_file->scenario.child_medical_data		= text_input[3];
	}

	m_render = false;
	m_delete_layer = true;

	Events::GUI_Editor_Event event;
	Event_Call_back(event);
}

void GUI_Layer::page_one()
{
	Menu_Background* settings = new Menu_Background({ 0,0 }, { 500, 600 }, this, { 0.09375f, 0.09375f, 0.09375f, 1.0f }, nullptr, GUI_Layer::m_base_layer);
	settings->Bind_function(BIND_FUNCTION(GUI_Layer::setting_exit_func));
	m_objects.push_back(settings);
	

	Text title_text("Settings", { 0 + settings->get_position().x , 250 + settings->get_position().y }, 70.0f, { (float)220 / (float)256, (float)220 / (float)256, (float)220 / (float)256, 1.0f }, true);
	Text_Menu_object* title = new Text_Menu_object(title_text, { 0 + settings->get_position().x, 250 + settings->get_position().y }, this, m_base_layer + 2);
	m_objects.push_back(title);
	

	Text num_name_text("Scenario name", { 0 + settings->get_position().x , 175 + settings->get_position().y }, 50.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* num_name = new Text_Menu_object(num_name_text, { 0 + settings->get_position().x, 110 + settings->get_position().y }, this, m_base_layer + 2);
	m_objects.push_back(num_name);
	

	Text_Box* num_name_box = new Text_Box({ 0 + settings->get_position().x, 140 + settings->get_position().y }, { 400, 30 }, this, false, m_base_layer, true);
	m_objects.push_back(num_name_box);
	

	Text num_tiles_text("Size of world (it's a square)", { 0 + settings->get_position().x , 90 + settings->get_position().y }, 50.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* num_tiles = new Text_Menu_object(num_tiles_text, { 0 + settings->get_position().x, 100 + settings->get_position().y }, this, m_base_layer + 2);
	m_objects.push_back(num_tiles);

	Text_Box* num_tiles_box = new Text_Box({ 0 + settings->get_position().x, 50 + settings->get_position().y }, { 200, 30 }, this, true, m_base_layer, true);
	m_objects.push_back(num_tiles_box);

	Text num_agents_text("Population size", { 0 + settings->get_position().x , 0 + settings->get_position().y }, 50.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* num_agents = new Text_Menu_object(num_agents_text, { 0 + settings->get_position().x, 25 + settings->get_position().y }, this, m_base_layer + 2);
	m_objects.push_back(num_agents);

	Text_Box* num_agents_box = new Text_Box({ 0 + settings->get_position().x, -40 + settings->get_position().y }, { 200, 30 }, this, true, m_base_layer, true);
	m_objects.push_back(num_agents_box);

	Text num_length_text("Length of model (number of counts)", { 0 + settings->get_position().x , -90 + settings->get_position().y }, 50.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* num_length = new Text_Menu_object(num_length_text, { 0 + settings->get_position().x, 25 + settings->get_position().y }, this, m_base_layer + 2);
	m_objects.push_back(num_length);

	Text_Box* num_length_box = new Text_Box({ 0 + settings->get_position().x, -130 + settings->get_position().y }, { 200, 30 }, this, true, m_base_layer, true);
	m_objects.push_back(num_length_box);

	Button* confirm = new Button("Save", { 0 + settings->get_position().x, -260 + settings->get_position().y }, { 150, 60 }, this, true, 45.0f);
	confirm->base_colour = { 0.09375f, 0.09375f, 0.09375f, 1.0f };
	confirm->box_colour = { 0.8f, 0.8f, 0.8f, 1.0f };
	confirm->selected_colour = { 0.2f, 0.2f, 0.2f, 1.0f };
	confirm->rendering_layer = m_base_layer;
	confirm->Bind_function(BIND_BUTTON_FN(GUI_Layer::save_exit_func));
	m_objects.push_back(confirm);
}

void GUI_Layer::page_two()
{
	Menu_Background* settings = new Menu_Background({ 0,0 }, { 500, 600 }, this, { 0.09375f, 0.09375f, 0.09375f, 1.0f }, nullptr, GUI_Layer::m_base_layer);
	settings->Bind_function(BIND_FUNCTION(GUI_Layer::setting_exit_func));
	m_objects.push_back(settings);

	Text title_text("Settings", { 0 + settings->get_position().x , 250 + settings->get_position().y }, 70.0f, { (float)220 / (float)256, (float)220 / (float)256, (float)220 / (float)256, 1.0f }, true);
	Text_Menu_object* title = new Text_Menu_object(title_text, { 0 + settings->get_position().x, 250 + settings->get_position().y }, this, m_base_layer + 2);
	m_objects.push_back(title);

	Text pop_pyramind_text("Population pyramid filename", { 0 + settings->get_position().x , 175 + settings->get_position().y }, 50.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* pop_py = new Text_Menu_object(pop_pyramind_text, { 0 + settings->get_position().x, 110 + settings->get_position().y }, this, m_base_layer + 2);
	m_objects.push_back(pop_py);

	Text_Box* pop_pyramind_box = new Text_Box({ 0 + settings->get_position().x, 140 + settings->get_position().y }, { 400, 30 }, this, false, m_base_layer, true);
	m_objects.push_back(pop_pyramind_box);

	Text pop_race_dist_text("Population race distribution filename", { 0 + settings->get_position().x , 90 + settings->get_position().y }, 50.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* pop_race_dist = new Text_Menu_object(pop_race_dist_text, { 0 + settings->get_position().x, 100 + settings->get_position().y }, this, m_base_layer + 2);
	m_objects.push_back(pop_race_dist);

	Text_Box* pop_race_dist_box = new Text_Box({ 0 + settings->get_position().x, 50 + settings->get_position().y }, { 200, 30 }, this, false, m_base_layer, true);
	m_objects.push_back(pop_race_dist_box);

	Text adult_medical_text("adult medical data filename", { 0 + settings->get_position().x , 0 + settings->get_position().y }, 50.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* adult_medical = new Text_Menu_object(adult_medical_text, { 0 + settings->get_position().x, 100 + settings->get_position().y }, this, m_base_layer + 2);
	m_objects.push_back(adult_medical);

	Text_Box* adult_medical_box = new Text_Box({ 0 + settings->get_position().x, -40 + settings->get_position().y }, { 200, 30 }, this, false, m_base_layer, true);
	m_objects.push_back(adult_medical_box);

	Text child_medical_text("child medical data filename", { 0 + settings->get_position().x , -90 + settings->get_position().y }, 50.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* child_medical = new Text_Menu_object(child_medical_text, { 0 + settings->get_position().x, 100 + settings->get_position().y }, this, m_base_layer + 2);
	m_objects.push_back(child_medical);

	Text_Box* child_medical_box = new Text_Box({ 0 + settings->get_position().x, -130 + settings->get_position().y }, { 200, 30 }, this, false, m_base_layer, true);
	m_objects.push_back(child_medical_box);

	Button* confirm = new Button("Save", { 0 + settings->get_position().x, -260 + settings->get_position().y }, { 150, 60 }, this, true, 45.0f);
	confirm->base_colour = { 0.09375f, 0.09375f, 0.09375f, 1.0f };
	confirm->box_colour = { 0.8f, 0.8f, 0.8f, 1.0f };
	confirm->selected_colour = { 0.2f, 0.2f, 0.2f, 1.0f };
	confirm->rendering_layer = m_base_layer;
	confirm->Bind_function(BIND_BUTTON_FN(GUI_Layer::save_exit_func_2));
	m_objects.push_back(confirm);

}

void GUI_Layer::close_menu()
{
	m_render = false;
	Button* caller_button = dynamic_cast<Button*>(m_caller);
	if (caller_button != nullptr)
	{
		caller_button->change_state(false);
	}
}