#include "awaiting_task.h"

awaiting_task::awaiting_task(std::string owner, std::shared_ptr<task_builder> director)
{
	m_type = FINITE_STATE_MACHINE::FSM_state_type::IDLE;
	m_agent = owner;
	m_task_director = std::move(director);
}

bool awaiting_task::enter_state()
{
	abstract_state::enter_state();
	bool task_present = (m_task_director != nullptr);
	
	m_entered_state = m_entered_state && task_present;
	if (m_entered_state == false)
	{
		return m_entered_state;
	}
	
	m_chances = 0;
	m_task_chance = (double)random::Random_number(0, 100, {}, false) * 0.01;
	m_group_task = (bool)random::Discrete_distribution({ 1 - TASK_CONSTANTS::GROUP_TASK, TASK_CONSTANTS::GROUP_TASK }, true);
	return m_entered_state;
}

bool awaiting_task::exit_state()
{
	abstract_state::exit_state();
	return true;
}

FINITE_STATE_MACHINE::FSM_state_type awaiting_task::update_state()
{
	if (random::Discrete_distribution({ 1 - m_task_chance, m_task_chance }, true) == 0)
	{
		m_task_chance = (m_task_chance >= 0.95) ? 1.00 : m_task_chance + 0.05;
	}
	else
	{
		std::vector<std::string> agents_list = {};
		agents_list.push_back(m_agent);
		bool task_allocated = false;
		if (m_group_task)
		{
			int size_of_group = m_task_director->friendship_director->agents[m_agent]->friends.size();
			int friends = random::Random_number(1, (CONSTANTS::MAX_FRIENDS > size_of_group) ? size_of_group : CONSTANTS::MAX_FRIENDS, {}, true);

			std::vector<int> used_numbers = {};
			for (int i = 0; i < friends; i++)
			{
				int result = random::Random_number(0, size_of_group, used_numbers, true);
				used_numbers.push_back(result);
				agents_list.push_back(m_task_director->friendship_director->agents[m_agent]->friends[result]);
			}
		}
		task_allocated = m_task_director->request_task(agents_list);
		if (task_allocated == false)
		{
			m_chances++;
			if (m_chances == TASK_CONSTANTS::TASK_RECURSION_LIMIT)
			{
				return FINITE_STATE_MACHINE::FSM_state_type::IDLE;
			}
		}
		else
		{
			return FINITE_STATE_MACHINE::FSM_state_type::TRANSIT;
		}
	}
	return FINITE_STATE_MACHINE::FSM_state_type::NONE;
}
