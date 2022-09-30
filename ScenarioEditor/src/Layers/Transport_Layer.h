#pragma once
#include "../Layer.h"

#include "../Entity/scriptable_object.h"
#include "../Entity/Scrollable_Menu.h"


struct Line
{
	std::vector<uint32_t> stops = {};
	std::vector<glm::vec2> positions = {};
	Transport_Type determined_type = Transport_Type::NONE;
	std::vector<Transport_Type>type = {};

	std::string name = "";
	glm::vec4 colour = { 0.0f, 0.0f, 0.0f, 1.0f };

	bool render = false;
	bool changed = false;
};

enum class MODE
{
	ADD = 0,
	REMOVE,
	DEFAULT
};

class Transport_Layer : public Layer
{
public:
	MODE m_mode = MODE::DEFAULT;
private:
	std::vector<scriptable_object*> m_objects;
	std::unordered_map<std::string, unsigned int> m_textures;
	std::vector<glm::vec4> m_overlay;
	std::unordered_map<std::string, std::vector<std::pair <glm::vec4, glm::vec4>>> m_line_overlay;

	unsigned int m_base_layer;

	unsigned int m_button_num;
	unsigned int m_selected;

	std::shared_ptr<Camera_Controller> m_orthographic_controller;
	std::unordered_map<std::string, std::shared_ptr<Line>> m_lines;
	std::string m_active_line;
public:
	Transport_Layer(unsigned int base_layer, std::shared_ptr<Camera_Controller> ortho_controll);
	virtual ~Transport_Layer() override;

	virtual void On_Attach(std::vector<std::pair<std::string, std::string>> textures) override;
	virtual void On_Detach() override;
	virtual void On_Update(Timestep ts) override;
	virtual void On_ImGui_Render() override;
	virtual void On_Event(Events::Event& e) override;

	void disable_overlay();
	void enable_overlay();

	std::shared_ptr<Line> update_line(std::string key);
	const std::unordered_map<std::string, std::shared_ptr<Line>> get_lines() const { return m_lines; }
	std::shared_ptr<Line> get_line(std::string key) { return m_lines[key];}

	inline bool get_attached() { return m_attached; }

	static Transport_Layer* get();
	const std::string get_active_line() const { return m_active_line; }
	void set_active_line(std::string line) { m_active_line = line; }

private:
	bool open_line_manager(Events::Key_Pressed_Event& e);
	void open_line_editor(std::string key);

	void draw_lines();
};

//Idea for this layer is to highlight all the transport all the transport zones and disable clicking on none transport zones
//Should have a menu that allows you to create lines, modify lines or delete lines
//will have a line editor panel that allows you to view the line and remove stations from the line