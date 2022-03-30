#include "Agent.h"

Agent::infection_state Agent::get_infection_state()
{
	return m_i_state;
}

void Agent::set_infection_state(infection_state state)
{
	m_i_state = state; 
}

unsigned int Agent::get_latent_time()
{
	return m_latent_time;
}

unsigned int Agent::get_infected_time()
{
	return m_infected_time;
}

void Agent::set_infected_time(unsigned int time)
{
	m_infected_time = time;
}

unsigned int Agent::get_recovery_time()
{
	return m_revovery_time;
}

void Agent::set_recovery_time(unsigned int time)
{
	m_revovery_time = time;
}

unsigned int Agent::get_recoverd_time()
{
	return m_recovered_time;
}

void Agent::set_recovered_time(unsigned int time)
{
	m_recovered_time = time;
}

void Agent::set_latent_time(unsigned int time)
{
	m_latent_time = time;
}

Agent::task_state Agent::get_task_state()
{
	return m_t_state;
}

void Agent::set_task_state(task_state state)
{
	m_t_state = state;
}

void Agent::set_location(const std::pair<unsigned int, unsigned int>& location)
{
	m_x = location.first;
	m_y = location.second;
}

std::pair<unsigned int, unsigned int> Agent::get_location()
{
	return std::pair<unsigned int, unsigned int>(m_x, m_y);
}
