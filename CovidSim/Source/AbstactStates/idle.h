#pragma once
#include "abstract_state.h"
class idle : public abstract_state
{
private:
	int m_idle_count;
	int m_idle_time;
public:
	idle();

	bool enter_state() override;
	bool exit_state() override;

	void update_state() override;
	
};

