#pragma once

#include "Agent.h"
#include "Matrix.h"

#include <map>

class friendship
{
private:
	std::map<std::string, std::shared_ptr<Agent>> m_agents;
	std::map<std::string, int> m_agents_index;
	std::shared_ptr<Matrix<double>> m_friendship_network;
private:
	void update_friendship_network();
	void identify_friendship_triangle();
};

