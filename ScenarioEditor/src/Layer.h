#pragma once

#include "Timestep.h"
#include "Events/Event.h"

class Layer
{
public:
	Layer(const std::string& name = "Layer")
		:m_debug_name(name)
	{
	}

	virtual ~Layer() = default;

	virtual void On_Attach() = 0;
	virtual void On_Detach() = 0;
	virtual void On_Update(Timestep ts) = 0;
	virtual void On_ImGui_Render() = 0;
	virtual void On_Event(Events::Event& event) = 0;

	const std::string& GetName() const { return m_debug_name; }
protected:
	std::string m_debug_name;

};

