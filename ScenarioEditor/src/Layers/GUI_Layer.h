#pragma once
#include "../Layer.h"

#include "../Entity/scriptable_object.h"

class GUI_Layer : public Layer
{
public:
	enum class Type
	{
		Overlay,
		BuildingSelectMenu
	};
private:
	Type m_type;
	std::vector<scriptable_object*> m_objects;
	std::unordered_map<std::string, unsigned int> m_textures;
private:
	GUI_Layer(Type menu_type);
	virtual ~GUI_Layer() override;

	virtual void On_Attach(std::vector<std::pair<std::string, std::string>> textures) override;
	virtual void On_Detach() override;
	virtual void On_Update(Timestep ts) override;
	virtual void On_ImGui_Render() override;
	virtual void On_Event(Events::Event& e) override;

	void add_scriptable_object(scriptable_object* obj);

	std::vector<scriptable_object*>& get_objects() { return m_objects; }

};

