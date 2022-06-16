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
