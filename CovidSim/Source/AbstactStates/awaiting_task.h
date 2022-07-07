#pragma once
#include "abstract_state.h"
#include "../task_builder.h"

class awaiting_task : public abstract_state
{
private:
	double m_task_chance;
	bool m_group_task;
	int m_chances;
public:
	awaiting_task(std::string owner);

	bool enter_state() override;
	bool exit_state() override;

	FINITE_STATE_MACHINE::FSM_state_type update_state() override;
};

