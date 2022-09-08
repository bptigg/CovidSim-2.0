#include "GUI_Layer.h"

#include "editor.h"
#include "../Entity/building_constants.h"

#define BIND_FUNCTION(x) std::bind(&GUI_Layer::x, this)

#define EVENT_KEY 0

static bool s_rmenu_active = false;
static bool s_lmenu_active = false;

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
	m_camera_block = false;
	m_side = Side::NONE;
	m_event_block = false;
}

GUI_Layer::~GUI_Layer()
{
	On_Detach();
}

void GUI_Layer::On_Attach(std::vector<std::pair<std::string, std::string>> textures)
{

	if (m_attached)
	{
		if (m_type != Type::BuildingSizeSubMenu && m_type != Type::ButtonDropDown && m_type != Type::ColourSelect)
		{
			return;
		}
	}

	for (std::pair<std::string, std::string> path : textures)
	{
		m_textures[path.first] = Texture::Load_Texture(path.second);
	}

	switch (m_type)
	{
	case Type::SetupMenu:
		menu_key = 1;
		create_settings_menu(0);
		break;
	case Type::BuildingSelectMenu:
		if (!editor::get()->get_only_transport())
		{
			menu_key = 2;
			create_building_menu();
		}
		break;
	case Type::PublicBuildingSubMenu:
		menu_key = 3;
		create_public_building_sub_menu();
		break;
	case Type::PublicTransportSubMenu:
		menu_key = 4;
		create_public_transport_sub_menu();
		break;
	case Type::BuildingSizeSubMenu:
		menu_key = 5;
		create_building_size_sub_menu();
		break;
	case Type::ButtonDropDown:
		if (!editor::get()->get_only_transport())
		{
			menu_key = 6;
			create_button_dropdown();
		}
		break;
	case Type::CapacityPopup:
		menu_key = 7;
		create_capacity_popup();
		break;
	case Type::StaffPopup:
		menu_key = 8;
		create_staff_popup();
		break;
	case Type::OpeningPopup:
		menu_key = 9;
		create_opening_popup();
		break;
	case Type::SettingsMenu:
		menu_key = 10;
		create_settings_menu();
		break;
	case Type::LineManager:
		menu_key = 11;
		create_line_manager();
		break;
	case Type::LineEditor:
		menu_key = 12;
		create_line_editor();
		break;
	case Type::ColourSelect:
		menu_key = 13;
		create_colour_dropdown();
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
			if (m_objects.size() != 1)
			{
				m_objects.erase(m_objects.begin() + i - removed);
			}
			else
			{
				m_objects.clear();
			}
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
	if (m_event_block)
	{
		return;
	}

	if (e.Get_Event_Type() == Events::Event_Type::Transport_Stop_Select)
	{
		if (m_type == Type::LineEditor)
		{
			add_stop(false);
		}
	}

	if (m_render)
	{
		//bool dont_unblock = m_orthographic_controller->get_block();
		//m_orthographic_controller->block_camera(true, EVENT_KEY);

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
			std::vector<scriptable_object*> reverse = reverse_vector(m_objects);
			for (auto obj : reverse)
			{
				if (e.Handled != true)
				{
					obj->event_callback(e);
				}
			}
		}
		//if (!dont_unblock)
		//{
			//m_orthographic_controller->block_camera(false, EVENT_KEY);
		//}
	}
}

void GUI_Layer::render(bool render)
{
	if (m_render)
	{
		return;
	}

	if (!editor::get()->get_only_transport())
	{
		m_render = true;
		side_check();
	}
	else
	{
		switch (m_type)
		{
		case GUI_Layer::Type::Overlay:
			m_render = true;
			side_check();
			break;
		case GUI_Layer::Type::BuildingSelectMenu:
			m_render = false;
			break;
		case GUI_Layer::Type::PublicBuildingSubMenu:
			m_render = false;
			break;
		case GUI_Layer::Type::PublicTransportSubMenu:
			m_render = false;
			break;
		case GUI_Layer::Type::BuildingSizeSubMenu:
			m_render = false;
			break;
		case GUI_Layer::Type::ButtonDropDown:
			m_render = false;
			break;
		case GUI_Layer::Type::CapacityPopup:
			m_render = false;
			break;
		case GUI_Layer::Type::StaffPopup:
			m_render = false;
			break;
		case GUI_Layer::Type::OpeningPopup:
			m_render = false;
			break;
		case GUI_Layer::Type::SettingsMenu:
			m_render = true;
			side_check();
			break;
		case GUI_Layer::Type::SetupMenu:
			m_render = false;
			break;
		case GUI_Layer::Type::LineManager:
			m_render = true;
			side_check();
			break;
		case GUI_Layer::Type::LineEditor:
			m_render = true;
			side_check();
			break;
		case GUI_Layer::Type::ColourSelect:
			m_render = true;
			side_check();
		default:
			break;
		}
	}
	if (!render)
	{
		m_render = false;
	}
}

void GUI_Layer::set_caller(scriptable_object* caller)
{
	m_caller = caller;
}

void GUI_Layer::set_prev_menu(scriptable_object* caller)
{
	m_menu_button = caller;
}

void GUI_Layer::set_menu(scriptable_object* p_menu)
{
	m_prev_menu = p_menu;
}

void GUI_Layer::set_call_layer(Layer* layer)
{
	m_call_layer = layer;
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

				{
					editor* temp_layer = dynamic_cast<editor*>(caller_button->get_layer());
					auto data = temp_layer->get_world_data(caller_button->get_id());
					data->building_type = get_building_code(caller_button->base_colour);

					if (data->building_type < 4)
					{
						data->action_needed = false;
						data->transport_building = false;
						data->capacity_action_needed = false;
						data->staff_action_needed = false;
						data->opening_action_needed = false;
					}
				}
				m_caller = nullptr;
				break;
			}
		}
	}
}

void GUI_Layer::change_line_colour()
{
	for (auto obj : m_objects)
	{
		if (obj->get_type() == entity_type::COLOUR_PALETTE)
		{
			auto button = dynamic_cast<Button*>(m_caller);
			button->base_colour = dynamic_cast<colour_palette*>(obj)->get_colour();
			button->selected_colour = button->base_colour;
		}
	}
}

//void GUI_Layer::change_box_colour_sub_menu()
//{
//	for (scriptable_object* obj : m_objects)
//	{
//		Button* temp_button = dynamic_cast<Button*>(obj);
//		if (temp_button != nullptr && temp_button->get_id() == m_selected)
//		{
//			Button* caller_button = dynamic_cast<Button*>(m_caller);
//			if (caller_button != nullptr)
//			{
//				caller_button->base_colour = temp_button->base_colour;
//				caller_button->selected_colour = temp_button->selected_colour;
//				caller_button->change_state(false);
//				
//				{
//					editor* temp_layer = dynamic_cast<editor*>(caller_button->get_layer());
//					auto data = temp_layer->get_world_data(caller_button->get_id());
//					//data->building_type = 
//				}
//				m_caller = nullptr;
//				m_render = false;
//				break;
//			}
//		}
//	}
//}

void GUI_Layer::set_transport_type()
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

				{
					editor* temp_layer = dynamic_cast<editor*>(caller_button->get_layer());
					temp_layer->delete_transport_cache();
					auto data = temp_layer->get_world_data(caller_button->get_id());
					data->building_type = get_building_code(caller_button->base_colour);
					data->transport_building = true;

					data->capacity_action_needed = true;
					data->staff_action_needed = true;
					data->opening_action_needed = true;
					data->action_needed = (data->capacity_action_needed || data->staff_action_needed || data->opening_action_needed);
				}
				m_caller = nullptr;
				break;
			}
		}
	}
}

void GUI_Layer::set_zone_size()
{
	for (scriptable_object* obj : m_objects)
	{
		Button* temp_button = dynamic_cast<Button*>(obj);
		if (temp_button != nullptr && temp_button->get_id() == m_selected)
		{
			Button* caller_button = dynamic_cast<Button*>(m_caller);
			Button* menu_button = dynamic_cast<Button*>(m_menu_button);
			if (caller_button != nullptr && menu_button != nullptr)
			{
				caller_button->base_colour = menu_button->base_colour;
				caller_button->selected_colour = menu_button->selected_colour;
				caller_button->change_state(false);

				{
					editor* temp_layer = dynamic_cast<editor*>(caller_button->get_layer());
					auto data = temp_layer->get_world_data(caller_button->get_id());
					data->size = temp_button->get_id();
					
					std::string button_text = temp_button->get_text();
					if (button_text == "Large")
					{
						data->size = 2;
					}
					else if (button_text == "Medium")
					{
						data->size = 1;
					}
					else
					{
						data->size = 0;
					}

					data->building_type = get_building_code(caller_button->base_colour);

					data->capacity_action_needed = true;
					data->staff_action_needed = true;
					data->opening_action_needed = true;
					data->action_needed = (data->capacity_action_needed || data->staff_action_needed || data->opening_action_needed);
				}

				m_caller = nullptr;
				m_render = false;
				close_size_menu();
				
				GUI_Layer* menu_layer = dynamic_cast<GUI_Layer*>(menu_button->get_layer());
				menu_layer->close_public_menu();

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
	m_side = Side::RIGHT;
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
	Text_Menu_object* reset_holder = new Text_Menu_object(reset_text, { 50 + settings->get_position().x, 230 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(reset_holder);

	Button* walking_zone = new Button({ -105.0f + settings->get_position().x, 170.0f + settings->get_position().y }, { 50.0f, 50.f }, this, true, button_id);
	walking_zone->base_colour = building_constants::WALKING_ZONE;
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
	housing_zone->base_colour = building_constants::HOUSING_ZONE;
	housing_zone->selected_colour = housing_zone->base_colour;
	housing_zone->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	housing_zone->rendering_layer = m_base_layer;
	housing_zone->Bind_function(BIND_BUTTON_FN(GUI_Layer::change_box_colour));
	add_scriptable_object(housing_zone);
	button_id++;

	Text house_zone_text("housing development", { 50 + settings->get_position().x , 110 + settings->get_position().y }, 40.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* housing_zone_holder = new Text_Menu_object(house_zone_text, { 50 + settings->get_position().x, 110 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(housing_zone_holder);

	Button* generic_zone = new Button({ -105.0f + settings->get_position().x, 50.0f + settings->get_position().y }, { 50.0f, 50.f }, this, true, button_id);
	generic_zone->base_colour = building_constants::GENERIC_WORK_ZONE;
	generic_zone->selected_colour = generic_zone->base_colour;
	generic_zone->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	generic_zone->rendering_layer = m_base_layer;
	generic_zone->Bind_function(BIND_BUTTON_FN(GUI_Layer::change_box_colour));
	add_scriptable_object(generic_zone);
	button_id++;

	Text generic_zone_text("generic development", { 50 + settings->get_position().x , 50 + settings->get_position().y }, 40.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* generic_zone_holder = new Text_Menu_object(generic_zone_text, { 50 + settings->get_position().x, 50 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(generic_zone_holder);

	Button* generic_shop_zone = new Button({ -105.0f + settings->get_position().x, -10.0f + settings->get_position().y }, { 50.0f, 50.f }, this, true, button_id);
	generic_shop_zone->base_colour = building_constants::GENERIC_SHOP_ZONE;
	generic_shop_zone->selected_colour = generic_shop_zone->base_colour;
	generic_shop_zone->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	generic_shop_zone->rendering_layer = m_base_layer;
	generic_shop_zone->Bind_function(BIND_BUTTON_FN(GUI_Layer::change_box_colour));
	add_scriptable_object(generic_shop_zone);
	button_id++;

	Text generic_shop_zone_text("generic shop development", { 50 + settings->get_position().x , -10 + settings->get_position().y }, 40.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* generic_shop_zone_holder = new Text_Menu_object(generic_shop_zone_text, { 50 + settings->get_position().x, -10 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(generic_shop_zone_holder);

	Button* public_building_zone = new Button({ -105.0f + settings->get_position().x, -70.0f + settings->get_position().y }, { 50.0f, 50.f }, this, true, button_id);
	public_building_zone->base_colour = { 255.0f / 215.0f, 51.0f / 255.0f, 0.0f, 1.0f };
	public_building_zone->selected_colour = public_building_zone->base_colour;
	public_building_zone->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	public_building_zone->rendering_layer = m_base_layer;
	public_building_zone->Bind_function(BIND_BUTTON_FN(GUI_Layer::open_public_sub));
	add_scriptable_object(public_building_zone);
	button_id++;

	Text public_building_zone_text("public development", { 50 + settings->get_position().x , -70 + settings->get_position().y }, 40.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* public_building_zone_holder = new Text_Menu_object(public_building_zone_text, { 50 + settings->get_position().x, -70 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(public_building_zone_holder);

	Button* transport_zone = new Button({ -105.0f + settings->get_position().x, -130.0f + settings->get_position().y }, { 50.0f, 50.f }, this, true, button_id);
	transport_zone->base_colour = { 255.0f / 255.0f, 144.0f / 255.0f, 0.0f, 1.0f };
	transport_zone->selected_colour = transport_zone->base_colour;
	transport_zone->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	transport_zone->rendering_layer = m_base_layer;
	transport_zone->Bind_function(BIND_BUTTON_FN(GUI_Layer::open_transport_sub));
	add_scriptable_object(transport_zone);
	button_id++;

	Text transport_zone_text("transport development", { 50 + settings->get_position().x , -130 + settings->get_position().y }, 40.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* transport_zone_holder = new Text_Menu_object(transport_zone_text, { 50 + settings->get_position().x, -130 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(transport_zone_holder);

	//generic shop

}

void GUI_Layer::create_public_building_sub_menu()
{
	int button_id = 0;
	m_render = false;
	m_side = Side::LEFT;
	Menu_Background* settings = new Menu_Background({ -430,0 }, { 420, 640 }, this, { 0.09375f, 0.09375f, 0.09375f, 1.0f }, nullptr, m_base_layer);
	settings->Bind_function(BIND_FUNCTION(GUI_Layer::close_pb_menu));
	add_scriptable_object(settings);

	Text title_text("Public Building selection", { 0 + settings->get_position().x , 280 + settings->get_position().y }, 60.0f, { (float)220 / (float)256, (float)220 / (float)256, (float)220 / (float)256, 1.0f }, true);
	Text_Menu_object* title = new Text_Menu_object(title_text, { 0 + settings->get_position().x, 280 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(title);

	Button* place_of_worship = new Button({ -105.0f + settings->get_position().x, 230.0f + settings->get_position().y }, { 40.0f, 40.f }, this, true, button_id);
	place_of_worship->base_colour = building_constants::PLACE_OF_WORSHIP;
	place_of_worship->selected_colour = place_of_worship->base_colour;
	place_of_worship->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	place_of_worship->rendering_layer = m_base_layer;
	place_of_worship->Bind_function(BIND_BUTTON_FN(GUI_Layer::open_size_sub));
	add_scriptable_object(place_of_worship);
	button_id++;

	Text pow_zone_text("place of worship", { 50 + settings->get_position().x , 230 + settings->get_position().y }, 40.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* pow_zone_holder = new Text_Menu_object(pow_zone_text, { 50 + settings->get_position().x, -70 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(pow_zone_holder);

	Button* park = new Button({ -105.0f + settings->get_position().x, 180.0f + settings->get_position().y }, { 40.0f, 40.f }, this, true, button_id);
	park->base_colour = building_constants::PARK;
	park->selected_colour = park->base_colour;
	park->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	park->rendering_layer = m_base_layer;
	park->Bind_function(BIND_BUTTON_FN(GUI_Layer::open_size_sub));
	add_scriptable_object(park);
	button_id++;

	Text park_zone_text("park", { 50 + settings->get_position().x , 180 + settings->get_position().y }, 40.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* park_zone_holder = new Text_Menu_object(park_zone_text, { 50 + settings->get_position().x, -70 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(park_zone_holder);

	Button* cafe = new Button({ -105.0f + settings->get_position().x, 130.0f + settings->get_position().y }, { 40.0f, 40.f }, this, true, button_id);
	cafe->base_colour = building_constants::CAFE;
	cafe->selected_colour = park->base_colour;
	cafe->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	cafe->rendering_layer = m_base_layer;
	cafe->Bind_function(BIND_BUTTON_FN(GUI_Layer::open_size_sub));
	add_scriptable_object(cafe);
	button_id++;

	Text cafe_zone_text("cafe", { 50 + settings->get_position().x , 130 + settings->get_position().y }, 40.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* cafe_zone_holder = new Text_Menu_object(cafe_zone_text, { 50 + settings->get_position().x, -70 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(cafe_zone_holder);

	Button* restaurant = new Button({ -105.0f + settings->get_position().x, 80.0f + settings->get_position().y }, { 40.0f, 40.f }, this, true, button_id);
	restaurant->base_colour = building_constants::RESTAURANT;
	restaurant->selected_colour = restaurant->base_colour;
	restaurant->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	restaurant->rendering_layer = m_base_layer;
	restaurant->Bind_function(BIND_BUTTON_FN(GUI_Layer::open_size_sub));
	add_scriptable_object(restaurant);
	button_id++;

	Text rest_zone_text("restaurant", { 50 + settings->get_position().x , 80.0f + settings->get_position().y }, 40.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* rest_zone_holder = new Text_Menu_object(rest_zone_text, { 50 + settings->get_position().x, -70 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(rest_zone_holder);

	Button* cinema = new Button({ -105.0f + settings->get_position().x, 30.0f + settings->get_position().y }, { 40.0f, 40.f }, this, true, button_id);
	cinema->base_colour = building_constants::CINEMA;
	cinema->selected_colour = cinema->base_colour;
	cinema->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	cinema->rendering_layer = m_base_layer;
	cinema->Bind_function(BIND_BUTTON_FN(GUI_Layer::open_size_sub));
	add_scriptable_object(cinema);
	button_id++;

	Text cinema_zone_text("cinema", { 50 + settings->get_position().x , 30 + settings->get_position().y }, 40.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* cinema_zone_holder = new Text_Menu_object(cinema_zone_text, { 50 + settings->get_position().x, -70 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(cinema_zone_holder);

	Button* theatre = new Button({ -105.0f + settings->get_position().x, -20.0f + settings->get_position().y }, { 40.0f, 40.f }, this, true, button_id);
	theatre->base_colour = building_constants::THEATRE;
	theatre->selected_colour = theatre->base_colour;
	theatre->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	theatre->rendering_layer = m_base_layer;
	theatre->Bind_function(BIND_BUTTON_FN(GUI_Layer::open_size_sub));
	add_scriptable_object(theatre);
	button_id++;

	Text theatre_zone_text("theatre", { 50 + settings->get_position().x , -20 + settings->get_position().y }, 40.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* theatre_zone_holder = new Text_Menu_object(theatre_zone_text, { 50 + settings->get_position().x, -70 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(theatre_zone_holder);

	Button* supermarket = new Button({ -105.0f + settings->get_position().x, -70.0f + settings->get_position().y }, { 40.0f, 40.f }, this, true, button_id);
	supermarket->base_colour = building_constants::SUPERMARKET;
	supermarket->selected_colour = supermarket->base_colour;
	supermarket->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	supermarket->rendering_layer = m_base_layer;
	supermarket->Bind_function(BIND_BUTTON_FN(GUI_Layer::open_size_sub));
	add_scriptable_object(supermarket);
	button_id++;

	Text super_zone_text("supermarket", { 50 + settings->get_position().x , -70 + settings->get_position().y }, 40.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* super_zone_holder = new Text_Menu_object(super_zone_text, { 50 + settings->get_position().x, -70 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(super_zone_holder);

	Button* shopping_centre = new Button({ -105.0f + settings->get_position().x, -120.0f + settings->get_position().y }, { 40.0f, 40.f }, this, true, button_id);
	shopping_centre->base_colour = building_constants::SHOPPING_CENTRE;
	shopping_centre->selected_colour = shopping_centre->base_colour;
	shopping_centre->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	shopping_centre->rendering_layer = m_base_layer;
	shopping_centre->Bind_function(BIND_BUTTON_FN(GUI_Layer::open_size_sub));
	add_scriptable_object(shopping_centre);
	button_id++;

	Text shop_zone_text("shopping centre", { 50 + settings->get_position().x , -120.0f + settings->get_position().y }, 40.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* shop_zone_holder = new Text_Menu_object(shop_zone_text, { 50 + settings->get_position().x, -70 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(shop_zone_holder);

	Button* pub= new Button({ -105.0f + settings->get_position().x, -170.0f + settings->get_position().y }, { 40.0f, 40.f }, this, true, button_id);
	pub->base_colour = building_constants::PUB;
	pub->selected_colour = pub->base_colour;
	pub->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	pub->rendering_layer = m_base_layer;
	pub->Bind_function(BIND_BUTTON_FN(GUI_Layer::open_size_sub));
	add_scriptable_object(pub);
	button_id++;

	Text pub_zone_text("pub", { 50 + settings->get_position().x , -170 + settings->get_position().y }, 40.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* pub_zone_holder = new Text_Menu_object(pub_zone_text, { 50 + settings->get_position().x, -70 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(pub_zone_holder);

	Button* nightclub = new Button({ -105.0f + settings->get_position().x, -220.0f + settings->get_position().y }, { 40.0f, 40.f }, this, true, button_id);
	nightclub->base_colour = building_constants::NIGHTCLUB;
	nightclub->selected_colour = nightclub->base_colour;
	nightclub->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	nightclub->rendering_layer = m_base_layer;
	nightclub->Bind_function(BIND_BUTTON_FN(GUI_Layer::open_size_sub));
	add_scriptable_object(nightclub);
	button_id++;

	Text club_zone_text("nightclub", { 50 + settings->get_position().x , -220 + settings->get_position().y }, 40.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* club_zone_holder = new Text_Menu_object(club_zone_text, { 50 + settings->get_position().x, -70 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(club_zone_holder);

	Button* arena = new Button({ -105.0f + settings->get_position().x, -270.0f + settings->get_position().y }, { 40.0f, 40.f }, this, true, button_id);
	arena->base_colour = building_constants::ARENA;
	arena->selected_colour = arena->base_colour;
	arena->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	arena->rendering_layer = m_base_layer;
	arena->Bind_function(BIND_BUTTON_FN(GUI_Layer::open_size_sub));
	add_scriptable_object(arena);
	button_id++;

	Text arena_zone_text("arena", { 50 + settings->get_position().x , -270 + settings->get_position().y }, 40.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* arena_zone_holder = new Text_Menu_object(arena_zone_text, { 50 + settings->get_position().x, -70 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(arena_zone_holder);
}

void GUI_Layer::create_public_transport_sub_menu()
{
	int button_id = 0;
	m_render = false;
	m_side = Side::LEFT;
	Menu_Background* settings = new Menu_Background({ -430,0 }, { 420, 640 }, this, { 0.09375f, 0.09375f, 0.09375f, 1.0f }, nullptr, m_base_layer);
	settings->Bind_function(BIND_FUNCTION(GUI_Layer::close_tb_menu));
	add_scriptable_object(settings);

	Text title_text("Public transport selection", { 0 + settings->get_position().x , 280 + settings->get_position().y }, 60.0f, { (float)220 / (float)256, (float)220 / (float)256, (float)220 / (float)256, 1.0f }, true);
	Text_Menu_object* title = new Text_Menu_object(title_text, { 0 + settings->get_position().x, 280 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(title);

	Button*	Bus = new Button({ -105.0f + settings->get_position().x, 230.0f + settings->get_position().y }, { 40.0f, 40.f }, this, true, button_id);
	Bus->base_colour = building_constants::BUS;
	Bus->selected_colour = Bus->base_colour;
	Bus->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	Bus->rendering_layer = m_base_layer;
	Bus->Bind_function(BIND_BUTTON_FN(GUI_Layer::set_transport_type));
	add_scriptable_object(Bus);
	button_id++;

	Text bus_zone_text("bus", { 50 + settings->get_position().x , 230 + settings->get_position().y }, 40.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* bus_zone_holder = new Text_Menu_object(bus_zone_text, { 50 + settings->get_position().x, -70 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(bus_zone_holder);

	Button* Light_Rail = new Button({ -105.0f + settings->get_position().x, 180.0f + settings->get_position().y }, { 40.0f, 40.f }, this, true, button_id);
	Light_Rail->base_colour = building_constants::LIGHT_RAIL;
	Light_Rail->selected_colour = Bus->base_colour;
	Light_Rail->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	Light_Rail->rendering_layer = m_base_layer;
	Light_Rail->Bind_function(BIND_BUTTON_FN(GUI_Layer::set_transport_type));
	add_scriptable_object(Light_Rail);
	button_id++;

	Text light_rail_zone_text("light rail", { 50 + settings->get_position().x , 180 + settings->get_position().y }, 40.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* light_rail_zone_holder = new Text_Menu_object(light_rail_zone_text, { 50 + settings->get_position().x, -70 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(light_rail_zone_holder);

	Button* Rapid_Transit = new Button({ -105.0f + settings->get_position().x, 130.0f + settings->get_position().y }, { 40.0f, 40.f }, this, true, button_id);
	Rapid_Transit->base_colour = building_constants::RAPID_TRANSIT;
	Rapid_Transit->selected_colour = Bus->base_colour;
	Rapid_Transit->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	Rapid_Transit->rendering_layer = m_base_layer;
	Rapid_Transit->Bind_function(BIND_BUTTON_FN(GUI_Layer::set_transport_type));
	add_scriptable_object(Rapid_Transit);
	button_id++;

	Text rapid_transit_zone_text("rapid transit", { 50 + settings->get_position().x , 130 + settings->get_position().y }, 40.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* rt_zone_holder = new Text_Menu_object(rapid_transit_zone_text, { 50 + settings->get_position().x, -70 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(rt_zone_holder);

	Button* Trains = new Button({ -105.0f + settings->get_position().x, 80.0f + settings->get_position().y }, { 40.0f, 40.f }, this, true, button_id);
	Trains->base_colour = building_constants::TRAINS;
	Trains->selected_colour = Bus->base_colour;
	Trains->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	Trains->rendering_layer = m_base_layer;
	Trains->Bind_function(BIND_BUTTON_FN(GUI_Layer::set_transport_type));
	add_scriptable_object(Trains);
	button_id++;

	Text train_zone_text("train", { 50 + settings->get_position().x , 80 + settings->get_position().y }, 40.0f, { (float)200 / (float)256, (float)200 / (float)256, (float)200 / (float)256, 1.0f }, true);
	Text_Menu_object* train_zone_holder = new Text_Menu_object(train_zone_text, { 50 + settings->get_position().x, -70 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(train_zone_holder);
}

void GUI_Layer::create_building_size_sub_menu()
{
	int button_id = 0;
	m_render = false;
	float y_pos = m_menu_button->get_position().y; 
	
	Menu_Background* menu = new Menu_Background({-140, y_pos}, {160, 90}, this, { 0.09375f, 0.09375f, 0.09375f, 1.0f }, nullptr, m_base_layer);
	menu->Bind_function(BIND_FUNCTION(GUI_Layer::close_size_menu));
	add_scriptable_object(menu);

	Button* large = new Button("Large", { -140, 30 + menu->get_position().y }, { 160, 30 }, this, true, 30.0f, button_id);
	large->base_colour = { 0.09375f, 0.09375f, 0.09375f, 1.0f };
	large->selected_colour = large->base_colour;
	large->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	large->rendering_layer = m_base_layer;
	large->Bind_function(BIND_BUTTON_FN(GUI_Layer::set_zone_size));
	add_scriptable_object(large);
	button_id++;

	Button* medium = new Button("Medium", { -140, menu->get_position().y }, { 160, 30 }, this, true, 30.0f, button_id);
	medium->base_colour = { 0.09375f, 0.09375f, 0.09375f, 1.0f };
	medium->selected_colour = medium->base_colour;
	medium->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	medium->rendering_layer = m_base_layer + 3;
	medium->Bind_function(BIND_BUTTON_FN(GUI_Layer::set_zone_size));
	add_scriptable_object(medium);
	button_id++;

	Button* small = new Button("Small", { -140, -30 + menu->get_position().y }, { 160, 30 }, this, true, 30.0f, button_id);
	small->base_colour = { 0.09375f, 0.09375f, 0.09375f, 1.0f };
	small->selected_colour = small->base_colour;
	small->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	small->rendering_layer = m_base_layer + 6;
	small->Bind_function(BIND_BUTTON_FN(GUI_Layer::set_zone_size));
	add_scriptable_object(small);
	button_id++;

}

void GUI_Layer::create_settings_menu()
{
	int button_id = 0;
	m_render = false;
	m_side = Side::LEFT;
	Menu_Background* settings = new Menu_Background({ -430,0 }, { 420, 640 }, this, { 0.09375f, 0.09375f, 0.09375f, 1.0f }, nullptr, m_base_layer);
	settings->Bind_function(BIND_FUNCTION(GUI_Layer::close_menu));
	add_scriptable_object(settings);

	Text title_text("Overlays", { 0 + settings->get_position().x , 280 + settings->get_position().y }, 60.0f, { (float)220 / (float)256, (float)220 / (float)256, (float)220 / (float)256, 1.0f }, true);
	Text_Menu_object* title = new Text_Menu_object(title_text, { 0 + settings->get_position().x, 280 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(title);

	Button* Transport_overlay = new Button("Transport overlay", { settings->get_position().x, 220 + settings->get_position().y}, {320, 60}, this, true, 50.0f, button_id);
	Transport_overlay->base_colour = { 0.2f, 0.2f, 0.2f, 1.0f };
	Transport_overlay->selected_colour = Transport_overlay->base_colour;
	Transport_overlay->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	Transport_overlay->rendering_layer = m_base_layer + 6;
	Transport_overlay->Bind_function(BIND_BUTTON_FN(GUI_Layer::open_transport_overlay));
	add_scriptable_object(Transport_overlay);
	button_id++;

	Button* Editor_overlay = new Button("Editor overlay", { settings->get_position().x, 150 + settings->get_position().y }, { 320, 60 }, this, true, 50.0f, button_id);
	Editor_overlay->base_colour = { 0.2f, 0.2f, 0.2f, 1.0f };
	Editor_overlay->selected_colour = Editor_overlay->base_colour;
	Editor_overlay->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	Editor_overlay->rendering_layer = m_base_layer + 6;
	Editor_overlay->Bind_function(BIND_BUTTON_FN(GUI_Layer::close_transport_overlay));
	add_scriptable_object(Editor_overlay);
	button_id++;
}

void GUI_Layer::create_button_dropdown()
{

	if (m_orthographic_controller->get_block())
	{
		m_camera_block = false;
	}
	else
	{
		m_orthographic_controller->block_camera(true, menu_key);
		m_camera_block = true;
	}

	editor* temp_layer = dynamic_cast<editor*>(m_caller->get_layer());
	auto data = temp_layer->get_world_data(m_caller->get_id());
	
	if (data->building_type <= 4)
	{
		this->close_dropdown();
		return;
	}

	int button_id = 0;
	m_render = false;

	float y_pos = m_caller->get_position().y - m_orthographic_controller->get_position().y;
	float x_pos = m_caller->get_position().x;

	float x_size = m_caller->get_size().x;
	float y_size = m_caller->get_size().y;

	float menu_x_pos = x_pos + (x_size / 2.0f) + 80.0f;

	Menu_Background* dropdown = new Menu_Background({menu_x_pos, y_pos + (y_size / 2.0f) - 45.0f }, { 160, 90 }, this, { 0.09375f, 0.09375f, 0.09375f, 1.0f }, nullptr, m_base_layer);
	dropdown->Bind_function(BIND_FUNCTION(GUI_Layer::close_dropdown));
	add_scriptable_object(dropdown);

	Button* capacity = new Button("set building capacity", { menu_x_pos, 30 + dropdown->get_position().y }, { 160, 30 }, this, true, 30.0f, button_id);
	capacity->base_colour = { 0.09375f, 0.09375f, 0.09375f, 1.0f };
	capacity->selected_colour = capacity->base_colour;
	capacity->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	capacity->rendering_layer = m_base_layer;
	capacity->Bind_function(BIND_BUTTON_FN(GUI_Layer::open_capacity_popup));
	add_scriptable_object(capacity);
	button_id++;

	Button* staff = new Button("set staff", { menu_x_pos, dropdown->get_position().y }, { 160, 30 }, this, true, 30.0f, button_id);
	staff->base_colour = { 0.09375f, 0.09375f, 0.09375f, 1.0f };
	staff->selected_colour = staff->base_colour;
	staff->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	staff->rendering_layer = m_base_layer + 3;
	staff->Bind_function(BIND_BUTTON_FN(GUI_Layer::open_staff_popup));
	add_scriptable_object(staff);
	button_id++;
	
	Button* opening_hours = new Button("set opening hours", { menu_x_pos, -30 + dropdown->get_position().y }, { 160, 30 }, this, true, 30.0f, button_id);
	opening_hours->base_colour = { 0.09375f, 0.09375f, 0.09375f, 1.0f };
	opening_hours->selected_colour = opening_hours->base_colour;
	opening_hours->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	opening_hours->rendering_layer = m_base_layer + 6;
	opening_hours->Bind_function(BIND_BUTTON_FN(GUI_Layer::open_opening_popup));
	add_scriptable_object(opening_hours);
	button_id++;
}

void GUI_Layer::create_colour_dropdown()
{
	int button_id = 0;
	m_render = false;
	float y_pos = m_caller->get_position().y;
	float x_pos = m_caller->get_position().x;

	dynamic_cast<GUI_Layer*>(m_caller->get_layer())->disable_events();

	Menu_Background* menu = new Menu_Background({ 40 , y_pos - 180 }, { 340, 320 }, this, { 0.09375f, 0.09375f, 0.09375f, 1.0f }, nullptr, m_base_layer);
	menu->Bind_function(BIND_FUNCTION(GUI_Layer::close_colour_sub));
	add_scriptable_object(menu);

	colour_palette* colour = new colour_palette({ menu->get_position().x - 10.0f, menu->get_position().y + 8.0f }, this, m_base_layer, button_id);
	add_scriptable_object(colour);
}

void GUI_Layer::create_capacity_popup()
{
	int button_id = 0;
	m_render = false;
	m_side = Side::RIGHT;
	Menu_Background* popup = new Menu_Background({ 430,0 }, { 420, 640 }, this, { 0.09375f, 0.09375f, 0.09375f, 1.0f }, nullptr, m_base_layer);
	popup->Bind_function(BIND_FUNCTION(GUI_Layer::close_popup));
	add_scriptable_object(popup);

	Text title_text("Capacity", { 0 + popup->get_position().x , 280 + popup->get_position().y }, 60.0f, { (float)220 / (float)256, (float)220 / (float)256, (float)220 / (float)256, 1.0f }, true);
	Text_Menu_object* title = new Text_Menu_object(title_text, { 0 + popup->get_position().x, 280 + popup->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(title);

	Text_Box* capacity = new Text_Box({ 0 + popup->get_position().x, 200 + popup->get_position().y }, { 200, 30 }, this, true, m_base_layer, true, button_id);
	add_scriptable_object(capacity);
	button_id++;

	Button* save = new Button("Save", { 0 + popup->get_position().x, -260 + popup->get_position().y}, {150, 60}, this, true, 45.0f, button_id);
	save->base_colour = { 0.09375f, 0.09375f, 0.09375f, 1.0f };
	save->box_colour = { 0.8f, 0.8f, 0.8f, 1.0f };
	save->selected_colour = { 0.2f, 0.2f, 0.2f, 1.0f };
	save->rendering_layer = m_base_layer;
	save->Bind_function(BIND_BUTTON_FN(GUI_Layer::save_popup));
	add_scriptable_object(save);
	button_id++;
}

void GUI_Layer::create_staff_popup()
{
	int button_id = 0;
	m_render = false;
	m_side = Side::RIGHT;
	Menu_Background* popup = new Menu_Background({ 430,0 }, { 420, 640 }, this, { 0.09375f, 0.09375f, 0.09375f, 1.0f }, nullptr, m_base_layer);
	popup->Bind_function(BIND_FUNCTION(GUI_Layer::close_popup));
	add_scriptable_object(popup);

	Text title_text("Staff amount", { 0 + popup->get_position().x , 280 + popup->get_position().y }, 60.0f, { (float)220 / (float)256, (float)220 / (float)256, (float)220 / (float)256, 1.0f }, true);
	Text_Menu_object* title = new Text_Menu_object(title_text, { 0 + popup->get_position().x, 280 + popup->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(title);

	Text_Box* staff = new Text_Box({ 0 + popup->get_position().x, 200 + popup->get_position().y }, { 200, 30 }, this, true, m_base_layer, true, button_id);
	add_scriptable_object(staff);
	button_id++;

	Button* save = new Button("Save", { 0 + popup->get_position().x, -260 + popup->get_position().y }, { 150, 60 }, this, true, 45.0f, button_id);
	save->base_colour = { 0.09375f, 0.09375f, 0.09375f, 1.0f };
	save->box_colour = { 0.8f, 0.8f, 0.8f, 1.0f };
	save->selected_colour = { 0.2f, 0.2f, 0.2f, 1.0f };
	save->rendering_layer = m_base_layer;
	save->Bind_function(BIND_BUTTON_FN(GUI_Layer::save_popup));
	add_scriptable_object(save);
	button_id++;
}

void GUI_Layer::create_opening_popup()
{
	int button_id = 0;
	m_render = false;
	m_side = Side::RIGHT;
	Menu_Background* popup = new Menu_Background({ 430,0 }, { 420, 640 }, this, { 0.09375f, 0.09375f, 0.09375f, 1.0f }, nullptr, m_base_layer);
	popup->Bind_function(BIND_FUNCTION(GUI_Layer::close_popup));
	add_scriptable_object(popup);

	Text title_text("Hours (1 count = 1 minute)", { 0 + popup->get_position().x , 280 + popup->get_position().y }, 60.0f, { (float)220 / (float)256, (float)220 / (float)256, (float)220 / (float)256, 1.0f }, true);
	Text_Menu_object* title = new Text_Menu_object(title_text, { 0 + popup->get_position().x, 280 + popup->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(title);


	Text open_text("Open", { 0 + popup->get_position().x , 240 + popup->get_position().y }, 40.0f, { (float)220 / (float)256, (float)220 / (float)256, (float)220 / (float)256, 1.0f }, true);
	Text_Menu_object* open_container = new Text_Menu_object(open_text, { 0 + popup->get_position().x, 280 + popup->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(open_container);

	Text_Box* open = new Text_Box({ 0 + popup->get_position().x, 200 + popup->get_position().y }, { 200, 30 }, this, true, m_base_layer, true, button_id);
	add_scriptable_object(open);
	button_id++;

	Text closed_text("Closed", { 0 + popup->get_position().x , 120 + popup->get_position().y }, 40.0f, { (float)220 / (float)256, (float)220 / (float)256, (float)220 / (float)256, 1.0f }, true);
	Text_Menu_object* closed_container = new Text_Menu_object(closed_text, { 0 + popup->get_position().x, 280 + popup->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(closed_container);

	Text_Box* close = new Text_Box({ 0 + popup->get_position().x, 80 + popup->get_position().y }, { 200, 30 }, this, true, m_base_layer, true, button_id);
	add_scriptable_object(close);
	button_id++;

	Button* save = new Button("Save", { 0 + popup->get_position().x, -260 + popup->get_position().y }, { 150, 60 }, this, true, 45.0f, button_id);
	save->base_colour = { 0.09375f, 0.09375f, 0.09375f, 1.0f };
	save->box_colour = { 0.8f, 0.8f, 0.8f, 1.0f };
	save->selected_colour = { 0.2f, 0.2f, 0.2f, 1.0f };
	save->rendering_layer = m_base_layer;
	save->Bind_function(BIND_BUTTON_FN(GUI_Layer::save_popup));
	add_scriptable_object(save);
	button_id++;
}

void GUI_Layer::create_line_manager()
{
	int button_id = 0;
	m_render = false;
	m_side = Side::LEFT;
	Menu_Background* settings = new Menu_Background({ -430,0 }, { 420, 640 }, this, { 0.09375f, 0.09375f, 0.09375f, 1.0f }, nullptr, m_base_layer);
	settings->Bind_function(BIND_FUNCTION(GUI_Layer::close_menu));
	add_scriptable_object(settings);

	Text title_text("Line Manager", { 0 + settings->get_position().x , 280 + settings->get_position().y }, 60.0f, { (float)220 / (float)256, (float)220 / (float)256, (float)220 / (float)256, 1.0f }, true);
	Text_Menu_object* title = new Text_Menu_object(title_text, { 0 + settings->get_position().x, 280 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(title);

	Scrollable_Menu* menu = new Scrollable_Menu({ settings->get_position().x, settings->get_position().y }, {400, 500}, this, 0, 0);
	add_scriptable_object(menu);

	Button* new_line= new Button("Add new line", { settings->get_position().x, 220 + settings->get_position().y }, { 320, 60 }, this, true, 50.0f, button_id);
	new_line->base_colour = { 0.2f, 0.2f, 0.2f, 1.0f };
	new_line->selected_colour = new_line->base_colour;
	new_line->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	new_line->rendering_layer = m_base_layer + 6;
	new_line->Bind_function(BIND_BUTTON_FN(GUI_Layer::new_line));
	add_scriptable_object(new_line);
	menu->add_object(new_line);
	button_id++;
}

void GUI_Layer::create_line_editor()
{
	int button_id = 0;
	m_render = false;
	m_side = Side::RIGHT;
	Menu_Background* settings = new Menu_Background({ 430,0 }, { 420, 640 }, this, { 0.09375f, 0.09375f, 0.09375f, 1.0f }, nullptr, m_base_layer);
	settings->Bind_function(BIND_FUNCTION(GUI_Layer::close_line_editor));
	add_scriptable_object(settings);

	Text title_text("Line Editor", { 0 + settings->get_position().x , 280 + settings->get_position().y }, 60.0f, { (float)220 / (float)256, (float)220 / (float)256, (float)220 / (float)256, 1.0f }, true);
	Text_Menu_object* title = new Text_Menu_object(title_text, { 0 + settings->get_position().x, 280 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(title);

	Text name_text("Line name", { 0 + settings->get_position().x , 240 + settings->get_position().y }, 40.0f, { (float)220 / (float)256, (float)220 / (float)256, (float)220 / (float)256, 1.0f }, true);
	Text_Menu_object* name_t = new Text_Menu_object(name_text, { 0 + settings->get_position().x, 280 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(name_t);

	Text_Box* name = new Text_Box({ 0 + settings->get_position().x, 210 + settings->get_position().y }, { 200, 30 }, this, false, m_base_layer, true, button_id);
	add_scriptable_object(name);
	button_id++;

	Button* line_colour = new Button({ -105.0f + settings->get_position().x, 160.0f + settings->get_position().y }, { 40.0f, 40.f }, this, true, button_id);
	line_colour->base_colour = building_constants::PLACE_OF_WORSHIP;
	line_colour->selected_colour = line_colour->base_colour;
	line_colour->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	line_colour->rendering_layer = m_base_layer;
	line_colour->Bind_function(BIND_BUTTON_FN(GUI_Layer::open_colour_sub));
	add_scriptable_object(line_colour);
	button_id++;

	Text line_c("Line colour", { 50 + settings->get_position().x , 160 + settings->get_position().y }, 40.0f, { (float)220 / (float)256, (float)220 / (float)256, (float)220 / (float)256, 1.0f }, true);
	Text_Menu_object* line_c_holder = new Text_Menu_object(line_c, { 0 + settings->get_position().x, 280 + settings->get_position().y }, this, m_base_layer + 2);
	add_scriptable_object(line_c_holder);

	Button* show_line = new Button("Toggle line view", { settings->get_position().x, 90.0f + settings->get_position().y }, { 320.0f, 60.0f }, this, true, 50.0f, button_id);
	show_line->base_colour = { 0.2f, 0.2f, 0.2f, 1.0f };
	show_line->selected_colour = show_line->base_colour;
	show_line->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	show_line->rendering_layer = m_base_layer + 6;
	//show_line->Bind_function(BIND_BUTTON_FN(GUI_Layer::new_line));
	add_scriptable_object(show_line);
	button_id++;

	Scrollable_Menu* menu = new Scrollable_Menu({ settings->get_position().x, -50.0f + settings->get_position().y }, { 320, 200 }, this, 0, 0);
	add_scriptable_object(menu);
	menu->no_bounds = true;

	Button* add_stop = new Button("add station", { settings->get_position().x, -200 + settings->get_position().y }, { 320.0f, 60.0f }, this, true, 50.0f, button_id);
	add_stop->base_colour = { 0.2f, 0.2f, 0.2f, 1.0f };
	add_stop->selected_colour = show_line->base_colour;
	add_stop->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	add_stop->rendering_layer = m_base_layer + 6;
	add_stop->Bind_function(BIND_BUTTON_FN(GUI_Layer::add_stop_fn));
	add_scriptable_object(add_stop);
	button_id++;

	Button* remove_stop = new Button("remove station", { settings->get_position().x, -270 + settings->get_position().y }, { 320.0f, 60.0f }, this, true, 50.0f, button_id);
	remove_stop->base_colour = { 0.2f, 0.2f, 0.2f, 1.0f };
	remove_stop->selected_colour = show_line->base_colour;
	remove_stop->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	remove_stop->rendering_layer = m_base_layer + 6;
	//show_line->Bind_function(BIND_BUTTON_FN(GUI_Layer::new_line));
	add_scriptable_object(remove_stop);
	button_id++;
	
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

void GUI_Layer::open_transport_overlay()
{
	Events::Transport_Overlay_Event event(true);
	Event_Call_back(event);
}

void GUI_Layer::close_transport_overlay()
{
	//editor::get()->get_overlay();
	Events::Transport_Overlay_Event event(false);
	Event_Call_back(event);
}

void GUI_Layer::open_line_editor()
{
	if (m_orthographic_controller->get_block())
	{
		m_camera_block = false;
	}
	else
	{
		m_orthographic_controller->block_camera(true, menu_key);
		m_camera_block = true;
	}

	std::string key = "";
	for (auto obj : m_objects)
	{
		if (obj->get_id() == m_selected && obj->get_type() == entity_type::BUTTON)
		{
			key = dynamic_cast<Button*>(obj)->get_text();

			if (key != "Add new line" && key != "")
			{
				break;
			}
			else if(key == "Add new line")
			{
				key = "";
			}
		}
	}

	Events::GUI_Line_Editor_Event event(true);
	
	if (key != "")
	{
		event.add_key(key);
	}

	Event_Call_back(event);
}

void GUI_Layer::update_line_manager()
{
	auto transport = dynamic_cast<Transport_Layer*>(m_call_layer);
	
	auto lines = transport->get_lines();
	std::vector<scriptable_object*> objects; 

	for (auto obj : m_objects)
	{
		if (obj->get_type() == entity_type::SCROLLABLE_MENU)
		{
			objects = dynamic_cast<Scrollable_Menu*>(obj)->get_list();
		}
	}

	int i = 0;
	for (auto it = lines.begin(); it != lines.end(); it++)
	{
		auto button = dynamic_cast<Button*>(objects[i]);
		if (button->get_text() == "new line")
		{
			button->change_text(it->first);
		}
		i++;

	}
}

void GUI_Layer::close_line_editor()
{
	//Events::GUI_Line_Editor_Event event(false);
	//Event_Call_back(event);

	Transport_Layer* transport = dynamic_cast<Transport_Layer*>(m_call_layer);
	std::shared_ptr<Line> line;

	for (scriptable_object* obj : m_objects)
	{
		if (obj->get_type() == entity_type::TEXT_BOX)
		{
			std::string name = dynamic_cast<Text_Box*>(obj)->get_string();
			if (name == "")
			{
				break;
			}
			line = transport->update_line(name);
			line->name = name;
		}
		else if (obj->get_type() == entity_type::BUTTON && obj->get_id() == 1)
		{
			line->colour = dynamic_cast<Button*>(obj)->base_colour;
		}
		else if (obj->get_type() == entity_type::SCROLLABLE_MENU)
		{
			std::vector<scriptable_object*> list = dynamic_cast<Scrollable_Menu*>(obj)->get_list();
			for (int i = 0; i < list.size(); i++)
			{
				line->stops.push_back(list[i]->get_id());
			}
		}
	}

	close_menu();
}

void GUI_Layer::new_line()
{
	//glm::vec2 menu_size(0);
	//for (scriptable_object* obj : m_objects)
	//{
	//	if (dynamic_cast<Menu_Background*>(obj) != nullptr)
	//	{
	//		menu_size = { obj->get_position().y + obj->get_size().y / 2.0f, obj->get_position().y - obj->get_size().y / 2.0f };
	//		break;
	//	}
	//}

	Scrollable_Menu* menu = nullptr;
	for (auto obj : m_objects)
	{
		menu = dynamic_cast<Scrollable_Menu*>(obj);
		if (menu != nullptr)
		{
			break;
		}
	}

	int button_id = 0;
	glm::vec2 pos(0);
	scriptable_object* button;
	for (scriptable_object* obj : m_objects)
	{
		if (dynamic_cast<Button*>(obj) != nullptr)
		{
			if (obj->get_id() == 0)
			{
				pos = obj->get_position();

				if (!menu->check_valid(pos.y - 80.0f))
				{
					pos.y = menu->get_top() + 80.0f;
				}
				else
				{
					obj->change_position({ pos.x, pos.y - 80.0f });
				}

				button = obj;
			}
			if (obj->get_id() >= button_id)
			{
				button_id = obj->get_id();
			}
		}
	}

	Button* new_line = new Button("new line", { pos.x, pos.y }, { 320, 60 }, this, true, 50.0f, button_id + 1);
	new_line->base_colour = { 0.2f, 0.2f, 0.2f, 1.0f };
	new_line->selected_colour = new_line->base_colour;
	new_line->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
	new_line->rendering_layer = m_base_layer + 6;
	new_line->Bind_function(BIND_BUTTON_FN(GUI_Layer::open_line_editor));
	add_scriptable_object(new_line);

	menu->add_object(new_line);
	open_line_editor();
}

void GUI_Layer::load_line(std::string key)
{
	if (m_type != Type::LineEditor)
	{
		return;
	}

	if (key == "new line")
	{
		return;
	}

	auto lines = dynamic_cast<Transport_Layer*>(m_call_layer)->get_lines();
	auto line = lines[key];

	Transport_Layer::get()->set_active_line(key);

	for (auto obj : m_objects)
	{
		if (obj->get_type() == entity_type::TEXT_BOX)
		{
			Text_Box* text_box = dynamic_cast<Text_Box*>(obj);
			text_box->set_string(line->name);
		}
		else if (obj->get_type() == entity_type::BUTTON && obj->get_id() == 1)
		{
			Button* button = dynamic_cast<Button*>(obj);
			button->base_colour = line->colour;
			button->selected_colour = button->base_colour;
		}
	}
}

void GUI_Layer::add_stop(bool start)
{
	Transport_Layer* t_overlay = Transport_Layer::get();

	if (t_overlay->get_active_line() == "")
	{
		return;
	}

	std::shared_ptr<Line> active_line = t_overlay->get_line(t_overlay->get_active_line());
	auto e_overlay = editor::get();

	if (start)
	{
		if (active_line->type != Transport_Type::NONE)
		{
			//will need to rework this when transport get's reworked

			std::unordered_map<uint32_t, std::shared_ptr<button_data>> button_data = e_overlay->get_world_data_list();
			for (auto it = button_data.begin(); it != button_data.end(); it++)
			{
				if (!it->second->transport_building)
				{
					continue;
				}

				if (it->second->type == active_line->type)
				{
					continue;
				}

				it->second->render = false;
			}

		}

		e_overlay->bind_transport_select(true);
		return;
	}
	
	if (!start)
	{
		active_line->stops.push_back(e_overlay->selected());

		for (auto obj : m_objects)
		{
			if (obj->get_type() == entity_type::SCROLLABLE_MENU)
			{
				auto menu = dynamic_cast<Scrollable_Menu*>(obj);
				
				Button* new_line = new Button("", { 0, 0 }, { 320, 60 }, this, true, 50.0f, 0);
				new_line->base_colour = { 0.2f, 0.2f, 0.2f, 1.0f };
				new_line->selected_colour = new_line->base_colour;
				new_line->box_colour = { 1.0f, 1.0f, 1.0f, 1.0f };
				new_line->rendering_layer = m_base_layer + 6;

				glm::vec2 pos = e_overlay->get_position(e_overlay->selected());
				
				std::stringstream ss;
				ss << "( " << pos.x << " , " << pos.y << " )";
				std::string text = ss.str();

				new_line->change_text(text);
				menu->insert_below(new_line);
				add_scriptable_object(new_line);
			}
		}

	}

}

void GUI_Layer::remove_stop()
{
}

void GUI_Layer::open_public_sub()
{
	Menu_Background* temp_menu = nullptr;
	for (scriptable_object* obj : m_objects)
	{
		temp_menu = dynamic_cast<Menu_Background*>(obj);
		if (temp_menu != nullptr)
		{
			temp_menu->Bind_function(BIND_FUNCTION(GUI_Layer::defualt_func));
			break;
		}
	}


	Events::GUI_Public_Building_Event event(m_caller, temp_menu);
	Event_Call_back(event);
}

void GUI_Layer::open_transport_sub()
{
	Menu_Background* temp_menu = nullptr;
	for (scriptable_object* obj : m_objects)
	{
		temp_menu = dynamic_cast<Menu_Background*>(obj);
		if (temp_menu != nullptr)
		{
			temp_menu->Bind_function(BIND_FUNCTION(GUI_Layer::defualt_func));
			break;
		}
	}

	Events::GUI_Transport_Building_Event event(m_caller, temp_menu);
	Event_Call_back(event);
}

void GUI_Layer::open_size_sub()
{
	for (scriptable_object* obj : m_objects)
	{
		Button* temp_button = dynamic_cast<Button*>(obj);
		if (temp_button != nullptr && temp_button->get_id() == m_selected)
		{
			Events::GUI_Building_Size_Event event(m_caller, obj);
			Event_Call_back(event);
		}
	}
}

void GUI_Layer::open_staff_popup()
{
	Events::Popup_Staff_Event event(m_caller);
	Event_Call_back(event);
}

void GUI_Layer::open_capacity_popup()
{
	if (m_orthographic_controller->get_block())
	{
		m_camera_block = false;
	}
	else
	{
		m_orthographic_controller->block_camera(true, menu_key);
		m_camera_block = true;
	}

	Events::Popup_Capacity_Event event(m_caller);
	Event_Call_back(event);
}

void GUI_Layer::open_opening_popup()
{
	Events::Popup_Opening_Event event(m_caller);
	Event_Call_back(event);
}

void GUI_Layer::open_colour_sub()
{
	for (scriptable_object* obj : m_objects)
	{
		Button* temp_button = dynamic_cast<Button*>(obj);
		if (temp_button != nullptr && temp_button->get_id() == m_selected)
		{
			Events::GUI_Colour_Event event(obj);
			Event_Call_back(event);
		}
	}
}

void GUI_Layer::defualt_func()
{
	//a empty function
}

void GUI_Layer::side_check()
{
	if (m_side == Side::LEFT && s_lmenu_active == true)
	{
		m_render = false;

		if (m_camera_block)
		{
			m_orthographic_controller->block_camera(false, menu_key);
		}

		return;
	}
	else if (m_side == Side::LEFT && s_lmenu_active == false)
	{
		s_lmenu_active = true;
		return;
	}
	
	if (m_side == Side::RIGHT && s_rmenu_active == true)
	{
		m_render = false;

		if (m_camera_block)
		{
			m_orthographic_controller->block_camera(false, menu_key);
		}

		return;
	}
	else if (m_side == Side::RIGHT && s_rmenu_active == false)
	{
		s_rmenu_active = true;
		return;
	}
}

void GUI_Layer::menu_close()
{
	if (m_side == Side::LEFT)
	{
		s_lmenu_active = false;
	}
	else if (m_side == Side::RIGHT)
	{
		s_rmenu_active = false;
	}
}

void GUI_Layer::close_menu()
{
	m_render = false;
	menu_close();

	if (m_camera_block)
	{
		m_orthographic_controller->block_camera(false, menu_key);
	}

	Button* caller_button = dynamic_cast<Button*>(m_caller);
	if (caller_button != nullptr)
	{
		caller_button->change_state(false);
	}

	for (scriptable_object* obj : m_objects)
	{
		Text_Box* text_box = dynamic_cast<Text_Box*>(obj);
		if (text_box != nullptr)
		{
			text_box->clear_box();
		}
	}
}

void GUI_Layer::close_pb_menu()
{
	m_render = false;
	menu_close();
	Button* caller_button = dynamic_cast<Button*>(m_caller);
	if (caller_button != nullptr)
	{
		caller_button->change_state(false);
	}

	Menu_Background* temp_menu = dynamic_cast<Menu_Background*>(m_prev_menu);
	GUI_Layer* menu_layer = dynamic_cast<GUI_Layer*>(temp_menu->get_layer());
	temp_menu->Bind_function(std::bind(&GUI_Layer::close_menu, menu_layer));
}

void GUI_Layer::close_tb_menu()
{
	m_render = false;
	//menu_close();
	Button* caller_button = dynamic_cast<Button*>(m_caller);
	if (caller_button != nullptr)
	{
		caller_button->change_state(false);
	}

	Menu_Background* temp_menu = dynamic_cast<Menu_Background*>(m_prev_menu);
	GUI_Layer* menu_layer = dynamic_cast<GUI_Layer*>(temp_menu->get_layer());
	temp_menu->Bind_function(std::bind(&GUI_Layer::close_menu, menu_layer));

	close_menu();
}


void GUI_Layer::close_size_menu()
{
	for (scriptable_object* obj : m_objects)
	{
		obj->delete_obj(true);
	}
	m_render = false;
}

void GUI_Layer::close_dropdown()
{
	Button* caller_button = dynamic_cast<Button*>(m_caller);
	if (caller_button != nullptr)
	{
		caller_button->change_state(false);
	}

	for (scriptable_object* obj : m_objects)
	{
		obj->delete_obj(true);
	}
	m_render = false;

}

void GUI_Layer::close_colour_sub()
{
	Button* caller_button = dynamic_cast<Button*>(m_caller);
	if (caller_button != nullptr)
	{
		caller_button->change_state(false);
	}

	for (scriptable_object* obj : m_objects)
	{
		obj->delete_obj(true);
	}
	m_render = false;
	dynamic_cast<GUI_Layer*>(m_caller->get_layer())->enable_events();
}

void GUI_Layer::save_popup()
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

	Button* caller_button = dynamic_cast<Button*>(m_caller);
	editor* temp_layer = dynamic_cast<editor*>(caller_button->get_layer());
	auto data = temp_layer->get_world_data(caller_button->get_id());

	switch (this->get_type())
	{
	case Type::CapacityPopup:

		for (scriptable_object* obj : m_objects)
		{
			if (obj->get_type() == entity_type::TEXT_BOX)
			{
				Text_Box* box = dynamic_cast<Text_Box*>(obj);
				if (box != nullptr)
				{
					if (box->get_id() == 0)
					{
						std::string input = box->get_string();
						auto it = input.begin();

						while (it != input.end() && std::isdigit(*it)) ++it;

						if (it != input.end())
						{
							dialouge_box* not_int = new dialouge_box(input + " is not a valid number", { 0.0f, 0.0f }, { 600.0f, 200.0f }, this, m_base_layer + 4);
							m_objects.push_back(not_int);
							m_dialog_box = true;
							return;
						}

						data->capacity = stoi(box->get_string());
						data->capacity_action_needed = false;
						data->action_needed = (data->capacity_action_needed || data->staff_action_needed || data->opening_action_needed);
					}
				}
			}
		}
		break;
	case Type::StaffPopup:
		for (scriptable_object* obj : m_objects)
		{
			if (obj->get_type() == entity_type::TEXT_BOX)
			{
				Text_Box* box = dynamic_cast<Text_Box*>(obj);
				if (box != nullptr)
				{
					if (box->get_id() == 0)
					{
						std::string input = box->get_string();
						auto it = input.begin();

						while (it != input.end() && std::isdigit(*it)) ++it;

						if (it != input.end())
						{
							dialouge_box* not_int = new dialouge_box(input + " is not a valid number", { 0.0f, 0.0f }, { 600.0f, 200.0f }, this, m_base_layer + 4);
							m_objects.push_back(not_int);
							m_dialog_box = true;
							return;
						}

						data->staff = stoi(box->get_string());
						data->staff_action_needed = false;
						data->action_needed = (data->capacity_action_needed || data->staff_action_needed || data->opening_action_needed);
					}
				}
			}
		}
		break;
	case Type::OpeningPopup:
		for (scriptable_object* obj : m_objects)
		{
			if (obj->get_type() == entity_type::TEXT_BOX)
			{
				Text_Box* box = dynamic_cast<Text_Box*>(obj);
				if (box != nullptr)
				{
					std::string input = box->get_string();
					auto it = input.begin();

					while (it != input.end() && std::isdigit(*it)) ++it;

					if (it != input.end())
					{
						dialouge_box* not_int = new dialouge_box(input + " is not a valid number", { 0.0f, 0.0f }, { 600.0f, 200.0f }, this, m_base_layer + 4);
						m_objects.push_back(not_int);
						m_dialog_box = true;
						return;
					}

					if (box->get_id() == 0)
					{
						data->opening_hours.first = stoi(box->get_string());
					}
					else if (box->get_id() == 1)
					{
						data->opening_hours.second = stoi(box->get_string());
					}
				}
			}
		}

		data->opening_action_needed = false;
		data->action_needed = (data->capacity_action_needed || data->staff_action_needed || data->opening_action_needed);

		break;
	default:
		break;
	}

	close_menu();
}

void GUI_Layer::close_popup()
{
	close_menu();
}
