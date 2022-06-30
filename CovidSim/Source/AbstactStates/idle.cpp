#include "idle.h"

idle::idle(std::string owner)
{
	m_type = FINITE_STATE_MACHINE::FSM_state_type::IDLE;
	m_agent = owner;
}

bool idle::enter_state()
{
	abstract_state::enter_state();
	if (m_entered_state == false)
	{
		return m_entered_state;
	}
	m_idle_count = 0;
	m_idle_time = random::Normal_distribution(FINITE_STATE_MACHINE::AVG_IDLE_TIME, FINITE_STATE_MACHINE::IDLE_SD);
	return m_entered_state;
}

bool idle::exit_state()
{
	abstract_state::exit_state();
	return true;
}

FINITE_STATE_MACHINE::FSM_state_type idle::update_state()
{
	m_idle_count++;
	if (m_idle_count >= m_idle_time)
	{
		return FINITE_STATE_MACHINE::FSM_state_type::AWAITING_TASK;
	}
	return FINITE_STATE_MACHINE::FSM_state_type::NONE;
}
