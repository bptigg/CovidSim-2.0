#pragma once

#include <vector>
#include <tuple>

#include <thread>
#include <mutex>

#include "Agent.h"
#include "Matrix.h"

#include "clock.h"
#include "weather.h"

#include "building_types.h"

#include "Constants.h"


class Enviroment
{
public:
	struct Points
	{
		std::vector<std::shared_ptr<Agent>> agents = {};
	};

	CONSTANTS::DAY_OF_THE_WEEK& day = m_day;
	const int& time_of_day = m_time_of_day;
	//const double& weather_score = m_weather->weather_score;

	double min_weather_score;
	double max_weather_score;
private:
	unsigned int m_grid_size = 0;
	unsigned int m_num_agents = 0;
	std::vector<std::vector<std::shared_ptr<Points>>> grid = {};

	//std::map<std::string, std::tuple<std::shared_ptr<public_building>, std::shared_ptr<hospital>, std::shared_ptr<house>, std::shared_ptr<education_building>>> m_building_id_table;

	std::map<TASK_CONSTANTS::public_building_type, std::map<std::string, std::shared_ptr<public_building>>> m_pbuilding_id_table;
	std::map<education_building::edu_type, std::map<std::string, std::shared_ptr<education_building>>> m_ebuilding_id_table;
	std::map<std::string, std::shared_ptr<hospital>> m_hospital_id_table;
	std::map<std::string, std::shared_ptr<house>> m_house_id_table;

	std::mutex m_position_lock;

	CONSTANTS::DAY_OF_THE_WEEK m_day;
	int m_time_of_day;

	std::unique_ptr<world_clock> m_clk;
	std::unique_ptr<weather> m_weather;
public:
	Enviroment(unsigned int size, unsigned int num_agents, CONSTANTS::DAY_OF_THE_WEEK day, std::unique_ptr<world_clock> clk, std::unique_ptr<weather> weath);
	~Enviroment();
	void initilise_agent_location(std::vector<std::shared_ptr<Agent>> locations);

	std::map<std::string, std::shared_ptr<public_building>> return_public_list(TASK_CONSTANTS::public_building_type type) { return m_pbuilding_id_table[type]; }

	void edit_agent_location(const std::pair<unsigned int, unsigned int>& new_location, std::shared_ptr<Agent> target_agent);
	//void update_public_building(int count, std::unique_ptr<public_buildings>& building); // will probably need to change 

	unsigned int get_size();
	const std::shared_ptr<Enviroment::Points> pass_grid(unsigned int index_1, unsigned int index_2);

	unsigned int get_num_agents();

	void update_world_clock();

	double get_weather_score();

};

