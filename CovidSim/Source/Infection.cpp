#include "Infection.h"

bool Infection::collision_detection(Agent* infected_agent, Agent* target_agent)
{
	std::pair<unsigned int, unsigned int> agent_1 = infected_agent->get_location();
	std::pair<unsigned int, unsigned int> agent_2 = infected_agent->get_location();
	
	if (get_distance(agent_1.first, agent_2.first, agent_1.second, agent_1.second) < m_infection_radius)
	{
		return true;
	}
	else
	{
		return false; 
	}
}

double Infection::get_distance(const int x_1, const int x_2, const int y_1, const int y_2)
{
	int delta_x = x_2 - x_1;
	int delta_y = y_2 - y_1;

	return std::sqrt(delta_x * delta_x + delta_y * delta_y);
}

void Infection::set_infection_vector(std::vector<Agent*> infected)
{
	infected_agents = infected;
}

void Infection::update_latent_vector(std::vector<Agent*> infected_vec)
{
	m_infection_lock.lock();

	if (infected_vec.empty() == false)
	{
		//infected_agents.insert(infected_agents.end(), infected_vec.begin(), infected_vec.end());
		for (int i = 0; i < infected_vec.size(); i++)
		{
			infected_vec[i]->set_infection_state(Agent::infection_state::LATENT);
			latent_agents.push_back(infected_vec[i]);
			infected_vec[i]->set_latent_time(random::Random_number(CONSTANTS::MIN_lATENT_TIME, CONSTANTS::MAX_LATENT_TIME));
		}
	}
	
	m_infection_lock.unlock();
}

void Infection::update_infected_vector()
{
	int removed = 0;
	for (int i = 0; i < latent_agents.size(); i++)
	{
		int index = i - removed;
		if (latent_agents[index]->get_infected_time() == latent_agents[index]->get_latent_time())
		{
			infected_agents.push_back(latent_agents[index]);
			latent_agents[index]->set_infection_state(Agent::infection_state::INFECTED);
			latent_agents.erase(std::remove(latent_agents.begin(), latent_agents.end(), latent_agents[index]));
			removed++;
		}
		else
		{
			latent_agents[index]->set_infected_time(latent_agents[index]->get_infected_time() + 1);
		}
	}
}

Infection::Infection(Enviroment* world)
	:m_world(world)
{
}

void Infection::run_infection(Infection::infection_type type)
{
	std::vector<Agent*> o_infected = {};
	unsigned int num_infected = infected_agents.size();
	switch (type)
	{
	case Infection::infection_type::OUTDOORS:
		for (int i = 0; i < num_infected; i++)
		{
			if (infected_agents[i]->get_infection_state() == Agent::infection_state::INFECTED)
			{
				std::vector<Agent*> sample = get_sample(infected_agents[i]);
				for (int e = 0; e < sample.size(); e++)
				{
					if (infected_agents[i] == sample[e])
					{
						continue;
					}
					if (collision_detection(infected_agents[i], sample[e]) == true)
					{
						std::vector<double> infection_dist = { 1 - CONSTANTS::INFECTION_PROBABILTY, CONSTANTS::INFECTION_PROBABILTY };
						if (random::Discrete_distribution(infection_dist, 1, true)[0] == 1)
						{
							o_infected.push_back(sample[e]);
						}
					}
				}
			}
		}
		update_latent_vector(o_infected);
	default:
		break;
	}
	update_infected_vector();
}

void Infection::set_radius(unsigned int radius)
{
	m_infection_radius = radius;
}

std::vector<Agent*> Infection::get_sample(Agent*& target_agent)
{
	std::vector<Agent*> sample = {};
	std::pair<unsigned int, unsigned int> centre_point = target_agent->get_location();

	int min_x = centre_point.first - m_infection_radius;
	int max_x = centre_point.first + m_infection_radius + 1;
	int min_y = centre_point.second - m_infection_radius;
	int max_y = centre_point.second + m_infection_radius + 1;

	min_x = (min_x < 0) ? 0 : min_x;
	max_x = (max_x > m_world->get_size()) ? m_world->get_size() : max_x;
	min_y = (min_y < 0) ? 0 : min_x;
	max_y = (max_y > m_world->get_size()) ? m_world->get_size() : max_y;

	for (int i = min_x; i < max_y; i++)
	{
		for (int e = min_y; e < max_y; e++)
		{
			if (m_world->pass_grid(i,e)->agents.empty() == false)
			{
				for (int a = 0; a < m_world->pass_grid(i,e)->agents.size(); a++)
				{
					if (m_world->pass_grid(i, e)->agents[a]->get_infection_state() == Agent::infection_state::SUCEPTIBLE)
					{
						sample.push_back(m_world->pass_grid(i, e)->agents[a]);
					}
				}
			}

		}
	}

	return sample;
}
