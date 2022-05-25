#pragma once

#include <vector>
#include <tuple>

#include <thread>
#include <mutex>

#include "Agent.h"
#include "Matrix.h"
//#include "Building types/public_buildings.h"

#include "Constants.h"

class Enviroment
{
public:
	struct Points
	{
		std::vector<Agent*> agents = {};
	};

	CONSTANTS::DAY_OF_THE_WEEK m_day; // temp

private:
	unsigned int m_grid_size = 0;
	unsigned int m_num_agents = 0;
	std::vector<std::vector<Points*>> grid = {};

	std::mutex m_position_lock;
public:
	Enviroment(unsigned int size, unsigned int num_agents);
	~Enviroment();
	void initilise_agent_location(std::vector<Agent*> locations);

	void edit_agent_location(const std::pair<unsigned int, unsigned int>& new_location, Agent* target_agent);
	//void update_public_building(int count, std::unique_ptr<public_buildings>& building); // will probably need to change 

	unsigned int get_size();
	const Enviroment::Points* pass_grid(unsigned int index_1, unsigned int index_2);

	unsigned int get_num_agents();

};

