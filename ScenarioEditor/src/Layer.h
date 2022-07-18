#pragma once

#include "Timestep.h"
#include "Events/Event.h"
#include "Camera/Camera_Controller.h"
#include "Texture.h"

#include "Renderer.h"

#include <unordered_map>
#include <vector>

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

class Layer
{
public:
	Layer(const std::string& name = "Layer")
		:m_debug_name(name)
	{
	}

	virtual ~Layer() = default;

	virtual void On_Attach(std::vector<std::pair<std::string, std::string>> textures) = 0;
	virtual void On_Detach() = 0;
	virtual void On_Update(Timestep ts) = 0;
	virtual void On_ImGui_Render() = 0;
	virtual void On_Event(Events::Event& event) = 0;

	const std::string& GetName() const { return m_debug_name; }
protected:
#if _DEBUG
	std::string m_debug_name;
#endif

};

