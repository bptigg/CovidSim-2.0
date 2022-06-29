#include "Agent.h"

Agent::Agent()
{
	const void* m_address = (const void*)this; 
	std::stringstream ss;
	ss << m_address;
	m_agent_id = ss.str();
	m_edit = true;

	m_extrovert = (double)random::Random_number(0, 100, {}, true) * 0.01;

}

Agent::~Agent()
{
	//std::cout << "Agent deleted" << std::endl;
}

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

void Agent::set_target_location(const std::pair<unsigned int, unsigned int>& location)
{
	m_target_x = location.first;
	m_target_y = location.second;
}

std::pair<unsigned int, unsigned int> Agent::get_target_location()
{
	return std::pair<unsigned int, unsigned int>(m_target_x,m_target_y);
}

void Agent::attach_fsm(std::vector<std::shared_ptr<abstract_state>> states)
{
	while (m_edit == false)
	{

	};

	modify_permission();
	
	std::shared_ptr<finite_state_machine> state_machine(new finite_state_machine());
	for (int i = 0; i < states.size(); i++)
	{
		states[i]->attach_fsm(fsm);
	}

	fsm = std::move(state_machine);
	fsm->awake(states);

	modify_permission();
}
