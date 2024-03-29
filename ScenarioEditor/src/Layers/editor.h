#pragma once

#include "../Layer.h"
#include "../file.h"

#include "../Events/Gui_Events.h"

#include "../Entity/Button.h"

#include "../buildings_data.h"

class button_data
{
public:
	uint32_t button_id;

	int32_t building_type; //pow - 0, park - 1, ....., arena - 10, cba to write a enum for this given it will be passed into covid sim via a text file and so will be a integer anyway
	uint32_t size; //small - 0, medium - 1, large - 2, ''

	bool transport_building = false;
	std::vector<Transport_Type> types;

	bool education_building = false;
	SCHOOLS edu_type = SCHOOLS::NONE;

	uint32_t staff;
	uint32_t capacity;
	std::pair<uint32_t, uint32_t> opening_hours;

	bool staff_action_needed = false;
	bool capacity_action_needed = false;
	bool opening_action_needed = false;
	bool action_needed = false;

	//rendering stuff
	bool render = true;
};

class editor : public Layer
{
private:
	std::vector<scriptable_object*> m_objects;
	std::unordered_map<uint32_t, std::shared_ptr<button_data>> m_world_data; 
	std::unordered_map<std::string, unsigned int> m_textures;

	unsigned int m_base_layer;
	unsigned int m_size_of_grid;
	
	unsigned int m_button_num;
	unsigned int m_selected;

	bool m_disable_non_transport_events; 
	bool m_cached_transport_overlay;

	std::vector<glm::vec4> m_transport_overlay = {};

	std::shared_ptr<Camera_Controller> m_orthographic_controller;
public:
	editor(unsigned int base_layer, std::shared_ptr<Camera_Controller> ortho_control);
	virtual ~editor() override;

	virtual void On_Attach(std::vector<std::pair<std::string, std::string>> textures) override;
	virtual void On_Detach() override;
	virtual void On_Update(Timestep ts) override;
	virtual void On_ImGui_Render() override;
	virtual void On_Event(Events::Event& e) override;
	
	
	inline unsigned int selected() { return m_selected; }
	inline virtual void set_selected(unsigned int num) override { m_selected = num; }
	inline void delete_transport_cache() { m_cached_transport_overlay = false; }

	inline std::shared_ptr<button_data> get_world_data(uint32_t key) { return m_world_data[key]; };
	inline std::unordered_map<uint32_t, std::shared_ptr<button_data>> get_world_data_list() { return m_world_data; }

	static editor* get();

	glm::vec4 get_grid();
	std::vector<glm::vec4> get_overlay();

	void only_transport(bool arg);
	inline bool get_only_transport() { return m_disable_non_transport_events; }
	void bind_transport_select(bool arg);
	void bind_transport_remove(bool arg);

	void only_education(bool arg);

	glm::vec2 get_position(int button_id) { return m_objects[button_id]->get_position(); }
 

private:
	void add_scriptable_object(scriptable_object* object);
	std::vector<scriptable_object*>& get_objects() { return m_objects; }

	void draw_buttons(int amount);

	void open_zone_selector();
	void open_drop_down();
	void stop_selected();
	void stop_removed();
	bool open_settings_panel(Events::Key_Pressed_Event& e);
	bool export_scenario(Events::Key_Pressed_Event& e);
};

