#pragma once

#include <vector>
#include <cmath>

#include "Agent.h"
#include "Enviroment.h"
#include "Random.h"

class Infection
{
private:
	unsigned int m_infection_radius = 0;
	std::shared_ptr<Enviroment> m_world = NULL;

	std::vector<std::shared_ptr<Agent>> infected_agents;
	std::vector<std::shared_ptr<Agent>> latent_agents;
	std::vector<std::shared_ptr<Agent>> recovered_agents;

	std::mutex m_infection_lock;

	unsigned int m_infected_previous = 0;
	unsigned int m_infected_current = 0;
public:
	enum class infection_type
	{
		OUTDOORS = 1
	};
public:
	Infection(std::shared_ptr<Enviroment> world, unsigned int starting_infected);
	~Infection();

	void run_infection(infection_type type);
	void set_radius(unsigned int radius);

	void set_infection_vector(std::vector<std::shared_ptr<Agent>> infected);

	//for output
	const unsigned int num_sucept();
	const unsigned int num_latent();
	const unsigned int num_infected();
	const unsigned int num_recovered();

	std::pair<unsigned int, unsigned int> get_infected_numbers();
private:
	bool collision_detection(std::shared_ptr<Agent>& infected_agent, std::shared_ptr<Agent>& target_agent);
	double get_distance(const int x_1, const int x_2, const int y_1, const int y_2);

	void update_latent_vector(std::vector<std::shared_ptr<Agent>> infected_vec);
	void update_infected_vector();
	
	void update_infected();
	void update_recovered_vector();

	std::vector<std::shared_ptr<Agent>> get_sample(std::shared_ptr<Agent>& target_agent); 

};

