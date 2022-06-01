#pragma once

#include <vector>
#include <tuple>

#include <thread>
#include <mutex>

#include "Agent.h"
#include "Matrix.h"

#include "building_types.h"

#include "Constants.h"

class Enviroment
{
public:
	struct Points
	{
		std::vector<std::shared_ptr<Agent>> agents = {};
	};

	CONSTANTS::DAY_OF_THE_WEEK m_day = CONSTANTS::DAY_OF_THE_WEEK::MONDAY;

private:
	unsigned int m_grid_size = 0;
	unsigned int m_num_agents = 0;
	std::vector<std::vector<std::shared_ptr<Points>>> grid = {};

	std::map<std::string, std::tuple<std::shared_ptr<public_building>, std::shared_ptr<hospital>, std::shared_ptr<house>, std::shared_ptr<education_building>>> m_building_id_table;

	std::mutex m_position_lock;
public:
	Enviroment(unsigned int size, unsigned int num_agents, CONSTANTS::DAY_OF_THE_WEEK day);
	~Enviroment();
	void initilise_agent_location(std::vector<std::shared_ptr<Agent>> locations);

	void edit_agent_location(const std::pair<unsigned int, unsigned int>& new_location, std::shared_ptr<Agent> target_agent);
	//void update_public_building(int count, std::unique_ptr<public_buildings>& building); // will probably need to change 

	unsigned int get_size();
	const std::shared_ptr<Enviroment::Points> pass_grid(unsigned int index_1, unsigned int index_2);

	unsigned int get_num_agents();

};

