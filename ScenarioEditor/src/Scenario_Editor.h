#pragma once
#include "Layer.h"


#include "Entity/Button.h"
#include "Entity/Menu_Background.h"
#include "Entity/Text_Menu_object.h"

#include<memory>
#include<vector>

#include "Events/Key_Event.h"

class Scenario_Editor : public Layer
{
public:
	Scenario_Editor();
	virtual ~Scenario_Editor() = default;

	virtual void On_Attach(std::vector<std::pair<std::string, std::string>> textures) override;
	virtual void On_Detach() override;

	void On_Update(Timestep ts) override;
	virtual void On_ImGui_Render() override;
	void On_Event(Events::Event& e) override;

	bool On_Key_Pressed(Events::Key_Pressed_Event& e);
	bool On_Mouse_Press(Events::Mouse_Moved_Event& e);

private:

	Camera_Controller m_orthographic_controller;
	std::unordered_map<std::string, unsigned int> m_textures;
	bool present = false;

	std::vector<std::unique_ptr<Button>> buttons;

private:

	bool colision(glm::vec4& pos, glm::vec2& box_pos, float size);
	void test();

	
};

