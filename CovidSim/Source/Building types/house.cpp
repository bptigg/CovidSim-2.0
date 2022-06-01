#include "house.h"

house::house(std::vector<std::shared_ptr<Agent>>& family_group)
{
	m_capacity = -1;

	for (int i = 0; i < family_group.size(); i++)
	{
		if (find(family_group[i], m_family_group).first == true)
		{
			Log::warning("AGENT ALREADY FAMILY MEMBER");
		}
		else
		{
			m_family_lock.lock();
			m_family_group.push_back(std::move(family_group[i]));
			m_family_lock.unlock();
		}
	}
}

house::~house()
{
	m_family_group.clear();
}

std::vector<std::shared_ptr<Agent>> house::get_family_group()
{
	return m_family_group;
}
