#pragma once

#include "Timestep.h"
#include "Events/Event.h"
#include "Camera/Camera_Controller.h"
#include "Texture.h"


#include "Renderer.h"

#include <unordered_map>
#include <vector>

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

enum class Transport_Type
{
	BUS = 0,
	LIGHT_RAIL,
	RAPID_TRANSIT,
	TRAINS,
	NONE
};

class Layer
{
public:
	using Event_Call_back_fn = std::function<void(Events::Event&)>;

public:
	Layer(const std::string& name = "Layer")
		:m_debug_name(name)
	{
		m_dialog_box = false;
		m_text_box = false;
		m_render = true;
		m_delete_layer = false;
	}

	virtual ~Layer() = default;

	virtual void On_Attach(std::vector<std::pair<std::string, std::string>> textures) = 0;
	virtual void On_Detach() = 0;
	virtual void On_Update(Timestep ts) = 0;
	virtual void On_ImGui_Render() = 0;
	virtual void On_Event(Events::Event& event) = 0;

	inline virtual void set_selected(unsigned int num)
	{
	}

	inline virtual unsigned int get_selected()
	{
		return 0;
	}

	inline void Set_Event_Callback(const Event_Call_back_fn& callback) { Event_Call_back = callback; }
	inline virtual void Set_Text_Box(bool box) { m_text_box = box;}


	const std::string& GetName() const { return m_debug_name; }

	virtual void render(bool render) { m_render = render; }
	virtual void change_line_colour() {}
protected:
	Event_Call_back_fn Event_Call_back;
#if _DEBUG
	std::string m_debug_name;
#endif

	bool m_dialog_box = false;
	bool m_text_box = false;
	bool m_render = true;
	bool m_delete_layer = false;
	bool m_attached = false;

public:
	const bool& dialog_box = m_dialog_box;
	const bool& delete_layer = m_delete_layer;
	const bool& text_box = m_text_box;
};

