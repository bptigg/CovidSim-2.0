#include "friendship.h"

void friendship::update_friendship_network()
{
	for (auto it = m_agents_index.begin(); it != m_agents_index.end(); it++)
	{
		for (int i = 0; i < m_agents[it->first]->friends.size(); i++)
		{
			std::string friend_id = m_agents[it->first]->friends[i];
			if ((*m_friendship_network)(it->second, m_agents_index[friend_id]) == 0)
			{
				double strength = get_bond_strength();
				(*m_friendship_network)(it->second, m_agents_index[friend_id]) = strength;
				(*m_friendship_network)(m_agents_index[friend_id], it->second) = strength;
			}
		}
	}
}

void friendship::identify_friendship_triangle()
{
	m_skip = true;
	for (int i = 0; i < m_friendship_network->get_col_size(); i++)
	{
		for (int e = 0; e < m_friendship_network->get_row_size(); e++)
		{
			if ((*m_friendship_network)(i, e) == 0);
			{
				for (int a = 0; a < m_friendship_network->get_row_size(); a++)
				{
					if ((*m_friendship_network)(e, a) != 0)
					{
						if (i == a || triangles[a] == i || triangles[i] == a)
						{
							continue;
						}
						else
						{
							m_agents_triangles[std::make_pair(m_agents_index_ref[i],m_agents_index_ref[a])].push_back(std::make_shared<triangle>(m_agents_index_ref[i], m_agents_index_ref[e], m_agents_index_ref[a], (*m_friendship_network)(i, e), (*m_friendship_network)(e, a)));
							triangles[i] = a;
						}
					}
				}
			}
		}
	}
	get_confidence_value();
	m_skip = false;
}

double friendship::get_bond_strength()
{
	return ((double)random::Random_number(0, 100) * 0.01);
}

void friendship::get_confidence_value()
{
	for (auto it = m_agents_triangles.begin(); it != m_agents_triangles.end(); it++)
	{
		int size = it->second.size();
		double confidence_value = 0;
		for (int i = 0; i < size; i++)
		{
			confidence_value = confidence_value + it->second[i]->p_strength;
		}
		m_triangle_confidence[it->first] = confidence_value;
	}
}

std::vector<std::string> friendship::get_friends(std::pair<std::string, std::string> key)
{
	std::vector<std::string> friends;
	std::vector<std::shared_ptr<triangle>> connections = m_agents_triangles[key];

	for (int i = 0; i < connections.size(); i++)
	{
		friends.push_back(connections[i]->m_link_agent);
	}
	
	return friends;
}

std::map<std::pair<std::string, std::string>, std::vector<std::string>> friendship::get_viable_pairing(double min_value)
{
	std::map<std::pair<std::string, std::string>, std::vector<std::string>> viable_pairing;
	
	for (auto it = m_triangle_confidence.begin(); it != m_triangle_confidence.end(); it++)
	{
		if (it->second >= min_value)
		{
			viable_pairing[it->first] = get_friends(it->first);
		}
	}

	return viable_pairing;
}
