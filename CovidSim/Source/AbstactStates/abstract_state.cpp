#include "abstract_state.h"

abstract_state::abstract_state()
{
	m_state = FINITE_STATE_MACHINE::execution_state::NONE;
}

void abstract_state::attach_fsm(std::shared_ptr<finite_state_machine> state_machine)
{
	fsm = std::move(state_machine);
}

bool abstract_state::enter_state()
{
	m_entered_state = false;
	bool fsm_present = false;

	fsm_present = (fsm != nullptr);
	if (fsm_present == false)
	{
		Log::error("NO FSM FOUND", __FILE__, __LINE__);
	}
	m_entered_state = fsm_present;

	m_state = FINITE_STATE_MACHINE::execution_state::ACTIVE;
	return m_entered_state;
}

bool abstract_state::exit_state()
{
	m_state = FINITE_STATE_MACHINE::execution_state::COMPLETE;
	return true;
}
