#pragma once
#include "abstract_state.h"
class idle : public abstract_state
{
private:
	int m_idle_count;
	int m_idle_time;
public:
	idle(std::string owner);

	bool enter_state() override;
	bool exit_state() override;

	FINITE_STATE_MACHINE::FSM_state_type update_state() override;
	
};

