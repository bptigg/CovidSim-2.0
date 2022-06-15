#pragma once
#include <string>
#include <vector>

#include "Agent.h"
#include "Log.h"

class task
{
private:
	std::string m_task_id;

	std::pair<int, int> m_target_location;
	std::string m_building_id;

	std::vector<std::shared_ptr<Agent>> m_target_agents;

	int m_task_length;
	int m_task_cduration;

	bool task_initlized;

public:

	const std::string& task_id = m_task_id;
	const std::string& building_id = m_building_id;

	const std::pair<int, int>& location = m_target_location;

	const int& task_length = m_task_length;
	const int& task_cduration = m_task_cduration;

private:
	void create_id();
public:

	task();
	~task();

	bool initlize_task(std::pair<int,int> location, std::string b_id, std::vector<std::shared_ptr<Agent>>& target_agents, int run_time);

	bool update_task(int run);
	
	void update_task_length(int run_time);
	void update_target_agents(std::vector<std::shared_ptr<Agent>>& target_agents);
	void update_target_agent(std::shared_ptr<Agent> target_agent);

};

