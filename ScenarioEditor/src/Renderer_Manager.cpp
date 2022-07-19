#include "Renderer_Manager.h"

Renderer_Manager::Renderer_Manager()
{
	m_queue = {};
	finished = true;
	m_map_position = 0;
	m_max_positions = 0;
}

Renderer_Manager::~Renderer_Manager()
{
	delete_queue();
}

void Renderer_Manager::draw_rectangle_texture(const glm::vec2& position, const glm::vec2& size, const unsigned int texture_id)
{
	finished = false;
	render_queue_object* object = new render_queue_object(render_type::TEXTURED_RECTANGLE, position, size);

	object->texture_id = texture_id;

	Push_Object(object);
}

void Renderer_Manager::draw_rectangle_color(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
{
	finished = false;
	render_queue_object* object = new render_queue_object(render_type::COLOURED_RECTANGLE, position, size);

	object->color = color;

	Push_Object(object);
}

void Renderer_Manager::draw_box(const glm::vec2& centre, const glm::vec2& size, const float border_width, const glm::vec4& color)
{
	finished = false;
	render_queue_object* object = new render_queue_object(render_type::COLOURED_BOX, centre, size);

	object->color = color;
	object->border_width = border_width;

	Push_Object(object);
}

std::unordered_map<unsigned int, std::vector<render_queue_object*>>::iterator Renderer_Manager::next_draw()
{
	auto it = m_queue.begin();
	for (int i = 0; it != m_queue.end(); it++)
	{
		if (i == m_map_position)
		{
			m_map_position++;
			break;
		}
		i++;
	}

	if (m_map_position == m_max_positions)
	{
		finished = true;
	}

	return it;
}

void Renderer_Manager::delete_queue()
{
	for (auto it = m_queue.begin(); it != m_queue.end(); it++)
	{
		for (render_queue_object* object : it->second)
		{
			//it->second.erase(it->second.begin());
			delete object;
			//index++;
		}
		it->second.clear();
	}
	m_map_position = 0;

}

void Renderer_Manager::Push_Object(render_queue_object* object)
{
	if (m_queue.find(object->texture_id) == m_queue.end())
	{
		m_max_positions++;
	}
	m_queue[object->texture_id].push_back(object);
}

void Renderer_Manager::Pop_Object(render_queue_object* object)
{
	if (!m_queue[object->texture_id].empty())
	{
		auto it = std::find(m_queue[object->texture_id].begin(), m_queue[object->texture_id].end(), object);
		
			if(it != m_queue[object->texture_id].end())
			{
				m_queue[object->texture_id].erase(it);
			}
	}
	else
	{
		m_queue.erase(object->texture_id);
	}
}
