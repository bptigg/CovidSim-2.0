#pragma once

#include <glm/glm.hpp>
#include <functional>

#include "../Layer.h"
#include "../Events/Event.h"

#define BIND_BUTTON_FN(x) std::bind(&x, this)

class scriptable_object
{
protected:
	glm::vec2 m_location;
	glm::vec2 m_size;

	struct Collision_Box
	{
		glm::vec2 lower_bound;
		glm::vec2 upper_bound;
		float width;
	};

	Layer* m_Layer;
	Collision_Box m_collison_box;

public:
	scriptable_object(const glm::vec2& location, const glm::vec2& size, Layer* layer);
	virtual ~scriptable_object();

	virtual void render() = 0;
	virtual void update() = 0;
	virtual void event_callback(Events::Event& e) = 0;

	glm::vec2 get_position() {
		return m_location;
	}

	const glm::vec2 get_position() const {
		return m_location;
	}

	Layer* get_layer() {
		return m_Layer; 
	}

	const Layer* get_layer() const {
		return m_Layer;
	}

};

