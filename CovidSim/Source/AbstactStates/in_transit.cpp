#include "in_transit.h"

in_transit::in_transit(std::string owner, std::shared_ptr<Movement> m_movement_director)
{
	m_type = FINITE_STATE_MACHINE::FSM_state_type::TRANSIT;
	m_agent = owner;

	m_movement_director = std::move(m_movement_director);
}

bool in_transit::enter_state()
{
	abstract_state::enter_state();
	
	bool movement_director_present = !(m_movement_director == nullptr);
	bool task_director_present = !(m_task_director == nullptr);

	if (movement_director_present == false)
	{
		Log::warning("NO MOVEMENT COMPONENT PRESENT");
	}

	if (task_director_present == false)
	{
		Log::warning("NO TASK COMPONENT PRESENT");
	}

	if (m_entered_state && movement_director_present == false);
	{
		return m_entered_state && movement_director_present;
	}

	m_path_to_destination = false;
	m_at_destination = false;
	m_using_a_star = false;
	return m_entered_state && movement_director_present;
}

bool in_transit::exit_state()
{
	abstract_state::exit_state();
	return true;
}

FINITE_STATE_MACHINE::FSM_state_type in_transit::update_state()
{
	int result = 0;
	if (m_valid_path == false)
	{
		result = m_movement_director->update_agent(m_task_director->friendship_director->agents[m_agent], true, true);
		if (result == 1 || result == 2)
		{
			m_arrived_at_start = false;
			m_using_a_star = false;
			m_at_destination = false;
			m_path_to_destination = false;
		}

		if (result == 1)
		{
			m_path_to_destination = true;
		}
	}
	else if (m_valid_path == true && m_arrived_at_start == false)
	{
		m_movement_director->update_agent(m_task_director->friendship_director->agents[m_agent],false,false,true,false);
		if (result == 3)
		{
			m_arrived_at_start = true;
		}

	}
	else if (m_valid_path == true && m_arrived_at_start == true)
	{
		result = m_movement_director->update_agent(m_task_director->friendship_director->agents[m_agent], true, false, false, false);
		if (result == 4 && m_path_to_destination == true)
		{
			m_at_end = true;
		}
		else if (result == 4 && m_path_to_destination == false)
		{
			m_valid_path == false;
		}
	}
	else if (m_at_end == true)
	{
		result = m_movement_director->update_agent(m_task_director->friendship_director->agents[m_agent], false ,false, false, true);
		if (result == 3)
		{
			//need to add to location 
			return FINITE_STATE_MACHINE::FSM_state_type::ACTIVE;
		}
	}
	return FINITE_STATE_MACHINE::FSM_state_type::NONE;

}
