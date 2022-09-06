#pragma once
#include "../Layer.h"
#include "../file.h"

#include "../Entity/Button.h"
#include "../Entity/Menu_Background.h"
#include "../Entity/Text_Box.h"
#include "../Entity/Text_Menu_object.h"
#include "../Entity/dialouge_box.h"
#include "../Entity/Scrollable_Menu.h"

#include "../Events/Gui_Events.h"
#include "../Events/Transport_Events.h"



class GUI_Layer : public Layer
{
public:
	enum class Type
	{
		Overlay,
		BuildingSelectMenu,
		PublicBuildingSubMenu,
		PublicTransportSubMenu,
		BuildingSizeSubMenu,
		ButtonDropDown,
		CapacityPopup,
		StaffPopup,
		OpeningPopup,
		SettingsMenu,
		SetupMenu,
		LineManager,
		LineEditor
	};

	enum class Side
	{
		LEFT,
		RIGHT,
		NONE
	};

	//const bool dialog_box = &m_dialog_box;
private:
	Type m_type;
	std::vector<scriptable_object*> m_objects;

	std::unordered_map<std::string, unsigned int> m_textures;

	unsigned int m_base_layer;
	Side m_side;

	
	//bool m_dialog_box = false;

	std::shared_ptr<Camera_Controller> m_orthographic_controller;
	bool m_camera_block;

	uint32_t menu_key;

#pragma region settings_menu
	bool sm_saved;
#pragma endregion

#pragma region zone_menu
	unsigned int m_selected;
	scriptable_object* m_caller;
	scriptable_object* m_prev_menu;
#pragma endregion;

#pragma region transport
	Layer* m_call_layer;
#pragma endregiom

#pragma region size_menu
	scriptable_object* m_menu_button;
	//also uses the m_caller found in region zone_menu
#pragma endregion

public:
	GUI_Layer(Type menu_type, unsigned int base_layer, std::shared_ptr<Camera_Controller> ortho_controll);
	virtual ~GUI_Layer() override;

	virtual void On_Attach(std::vector<std::pair<std::string, std::string>> textures) override;
	virtual void On_Detach() override;
	virtual void On_Update(Timestep ts) override;
	virtual void On_ImGui_Render() override;
	virtual void On_Event(Events::Event& e) override;
	virtual void render(bool render) override;

	inline Type get_type() { return m_type; }
	inline virtual unsigned int get_selected() override { return m_selected; }

#pragma region zone_menu
	inline virtual void set_selected(unsigned int num) override { m_selected = num; }

	void set_caller(scriptable_object* caller);
	void set_prev_menu(scriptable_object* caller);
	void set_menu(scriptable_object* p_menu);
	void set_call_layer(Layer* layer);
	void change_box_colour();
	void change_box_texture();
	//void change_box_colour_sub_menu(); 

	void set_transport_type();
	void set_zone_size();
	inline void close_public_menu()
	{
		close_pb_menu();
	}

	inline void set_building_type()
	{

	}

	void update_line_manager();
#pragma endregion

private:
	void add_scriptable_object(scriptable_object* obj);
	inline std::vector<scriptable_object*> reverse_vector(std::vector<scriptable_object*> vec)
	{
		std::reverse(vec.begin(), vec.end());
		return vec;
	}
	
	void create_settings_menu(unsigned int menu);
	void create_building_menu();
	void create_public_building_sub_menu();
	void create_public_transport_sub_menu();
	void create_building_size_sub_menu();
	void create_settings_menu();
	void create_button_dropdown();

	void create_capacity_popup();
	void create_staff_popup();
	void create_opening_popup();

	void create_line_manager();
	void create_line_editor();

	std::vector<scriptable_object*>& get_objects() { return m_objects; }


#pragma region exit_functions

	void setting_exit_func();
	void save_exit_func();
	void save_exit_func_2();

	void close_menu();
	void close_pb_menu();
	void close_tb_menu();
	void close_size_menu();
	void close_dropdown();

	void save_popup();
	void close_popup();
#pragma endregion

#pragma region settings_pages
	void page_one();
	void page_two();
#pragma endregion

	void open_transport_overlay();
	void close_transport_overlay();

	void open_line_editor();
	void close_line_editor();

	void new_line();

#pragma region sub_menus
	void open_public_sub();
	void open_transport_sub();
	void open_size_sub();
	void open_staff_popup();
	void open_capacity_popup();
	void open_opening_popup();

#pragma endregion

	void defualt_func();
	void side_check();
	void menu_close();

	//void save_data();
};

