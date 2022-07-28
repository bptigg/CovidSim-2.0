#pragma once

#include "../Layer.h"
#include "../file.h"

#include "../Events/Gui_Events.h"

#include "../Entity/Button.h"

class editor : public Layer
{
private:
	std::vector<scriptable_object*> m_objects;
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

private:
	void add_scriptable_object(scriptable_object* object);
	std::vector<scriptable_object*>& get_objects() { return m_objects; }

	void draw_buttons(int amount);

	void open_zone_selector();
};

