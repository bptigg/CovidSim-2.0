#pragma once
#include "../Layer.h"
#include "../file.h"

#include "../Entity/Button.h"
#include "../Entity/Menu_Background.h"
#include "../Entity/Text_Box.h"
#include "../Entity/Text_Menu_object.h"
#include "../Entity/dialouge_box.h"

#include "../Events/Gui_Events.h"



class GUI_Layer : public Layer
{
public:
	enum class Type
	{
		Overlay,
		BuildingSelectMenu,
		PublicBuildingSubMenu,
		PublicTransportSubMenu,
		SetupMenu
	};

	//const bool dialog_box = &m_dialog_box;
private:
	Type m_type;
	std::vector<scriptable_object*> m_objects;

	std::unordered_map<std::string, unsigned int> m_textures;

	unsigned int m_base_layer;

	
	//bool m_dialog_box = false;

	std::shared_ptr<Camera_Controller> m_orthographic_controller;

#pragma region settings_menu
	bool sm_saved;
#pragma endregion

#pragma region zone_menu
	unsigned int m_selected;
	scriptable_object* m_caller;
#pragma endregion;

public:
	GUI_Layer(Type menu_type, unsigned int base_layer, std::shared_ptr<Camera_Controller> ortho_controll);
	virtual ~GUI_Layer() override;

	virtual void On_Attach(std::vector<std::pair<std::string, std::string>> textures) override;
	virtual void On_Detach() override;
	virtual void On_Update(Timestep ts) override;
	virtual void On_ImGui_Render() override;
	virtual void On_Event(Events::Event& e) override;

	inline Type get_type() { return m_type; }

#pragma region zone_menu
	inline virtual void set_selected(unsigned int num) override { m_selected = num; }

	void set_caller(scriptable_object* caller);
	void change_box_colour();
	void change_box_texture();
#pragma endregion

private:
	void add_scriptable_object(scriptable_object* obj);
	
	void create_settings_menu(unsigned int menu);
	void create_building_menu();
	void create_public_building_sub_menu();
	void create_public_transport_sub_menu();

	std::vector<scriptable_object*>& get_objects() { return m_objects; }


#pragma region exit_functions

	void setting_exit_func();
	void save_exit_func();
	void save_exit_func_2();

	void close_menu();
#pragma endregion

#pragma region settings_pages
	void page_one();
	void page_two();
#pragma endregion

#pragma region sub_menus
	void open_public_sub();
	void open_transport_sub();
#pragma endregion

};

