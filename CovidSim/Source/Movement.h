#pragma once

#include <memory>

#include "Agent.h"
#include "Matrix.h"
#include "Random.h"

#include "Enviroment.h"

class Movement
{
private:
	unsigned int m_grid_size; 
	std::shared_ptr<Enviroment> m_world;

public:
	Movement(unsigned int grid, std::shared_ptr<Enviroment> world);
	void update_agent(std::shared_ptr<Agent> target_agent);
private:
	std::pair<unsigned int, unsigned int> agent_random_walk(std::shared_ptr<Agent>& target_agent);
	std::pair<unsigned int, unsigned int> agent_weighted_walk(std::shared_ptr<Agent>& target_agent);

	Matrix<double> generate_move_matrix(std::shared_ptr<Agent> target_agent, bool random = false);
};

