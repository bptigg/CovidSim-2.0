#pragma once

#include "Agent.h"
#include "Matrix.h"
#include "Random.h"

#include "Enviroment.h"

class Movement
{
private:
	unsigned int m_grid_size; 
	Enviroment* m_world;

public:
	Movement(unsigned int grid, Enviroment* world);
	void update_agent(Agent* target_agent);
private:
	std::pair<unsigned int, unsigned int> agent_random_walk(Agent*& target_agent);
};

