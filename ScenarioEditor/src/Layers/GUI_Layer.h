#pragma once
#include "../Layer.h"

#include "../Entity/Button.h"
#include "../Entity/Menu_Background.h"
#include "../Entity/Text_Box.h"
#include "../Entity/Text_Menu_object.h"

class GUI_Layer : public Layer
{
public:
	enum class Type
	{
		Overlay,
		BuildingSelectMenu,
		SetupMenu
	};
private:
	Type m_type;
	std::vector<scriptable_object*> m_objects;
	std::unordered_map<std::string, unsigned int> m_textures;

	unsigned int m_base_layer;

	std::shared_ptr<Camera_Controller> m_orthographic_controller;

public:
	GUI_Layer(Type menu_type, unsigned int base_layer, std::shared_ptr<Camera_Controller> ortho_controll);
	virtual ~GUI_Layer() override;

	virtual void On_Attach(std::vector<std::pair<std::string, std::string>> textures) override;
	virtual void On_Detach() override;
	virtual void On_Update(Timestep ts) override;
	virtual void On_ImGui_Render() override;
	virtual void On_Event(Events::Event& e) override;

private:
	void add_scriptable_object(scriptable_object* obj);
	void create_menu();

	std::vector<scriptable_object*>& get_objects() { return m_objects; }

};

