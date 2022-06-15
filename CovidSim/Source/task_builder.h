#pragma once

#include <string>
#include <vector>
#include <algorithm>

#include "Enviroment.h"

#include "Agent.h"
#include "task.h"

#include "Log.h"

#include "Constants.h"

class task_builder 
{
private:
	bool task_initlized;

	std::shared_ptr<Enviroment> m_enviroment;
	
	std::map<std::string, std::shared_ptr<task>> m_task_list;
private:

	TASK_CONSTANTS::public_building_type generate_random_task(std::vector<int> age_range, int time, CONSTANTS::DAY_OF_THE_WEEK day, double weather_score);
	std::string get_task_building(std::pair<int, int> location, TASK_CONSTANTS::public_building_type building, double weather_mod, std::vector<std::shared_ptr<Agent>> target_group = {});
	
	double normalize_weather_modifier(double weather_score);
	double normalize_age_modifier(double mean_age, std::pair<double, double> age_bracket, bool age_limit = false); //takes the bottom age to be minumimum requirment
	double normalize_time_modifier(double time, std::pair<double, double> peak_time);
	double normalize_day_modifier(CONSTANTS::DAY_OF_THE_WEEK day, std::pair<CONSTANTS::DAY_OF_THE_WEEK, CONSTANTS::DAY_OF_THE_WEEK> peak_day);
	double normalize_distance(double distance, std::pair<double, double>disance_bracket);

	int mean_age(std::vector<int>& age_range);
	double get_distance(std::pair<int, int> agent, std::pair<int, int> building);

#pragma region QUICKSORT

	double partition(std::vector<std::pair<double, int>>& arr, int low, int high);
	void quicksort(std::vector<std::pair<double, int>>& arr, int low, int high);

#pragma endregion

public:
	task_builder(std::shared_ptr<Enviroment> env);

	bool create_task(std::vector<std::shared_ptr<Agent>>& target_agents, std::shared_ptr<task> m_task);
	bool request_task();
};

