#pragma once

#include <tuple>

class Agent
{
public:
	enum class infection_state
	{
		SUCEPTIBLE = 0, LATENT, INFECTED, REMOVED
	};

	enum class task_state
	{
		IDLE = 0, TRANSIT, ACTIVE
	};
private:
	int m_x = 0;
	int m_y = 0;

	infection_state m_i_state = infection_state::SUCEPTIBLE;
	task_state m_t_state = task_state::IDLE;
	unsigned int m_latent_time = 0;
	unsigned int m_infected_time = 0;

public:
	Agent::infection_state get_infection_state();
	void set_infection_state(infection_state state);

	unsigned int get_latent_time();
	void set_latent_time(unsigned int time);

	unsigned int get_infected_time();
	void set_infected_time(unsigned int time);

	Agent::task_state get_task_state();
	void set_task_state(task_state state);

	void set_location(const std::pair<unsigned int, unsigned int>& location);
	std::pair<unsigned int, unsigned int> get_location();

};

