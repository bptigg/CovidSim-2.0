#pragma once

#include <string>
#include <vector>
#include <algorithm>

#include "Enviroment.h"

#include "Agent.h"
#include "Log.h"

#include "Constants.h"

class task
{
private:
	std::string m_task_id;

	int m_target_x;
	int m_target_y;
	std::string m_building_id;

	std::vector<std::shared_ptr<Agent>> m_target_agents;

	int m_task_length;
	int m_current_run_time;

	bool task_initlized;

	std::shared_ptr<Enviroment> m_enviroment;

public:
	const std::string& task_id = m_task_id;
	const std::string& building_id = m_building_id;

	const int& task_length = m_task_length;
	const int& current_run_time = m_current_run_time;

private:
	std::string get_id();

	//TASK_CONSTANTS::public_building_type generate_random_task(std::vector<int> age_range, int time, int day, double weather_score);
	double normalize_weather_modifier(double weather_score);
	double normalize_age_modifier(double mean_age, std::pair<double, double> age_bracket, bool age_limit = false); //takes the bottom age to be minumimum requirment
	double normalize_time_modifier(double time, std::pair<double, double> peak_time);
	double normalize_day_modifier(CONSTANTS::DAY_OF_THE_WEEK day, std::pair<CONSTANTS::DAY_OF_THE_WEEK, CONSTANTS::DAY_OF_THE_WEEK> peak_day);

	int mean_age(std::vector<int>& age_range);

public:
	task(std::shared_ptr<Enviroment> env);
	~task();

	bool update_task();
	bool initlize_task(int x, int y, std::string b_id, std::vector<std::shared_ptr<Agent>>& target_agents, int run_time); 

	void update_task_length(int run_time);
	void update_target_agents(std::vector<std::shared_ptr<Agent>>& target_agents);
	void update_target_agents(std::shared_ptr<Agent> target_agent);

	TASK_CONSTANTS::public_building_type generate_random_task(std::vector<int> age_range, int time, CONSTANTS::DAY_OF_THE_WEEK day, double weather_score);
};

