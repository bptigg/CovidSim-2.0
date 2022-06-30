#include "finite_state_machine.h"

finite_state_machine::finite_state_machine()
{
	m_awake = false;
}

void finite_state_machine::awake(std::vector<std::shared_ptr<abstract_state>> valid_states)
{
	if (m_awake == true)
	{
		Log::info("STATE MACHINE ALREADY AWAKE");
		return;
	}
	m_current_state = nullptr;
	m_start_state = nullptr;
	m_awake = true;
	
	for (int i = 0; i < valid_states.size(); i++)
	{
		m_fsm_states[valid_states[i]->type] = std::move(valid_states[i]);
	}
}

void finite_state_machine::start(std::shared_ptr<abstract_state> start_state)
{
	if (m_start_state != start_state)
	{
		m_start_state = std::move(start_state);
		EnterState(m_start_state);
	}
	else if (m_start_state == nullptr)
	{
		EnterState(FINITE_STATE_MACHINE::FSM_state_type::IDLE);
	}
}

void finite_state_machine::update()
{
	if (m_current_state != nullptr && m_awake == true)
	{
		FINITE_STATE_MACHINE::FSM_state_type return_value = m_current_state->update_state();
		if (return_value != FINITE_STATE_MACHINE::FSM_state_type::NONE)
		{
			EnterState(return_value);
		}
	}
}

void finite_state_machine::EnterState(std::shared_ptr<abstract_state>& next_state)
{
	if (next_state == nullptr)
	{
		return;
	}

	if (m_current_state != nullptr)
	{
		m_current_state->exit_state();
	}

	m_previous_state = std::move(m_current_state);
	m_current_state = next_state;

	bool entered_next_state = m_current_state->enter_state();
	
	if (entered_next_state != true)
	{
		Log::error("FSM FAILED TO PROCEED TO NEXT STATE", __FILE__, __LINE__);
		bool enter_previous_state = false;
		if (m_previous_state != nullptr)
		{
			m_current_state = std::move(m_previous_state);
			m_previous_state = next_state;
			enter_previous_state = m_current_state->enter_state();
		}
		if (enter_previous_state == false)
		{
			Log::crit("FSM CAN'T USE PREVIOUS STATE AS BACKUP", __FILE__, __LINE__);
			m_awake = false;
		}
	}
}

void finite_state_machine::EnterState(FINITE_STATE_MACHINE::FSM_state_type state_type)
{
	if (m_fsm_states.find(state_type) != m_fsm_states.end())
	{
		EnterState(m_fsm_states[state_type]);
	}
}
