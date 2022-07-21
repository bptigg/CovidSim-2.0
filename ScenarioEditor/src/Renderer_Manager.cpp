#include "Renderer_Manager.h"

Renderer_Manager::Renderer_Manager()
{
	m_layer_queue = {};
	finished = true;
	m_map_position = 0;
	m_max_positions = {};
}

Renderer_Manager::~Renderer_Manager()
{
	delete_queue();
}

void Renderer_Manager::draw_rectangle_texture(const glm::vec2& position, const glm::vec2& size, const unsigned int texture_id, unsigned int layer)
{
	finished = false;
	render_queue_object* object = new render_queue_object(render_type::TEXTURED_RECTANGLE, position, size);

	object->texture_id = texture_id;
	object->layer = layer;

	Push_Object(object);
}

void Renderer_Manager::draw_rectangle_color(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, unsigned int layer)
{
	finished = false;
	render_queue_object* object = new render_queue_object(render_type::COLOURED_RECTANGLE, position, size);

	object->color = color;
	object->layer = layer;

	Push_Object(object);
}

void Renderer_Manager::draw_box(const glm::vec2& centre, const glm::vec2& size, const float border_width, const glm::vec4& color, unsigned int layer)
{
	finished = false;
	render_queue_object* object = new render_queue_object(render_type::COLOURED_BOX, centre, size);

	object->color = color;
	object->border_width = border_width;
	object->layer = layer;

	Push_Object(object);
}

void Renderer_Manager::draw_text(std::string& text, const glm::vec2& centre, const glm::vec2& size, const glm::vec4& color, unsigned int layer, float scale, bool centred)
{
	finished = false;
	render_queue_object* object = new render_queue_object(render_type::TEXT, centre, size);

	object->color = color;
	object->layer = layer;
	object->text = text;
	object->scale = scale;
	object->centred = centred;

	Push_Object(object);
}

std::unordered_map<unsigned int, std::vector<render_queue_object*>>::iterator Renderer_Manager::next_draw()
{
	auto it = m_layer_queue.begin();
	std::unordered_map<unsigned int, std::vector<render_queue_object*>>::iterator it_2;
	
	for (int i = 0; it != m_layer_queue.end(); it++)
	{
		if (i == m_layer_position)
		{
			empty = false;
			next_layer = false;
			//m_layer_position++;

			if (it->second.empty())
			{
				m_layer_position++;
				m_map_position = 0;
				empty = true;
				continue;
			}

			it_2 = it->second.begin();
			for (int a = 0; it_2 != it->second.end(); it++)
			{
				if (a == m_map_position)
				{
					m_map_position++;
					break;
				}
				a++;
			}

			if (m_map_position == m_max_positions[it->first])
			{
				m_layer_position++;
				m_map_position = 0;
				next_layer = true;
			}

			break;
		}
		i++;
	}

	if (m_layer_position == m_max_layers)
	{
		finished = true;
	}

	return it_2;
}

void Renderer_Manager::delete_queue()
{
	for (auto it = m_layer_queue.begin(); it != m_layer_queue.end(); it++)
	{
		for (auto it_2 = it->second.begin(); it_2 != it->second.end(); it_2++)
		{
			for (render_queue_object* object : it_2->second)
			{
				//it->second.erase(it->second.begin());
				delete object;
				//index++;
			}
			it_2->second.clear();
		}
		it->second.clear();

		m_max_positions[it->first] = 0;
	}
	m_map_position = 0;
	m_layer_position = 0;

}

void Renderer_Manager::Push_Object(render_queue_object* object)
{
	if (m_layer_queue.find(object->layer) == m_layer_queue.end())
	{
		m_max_layers++;
		m_max_positions[object->layer] = 0;
	}

	if (m_layer_queue[object->layer].find(object->texture_id) == m_layer_queue[object->layer].end())
	{
		m_max_positions[object->layer]++;
	}

	m_layer_queue[object->layer][object->texture_id].push_back(object);
}

void Renderer_Manager::Pop_Object(render_queue_object* object)
{
	if (!m_layer_queue[object->layer][object->texture_id].empty())
	{
		auto it = std::find(m_layer_queue[object->layer][object->texture_id].begin(), m_layer_queue[object->layer][object->texture_id].end(), object);
		
			if(it != m_layer_queue[object->layer][object->texture_id].end())
			{
				m_layer_queue[object->layer][object->texture_id].erase(it);
			}
	}
	else
	{
		m_layer_queue[object->layer].erase(object->texture_id);
	}
}
