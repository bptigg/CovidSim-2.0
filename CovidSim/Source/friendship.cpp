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


std::vector<std::string> friendship::get_friends(std::pair<std::string, std::string> key)
{
	std::vector<std::string> friends;
	std::shared_ptr<open_triad> connections = m_agents_triangles[key];

	std::vector<int> random_sample = {};
	int amount = random::Random_number(0, (m_agents[connections->m_target_agent]->friends.size() < CONSTANTS::MAX_INVITES) ? m_agents[connections->m_target_agent]->friends.size() : 10, {} ,true);
	for (int i = 0; i < amount; i++)
	{
		random_sample.push_back(random::Random_number(0, m_agents[connections->m_target_agent]->friends.size(), random_sample, true));
		friends.push_back(m_agents[connections->m_target_agent]->friends[random_sample[i]]);
	}

	random_sample = {};
	amount = random::Random_number(0, m_agents[connections->m_final_agent]->friends.size(), {}, true);
	for (int i = 0; i < amount; i++)
	{
		random_sample.push_back(random::Random_number(0, (m_agents[connections->m_final_agent]->friends.size() < CONSTANTS::MAX_INVITES) ? m_agents[connections->m_final_agent]->friends.size() : 10, random_sample, true));
		friends.push_back(m_agents[connections->m_final_agent]->friends[random_sample[i]]);
	}
	
	return friends;
}

std::map<std::pair<std::string, std::string>, std::vector<std::string>> friendship::get_viable_pairing(int min_value)
{
	std::map<std::pair<std::string, std::string>, std::vector<std::string>> viable_pairing;
	
	for (auto it = m_agents_triangles.begin(); it != m_agents_triangles.end(); it++)
	{
		if (it->second->m_amount >= min_value)
		{
			viable_pairing[it->first] = get_friends(it->first);
		}
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
