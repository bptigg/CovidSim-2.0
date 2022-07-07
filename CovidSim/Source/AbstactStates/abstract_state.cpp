#include "abstract_state.h"

abstract_state::abstract_state(std::shared_ptr<task_builder> director = nullptr)
{
	if (director != nullptr)
	{
		m_task_director = std::move(director);
	}
	else
	{
		m_task_director = nullptr;
	}
	m_state = FINITE_STATE_MACHINE::execution_state::NONE;
	m_entered_state = false;
	m_type = FINITE_STATE_MACHINE::FSM_state_type::NONE;
	//m_agent = owner;
}

bool abstract_state::enter_state()
{
	m_entered_state = true;
	if (m_agent == "")
	{
		m_entered_state == false;
		Log::error("FSM STATE HAS NO OWNER", __FILE__, __LINE__);
		return m_entered_state;
	}
	m_state = FINITE_STATE_MACHINE::execution_state::ACTIVE;
	return m_entered_state;
}

bool abstract_state::exit_state()
{
	m_state = FINITE_STATE_MACHINE::execution_state::COMPLETE;
	m_entered_state = false;
	return true;
}
