#pragma once

#include "Agent.h"
#include "Matrix.h"
#include "Random.h"

#include <map>

class friendship
{
private:
	struct triangle
	{
		std::string m_target_agent;
		std::string m_link_agent;
		std::string m_final_agent;

		double p_strength;

		triangle(std::string m_t, std::string m_l, std::string m_f, int tl, int lf)
			:m_target_agent(m_t), m_link_agent(m_l), m_final_agent(m_f)
		{
			p_strength = (tl + lf) * 0.5;

		}
	};
	
	std::map<std::string, std::shared_ptr<Agent>> m_agents;
	std::map<int, std::string > m_agents_index_ref;
	std::map<std::string, int> m_agents_index;

	std::map<std::pair<std::string,std::string>, std::vector<std::shared_ptr<triangle>>> m_agents_triangles;
	std::map<std::pair<std::string, std::string>, double> m_triangle_confidence;
	std::map<int, int> triangles;

	std::shared_ptr<Matrix<double>> m_friendship_network;

	bool m_skip = false;
public:
	const bool& skip = m_skip;
private:
	void update_friendship_network();
	void identify_friendship_triangle();
	double get_bond_strength(); //will change at somepoint
	void get_confidence_value();
	std::vector<std::string> get_friends(std::pair<std::string, std::string> key);

public:
	std::map<std::pair<std::string, std::string>, std::vector<std::string>> get_viable_pairing(double min_value);

};

