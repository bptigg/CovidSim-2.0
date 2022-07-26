#include "GUI_Layer.h"

#define BIND_FUNCTION(x) std::bind(&GUI_Layer::x, this)

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
	for (std::pair<std::string, std::string> path : textures)
	{
		m_textures[path.first] = Texture::Load_Texture(path.second);
	}

	create_settings_menu(0);

}

void GUI_Layer::On_Detach()
{
	for (scriptable_object* obj : m_objects)
	{
		delete obj;
	}

	for (scriptable_object* obj : m_inactive_objects)
	{
		delete obj;
	}
	m_objects.clear();
	m_inactive_objects.clear();
};

void GUI_Layer::On_Update(Timestep ts)
{
	m_orthographic_controller->On_Update(ts);

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
		obj->update();

	}

	if (m_render)
	{
		Renderer::draw();
	}
}

void GUI_Layer::On_ImGui_Render()
{
}

void GUI_Layer::On_Event(Events::Event& e)
{
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
		}
		
		for (scriptable_object* obj : m_objects)
		{
			obj->delete_obj(true);
		}
		page_two();
	}
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
	

	Text num_tiles_text("Number of Tiles", { 0 + settings->get_position().x , 90 + settings->get_position().y }, 50.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* num_tiles = new Text_Menu_object(num_tiles_text, { 0 + settings->get_position().x, 100 + settings->get_position().y }, this, m_base_layer + 2);
	m_objects.push_back(num_tiles);
	;

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
	m_objects.push_back(confirm);

}
