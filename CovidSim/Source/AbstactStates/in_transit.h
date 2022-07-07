#pragma once
#include "abstract_state.h"

#include "../Movement.h"

class in_transit : public abstract_state
{
private:
	bool m_path_to_destination = false;
	bool m_at_destination = false;
	bool m_using_a_star = false;
	bool m_arrived_at_start = false;
	bool m_at_end = false;
	bool m_valid_path = false;

	std::shared_ptr<Movement> m_movement_director; 
public:
	in_transit(std::string owner, std::shared_ptr<Movement> m_movement_director);

	bool enter_state() override;
	bool exit_state() override;

	FINITE_STATE_MACHINE::FSM_state_type update_state() override;
};

