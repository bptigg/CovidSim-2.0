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
				(*m_friendship_network)(it->second, m_agents_index[friend_id]) = 1;
				(*m_friendship_network)(m_agents_index[friend_id], it->second) = 1;
			}
		}
	}
}

void friendship::identify_friendship_triangle()
{
	for (int i = 0; i < m_friendship_network->get_col_size(); i++)
	{
		for (int e = 0; e < m_friendship_network->get_row_size(); i++)
		{
			if ((*m_friendship_network)(e, i) != 1);
			{
				for (int a = 0; a < m_agents[m_agents_index_ref[e]]->friends.size(); a++)
				{
					std::string friend_agent = m_agents[m_agents_index_ref[e]]->friends[a];
					if (friend_agent == m_agents_index_ref[i])
					{
						continue;
					}
					else
					{
						m_agents_triangles[m_agents_index_ref[i]].push_back(std::make_unique<triangle>(m_agents_index_ref[i], m_agents_index_ref[e], friend_agent));
					}
				}
			}
		}
	}
}
