#pragma once

#include <memory>

#include "../log.h"
#include "../Random.h"
#include "../Constants.h"

#include "../finite_state_machine.h"

class finite_state_machine; //forward decleration

class abstract_state
{
protected:
	FINITE_STATE_MACHINE::execution_state m_state;
	FINITE_STATE_MACHINE::FSM_state_type m_type;

	std::shared_ptr<finite_state_machine> fsm;
	bool m_entered_state;

public:
	const FINITE_STATE_MACHINE::execution_state& state = m_state;
	const FINITE_STATE_MACHINE::FSM_state_type& type = m_type;
	void set(const FINITE_STATE_MACHINE::execution_state new_state) { m_state = new_state; }
public:
	abstract_state();

	void attach_fsm(std::shared_ptr<finite_state_machine> state_machine);

	virtual bool enter_state();
	virtual bool exit_state();

	virtual void update_state() = 0;
};

