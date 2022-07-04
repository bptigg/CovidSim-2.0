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

	if (movement_director_present == false)
	{
		Log::warning("NO MOVEMENT COMPONENT PRESENT");
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
	return FINITE_STATE_MACHINE::FSM_state_type::NONE;
}
