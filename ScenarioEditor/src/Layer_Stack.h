#pragma once

#include "Layer.h"

#include <vector>

class Layer_Stack
{
public:
	Layer_Stack() = default;
	~Layer_Stack();

	void Push_Layer(Layer* layer);
	void Push_Overlay(Layer* layer);
	void Pop_Layer(Layer* layer);
	void Pop_Overlay(Layer* layer);

	std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
	std::vector<Layer*>::iterator end() { return m_layers.end(); }
	std::vector<Layer*>::reverse_iterator r_begin() { return m_layers.rbegin(); }
	std::vector<Layer*>::reverse_iterator rend() { return m_layers.rend(); }

	std::vector<Layer*>::const_iterator begin() const { return m_layers.begin(); }
	std::vector<Layer*>::const_iterator end() const { return m_layers.end(); }
	std::vector<Layer*>::const_reverse_iterator r_begin() const { return m_layers.rbegin(); }
	std::vector<Layer*>::const_reverse_iterator rend() const { return m_layers.rend(); }

	Layer* operator[](unsigned int index)
	{
		return m_layers[index];
	}

private:
	std::vector<Layer*> m_layers;
	unsigned int m_layer_index = 0;
};

