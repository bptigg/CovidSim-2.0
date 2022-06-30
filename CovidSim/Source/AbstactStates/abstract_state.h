#pragma once

#include <memory>

#include "../log.h"
#include "../Random.h"
#include "../Constants.h"


/*NEED TO CHANGE
	- REMOVE INTERACTION BETWEEN STATE AND STATE MANAGER (FSM)
	- DO THIS BY HAIVNG UPDATE RETURN A ENUM OF TYPE 'FSM_state_type'
	- REMOVE FORWARD DECLERATIONS
	- BY DOING THIS THE ISSUE OF A FSM NOT BEING ATTACHTED TO THE STATE DOESNT MATTER AS THE STATE IS DOES NOT NEED TO INTERACT WITH THE FSM
	- THIS SHOULD ALSO REMOVE ANY CIRCULAR REFERENCES DOWN THE LINE DEALING WITH AGENT MOVEMENT IN THE TRANSIT STATE
*/

class abstract_state
{
protected:
	FINITE_STATE_MACHINE::execution_state m_state;
	FINITE_STATE_MACHINE::FSM_state_type m_type;
	bool m_entered_state;
	std::string m_agent;

public:
	const FINITE_STATE_MACHINE::execution_state& state = m_state;
	const FINITE_STATE_MACHINE::FSM_state_type& type = m_type;
	void set(const FINITE_STATE_MACHINE::execution_state new_state) { m_state = new_state; }
public:
	abstract_state();

	virtual bool enter_state();
	virtual bool exit_state();

	virtual FINITE_STATE_MACHINE::FSM_state_type update_state() = 0;
};

