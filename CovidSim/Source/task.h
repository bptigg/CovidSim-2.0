#pragma once

#include <string>
#include <vector>

#include "Agent.h"
#include "Log.h"

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

public:
	const std::string& task_id = m_task_id;
	const std::string& building_id = m_building_id;

	const int& task_length = m_task_length;
	const int& current_run_time = m_current_run_time;

private:
	std::string get_id();

public:
	task();
	~task();

	bool update_task();
	bool initlize_task(int x, int y, std::string b_id, std::vector<std::shared_ptr<Agent>>& target_agents, int run_time); //also allows redefinition of the task

};

