#include "Layer_Stack.h"

Layer_Stack::~Layer_Stack()
{
	for (Layer* layer : m_layers)
	{
		layer->On_Detach();
		delete layer;
	}
}

void Layer_Stack::Push_Layer(Layer* layer)
{
	m_layers.emplace(m_layers.begin() + m_layer_index, layer);
	m_layer_index++;
}

void Layer_Stack::Push_Overlay(Layer* layer)
{
	m_layers.emplace_back(layer);
}

void Layer_Stack::Pop_Layer(Layer* layer)
{
	auto it = std::find(m_layers.begin(), m_layers.begin() + m_layer_index, layer);
	{
		if (it != m_layers.begin() + m_layer_index)
		{
			layer->On_Detach();
			m_layers.erase(it);
			m_layer_index--;
		}
	}
}

void Layer_Stack::Pop_Overlay(Layer* layer)
{
	auto it = std::find(m_layers.begin() + m_layer_index, m_layers.end(), layer);
	if (it != m_layers.end())
	{
		layer->On_Detach();
		m_layers.erase(it);
	}
}
