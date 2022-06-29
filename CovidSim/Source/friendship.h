#pragma once

#include "Agent.h"
#include "Matrix.h"
#include "Random.h"
#include "Constants.h"

#include <map>
#include <unordered_map>


class friendship
{
private:
	struct open_triad
	{
		std::string m_target_agent;
		std::string m_final_agent;

		int m_amount;

		open_triad(std::string m_t, std::string m_f, int amount)
			:m_target_agent(m_t), m_final_agent(m_f), m_amount(amount)
		{
		}
	};
	

	std::unordered_map<std::string, std::shared_ptr<Agent>> m_agents;
	std::unordered_map<int, std::string > m_agents_index_ref;
	std::unordered_map<std::string, int> m_agents_index;

	std::map<std::pair<std::string,std::string>, std::shared_ptr<open_triad>> m_agents_triangles;

	std::unique_ptr<Matrix<double>> m_friendship_network;
	std::unique_ptr<Matrix<double>> m_network_squared;

	bool m_skip = false;
	bool matrix_updated = false;
public:
	const bool& skip = m_skip;
	std::unordered_map<std::string, std::shared_ptr<Agent>>& agents = m_agents;
private:
	void identify_friendship_triangle();
	std::vector<std::string> get_friends(std::pair<std::string, std::string> key, int limit = CONSTANTS::MAX_INVITES);
	bool friend_check(double agent_1, double agent_2);
public:
	std::pair<std::pair<std::string, std::string>, std::vector<std::string>> get_viable_pairing(int min_value, std::pair<std::string, std::string> optional_key = {"",""});
	void update_friendship_network();

	void make_friends(std::string agent_1, std::string agent_2);

	friendship(std::vector<std::shared_ptr<Agent>> agent_vec);



};

