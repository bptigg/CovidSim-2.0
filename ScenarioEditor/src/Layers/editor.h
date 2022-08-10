#pragma once

#include "../Layer.h"
#include "../file.h"

#include "../Events/Gui_Events.h"

#include "../Entity/Button.h"

class button_data
{
public:
	uint32_t button_id;

	int32_t building_type; //pow - 0, park - 1, ....., arena - 10, cba to write a enum for this given it will be passed into covid sim via a text file and so will be a integer anyway
	uint32_t size; //small - 0, medium - 1, large - 2, ''

	uint32_t staff;
	uint32_t capacity;
	std::pair<uint32_t, uint32_t> opening_hours;


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

	std::shared_ptr<Camera_Controller> m_orthographic_controller;
public:
	editor(unsigned int base_layer, std::shared_ptr<Camera_Controller> ortho_control);
	virtual ~editor() override;

	virtual void On_Attach(std::vector<std::pair<std::string, std::string>> textures) override;
	virtual void On_Detach() override;
	virtual void On_Update(Timestep ts) override;
	virtual void On_ImGui_Render() override;
	virtual void On_Event(Events::Event& e) override;

	inline virtual void set_selected(unsigned int num) override { m_selected = num; }

	inline std::shared_ptr<button_data> get_world_data(uint32_t key) { return m_world_data[key]; };

private:
	void add_scriptable_object(scriptable_object* object);
	std::vector<scriptable_object*>& get_objects() { return m_objects; }

	void draw_buttons(int amount);

	void open_zone_selector();
	void open_drop_down();
};

