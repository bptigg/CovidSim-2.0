#include "friendship.h"

void friendship::update_friendship_network()
{
	std::map<std::pair<int, int>,int> updated;
	for (auto it = m_agents_index.begin(); it != m_agents_index.end(); it++)
	{
		int size = m_agents[it->first]->friends.size();
		for (int i = 0; i < size; i++)
		{
			std::string friend_id = m_agents[it->first]->friends[i];
			if (updated[std::make_pair(m_agents_index[friend_id],it->second)] == 0)
			{
				if ((*m_friendship_network)(it->second, m_agents_index[friend_id]) == 0)
				{
					(*m_friendship_network)(it->second, m_agents_index[friend_id]) = 1;
					(*m_friendship_network)(m_agents_index[friend_id], it->second) = 1;
					matrix_updated = true;
				}
				updated[std::make_pair(it->second, m_agents_index[friend_id])] = 1;
			}
		}
	}
	identify_friendship_triangle();
}

void friendship::make_friends(std::string agent_1, std::string agent_2)
{
	if (friend_check(m_agents[agent_1]->extrovert, m_agents[agent_2]->extrovert))
	{
		m_agents_triangles.erase(std::make_pair(agent_1, agent_2));
		
		bool agent_2_done = false;

		while (m_agents[agent_1]->edit == false)
		{
			if (m_agents[agent_2]->edit != false)
			{
				m_agents[agent_2]->modify_permission();
				m_agents[agent_2]->friends.push_back(agent_2);
				agent_2_done = true;
				m_agents[agent_2]->modify_permission();
			}
		}
		m_agents[agent_1]->modify_permission();
		m_agents[agent_1]->friends.push_back(agent_2);
		m_agents[agent_1]->modify_permission();

		if (agent_2_done != true)
		{
			while (m_agents[agent_2]->edit == false);
			m_agents[agent_2]->modify_permission();
			m_agents[agent_2]->friends.push_back(agent_2);
			m_agents[agent_2]->modify_permission();
		}
	}
}

void friendship::identify_friendship_triangle()
{
	m_skip = true;

	if (matrix_updated == true)
	{
		Matrix<double> network = (*m_friendship_network);
		Matrix<double> network_sq = network * network;

		int col_size = network.get_col_size();
		int row_size = network.get_row_size();

		for (int i = 0; i < col_size; i++)
		{
			for (int e = i; e < row_size; e++)
			{
				if (i != e)
				{
					std::pair<std::string, std::string> triad = std::make_pair(m_agents_index_ref[i], m_agents_index_ref[e]);
					if (m_agents_triangles[triad] == 0)
					{
						int amount = network_sq(i, e);
						if (amount > 0)
						{
							std::string target_a = m_agents_index_ref[i];
							std::string final_a = m_agents_index_ref[e];
							m_agents_triangles[triad] = std::make_shared<open_triad>(target_a, final_a, amount);
						}
						else
						{
							m_agents_triangles.erase(triad);
						}
					}
				}
			}
		}
	}
	matrix_updated = false;
	m_skip = false;
}


std::vector<std::string> friendship::get_friends(std::pair<std::string, std::string> key ,int limit)
{
	std::vector<std::string> friends;
	std::shared_ptr<open_triad> connections = m_agents_triangles[key];

	std::vector<int> random_sample = {};
	int amount = random::Random_number(0, (m_agents[connections->m_target_agent]->friends.size() < limit) ? m_agents[connections->m_target_agent]->friends.size() : CONSTANTS::MAX_INVITES, {} ,true);
	for (int i = 0; i < amount; i++)
	{
		random_sample.push_back(random::Random_number(0, m_agents[connections->m_target_agent]->friends.size(), random_sample, true));
		friends.push_back(m_agents[connections->m_target_agent]->friends[random_sample[i]]);
	}

	random_sample = {};
	amount = random::Random_number(0, (m_agents[connections->m_final_agent]->friends.size() < limit) ? m_agents[connections->m_final_agent]->friends.size() : CONSTANTS::MAX_INVITES, {}, true);
	for (int i = 0; i < amount; i++)
	{
		random_sample.push_back(random::Random_number(0, m_agents[connections->m_final_agent]->friends.size(), random_sample, true));
		friends.push_back(m_agents[connections->m_final_agent]->friends[random_sample[i]]);
	}
	
	return friends;
}

bool friendship::friend_check(double agent_1, double agent_2)
{
	double weight = (agent_1 + agent_2) / 2;
	return random::Discrete_distribution({ 1 - weight, weight }, true);
}

std::pair<std::pair<std::string, std::string>, std::vector<std::string>> friendship::get_viable_pairing(int min_value, std::pair<std::string, std::string> optional_key)
{
	std::pair<std::pair<std::string, std::string>, std::vector<std::string>> viable_pairing;
	
	if (optional_key.first == "")
	{
		bool pairing = false;
		int position = 0;
		std::vector<int> used = {};
		while (pairing == false)
		{
			position = random::Random_number(0, m_agents_triangles.size(), used, true);
			used.push_back(position);

			int i = 0;
			for (auto it = m_agents_triangles.begin(); it != m_agents_triangles.end(); it++)
			{
				if (i == position)
				{
					if (it->second->m_amount >= min_value)
					{
						pairing = true;
						viable_pairing = { it->first, get_friends(it->first) };
						break;
					}
					break;
				}
				i++;
			}
		}
	}
	else
	{
		viable_pairing = { optional_key, get_friends(optional_key) };
	}

	return viable_pairing;
}

friendship::friendship(std::vector<std::shared_ptr<Agent>> agent_vec)
{
	int i = 0;
	for (i = 0; i < agent_vec.size(); i++)
	{
		m_agents[agent_vec[i]->agent_id] = agent_vec[i];
		m_agents_index_ref[i] = agent_vec[i]->agent_id;
		m_agents_index[agent_vec[i]->agent_id] = i;
	}

	m_friendship_network = std::make_unique<Matrix<double>>(Matrix<double>(i, i));
}
