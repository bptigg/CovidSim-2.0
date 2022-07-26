#pragma once

#include <glm/glm.hpp>
#include <functional>

#include "../Layer.h"
#include "../Events/Event.h"

#define BIND_BUTTON_FN(x) std::bind(&x, this)

enum class entity_type
{
	BACKGROUND = 0,
	BUTTON,
	TEXT_MENU_OBJECT,
	TEXT_BOX,
	DIALOUGE_BOX,
	NONE
};

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

	entity_type m_type = entity_type::NONE;
	int m_object_id;

	float m_zoom;
	glm::vec2 m_camera_position;
	glm::mat4 m_camera_matrix;

	bool m_menu_object;
	bool m_set_up;

	bool m_delete_object;
	bool m_set_inactive;

public:
	scriptable_object(const glm::vec2& location, const glm::vec2& size, Layer* layer, int id);
	virtual ~scriptable_object();

	virtual void render() = 0;
	virtual void update() = 0;
	virtual void event_callback(Events::Event& e) = 0;

	virtual void update_position(const float& zoom, const glm::vec2& camera_pos, const glm::mat4& camera_mat) = 0;
	
	inline void delete_obj(bool obj) { m_delete_object = obj; }
	inline void inactive_obj(bool obj) { m_set_inactive = obj; }

	int get_id() { return m_object_id; }

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

	glm::vec2 get_size() {
		return m_size;
	}
	
	const glm::vec2 get_size() const {
		return m_size;
	}

	const entity_type get_type() const {
		return m_type;
	}

	entity_type get_type() {
		return m_type;
	}
public:
	const bool& delete_object = m_delete_object;
	const bool& set_inactive = m_set_inactive;
};

