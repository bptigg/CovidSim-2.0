#pragma once

#include <memory>
#include <vector>
#include <unordered_map>

#include "AbstactStates/abstract_state.h"

class finite_state_machine
{
private:
	std::shared_ptr<abstract_state> m_previous_state;
	std::shared_ptr<abstract_state> m_current_state;
	std::shared_ptr<abstract_state> m_start_state;

	std::unordered_map<FINITE_STATE_MACHINE::FSM_state_type, std::shared_ptr<abstract_state>> m_fsm_states;

	bool m_awake;
	

public:
	finite_state_machine();

	void awake(std::vector<std::shared_ptr<abstract_state>> valid_states);
	void start(std::shared_ptr<abstract_state> start_state = nullptr);
	void update();

#pragma region STATE_MANAGMENT
	void EnterState(std::shared_ptr<abstract_state>& next_state);
	void EnterState(FINITE_STATE_MACHINE::FSM_state_type state_type);
#pragma endregion
};

