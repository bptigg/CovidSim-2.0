#pragma once

#include "Agent.h"
#include "Matrix.h"

#include <map>

class friendship
{
private:
	struct triangle
	{
		std::string m_target_agent;
		std::string m_link_agent;
		std::string m_final_agent;

		triangle(std::string m_t, std::string m_l, std::string m_f)
			:m_target_agent(m_t), m_link_agent(m_l), m_final_agent(m_f)
		{
		}
	};
	
	std::map<std::string, std::shared_ptr<Agent>> m_agents;
	std::map<int, std::string > m_agents_index_ref;
	std::map<std::string, int> m_agents_index;

	std::map<std::string, std::vector<std::unique_ptr<triangle>>> m_agents_triangles;

	std::shared_ptr<Matrix<double>> m_friendship_network;
private:
	void update_friendship_network();
	void identify_friendship_triangle();
};

