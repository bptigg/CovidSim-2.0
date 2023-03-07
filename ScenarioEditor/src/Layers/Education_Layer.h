#pragma once

#include "../Layer.h"
#include "../Entity/scriptable_object.h"

class Education_Layer : public Layer
{
private:
	unsigned int m_base_layer;
	bool m_attached;
	
	std::vector<scriptable_object*> m_objects;
	std::unordered_map<std::string, unsigned int> m_textures;
	std::shared_ptr<Camera_Controller> m_orthographic_controller;
public:
	Education_Layer(unsigned int base_layer, std::shared_ptr<Camera_Controller> ortho_controll);
	virtual ~Education_Layer() override;

	virtual void On_Attach(std::vector<std::pair<std::string, std::string>> textures) override;
	virtual void On_Detach() override;
	virtual void On_Update(Timestep ts) override;	
	virtual void On_ImGui_Render() override;
	virtual void On_Event(Events::Event& event) override;

	inline bool get_attached() { return m_attached; }

	void enable_overlay();
	void disable_overlay();
};

