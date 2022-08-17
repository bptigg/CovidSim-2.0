#include "GUI_Layer.h"

#include "editor.h"
#include "../Entity/building_constants.h"

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
	m_camera_block = false;
}

GUI_Layer::~GUI_Layer()
{
	On_Detach();
}

void GUI_Layer::On_Attach(std::vector<std::pair<std::string, std::string>> textures)
{

	if (m_attached)
	{
		if (m_type != Type::BuildingSizeSubMenu && m_type != Type::ButtonDropDown)
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
		create_settings_menu(0);
		break;
	case Type::BuildingSelectMenu:
		create_building_menu();
		break;
	case Type::PublicBuildingSubMenu:
		create_public_building_sub_menu();
		break;
	case Type::BuildingSizeSubMenu:
		create_building_size_sub_menu();
		break;
	case Type::ButtonDropDown:
		create_button_dropdown();
		break;
	case Type::CapacityPopup:
		create_capacity_popup();
		break;
	case Type::StaffPopup:
		create_staff_popup();
		break;
	case Type::OpeningPopup:
		create_opening_popup();
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
	if (m_render)
	{
		bool dont_unblock = m_orthographic_controller->block;
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
		if (!dont_unblock)
		{
			m_orthographic_controller->block = false;
		}
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
				}
				m_caller = nullptr;
				break;
			}
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


	//place holders
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
	Menu_Background* settings = new Menu_Background({ -430,0 }, { 420, 640 }, this, { 0.09375f, 0.09375f, 0.09375f, 1.0f }, nullptr, m_base_layer);
	settings->Bind_function(BIND_FUNCTION(GUI_Layer::close_menu));
	add_scriptable_object(settings);
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

void GUI_Layer::create_button_dropdown()
{
	editor* temp_layer = dynamic_cast<editor*>(m_caller->get_layer());
	auto data = temp_layer->get_world_data(m_caller->get_id());
	
	if (data->building_type <= 4)
	{
		this->close_dropdown();
		return;
	}

	int button_id = 0;
	m_render = false;

	float y_pos = m_caller->get_position().y;
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

void GUI_Layer::create_capacity_popup()
{
	int button_id = 0;
	m_render = false;
	Menu_Background* popup = new Menu_Background({ 430,0 }, { 420, 640 }, this, { 0.09375f, 0.09375f, 0.09375f, 1.0f }, nullptr, m_base_layer);
	popup->Bind_function(BIND_FUNCTION(GUI_Layer::close_menu));
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
	Menu_Background* popup = new Menu_Background({ 430,0 }, { 420, 640 }, this, { 0.09375f, 0.09375f, 0.09375f, 1.0f }, nullptr, m_base_layer);
	popup->Bind_function(BIND_FUNCTION(GUI_Layer::close_menu));
	add_scriptable_object(popup);
}

void GUI_Layer::create_opening_popup()
{
	int button_id = 0;
	m_render = false;
	Menu_Background* popup = new Menu_Background({ 430,0 }, { 420, 640 }, this, { 0.09375f, 0.09375f, 0.09375f, 1.0f }, nullptr, m_base_layer);
	popup->Bind_function(BIND_FUNCTION(GUI_Layer::close_menu));
	add_scriptable_object(popup);
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
	Events::GUI_Transport_Building_Event event(m_caller);
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
	if (!m_orthographic_controller->block)
	{
		m_camera_block = false;
	}
	else
	{
		m_orthographic_controller->block = true;
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

void GUI_Layer::defualt_func()
{
	//a empty function
}

void GUI_Layer::close_menu()
{
	m_render = false;

	if (m_camera_block)
	{
		m_orthographic_controller->block = false;
	}

	Button* caller_button = dynamic_cast<Button*>(m_caller);
	if (caller_button != nullptr)
	{
		caller_button->change_state(false);
	}
}

void GUI_Layer::close_pb_menu()
{
	m_render = false;
	Button* caller_button = dynamic_cast<Button*>(m_caller);
	if (caller_button != nullptr)
	{
		caller_button->change_state(false);
	}

	Menu_Background* temp_menu = dynamic_cast<Menu_Background*>(m_prev_menu);
	GUI_Layer* menu_layer = dynamic_cast<GUI_Layer*>(temp_menu->get_layer());
	temp_menu->Bind_function(std::bind(&GUI_Layer::close_menu, menu_layer));
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
		break;
	case Type::OpeningPopup:
		break;
	default:
		break;
	}

	close_menu();
}

void GUI_Layer::close_popup()
{
}
