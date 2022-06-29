#include "idle.h"

idle::idle()
{
	m_type = FINITE_STATE_MACHINE::FSM_state_type::IDLE;
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

void idle::update_state()
{
	m_idle_count++;
	if (m_idle_count >= m_idle_time)
	{
		fsm->EnterState(FINITE_STATE_MACHINE::FSM_state_type::AWAITING_TASK);
	}
}
