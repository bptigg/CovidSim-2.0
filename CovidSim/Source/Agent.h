#pragma once
#include <iostream>
#include <tuple>
#include <vector>
#include <mutex>

#include "Random.h"

class Agent
{
public:
	enum class infection_state
	{
		SUCEPTIBLE = 0, LATENT, INFECTED, REMOVED
	};

	enum class task_state
	{
		IDLE = 0, TRANSIT, ACTIVE
	};
private:
	int m_x = 0;
	int m_y = 0;

	int m_target_x = 0;
	int m_target_y = 0;

	int m_task_x = 0;
	int m_task_y = 0;

	int m_age = 0;
	double m_extrovert = 0;

	infection_state m_i_state = infection_state::SUCEPTIBLE;
	task_state m_t_state = task_state::IDLE;
	
	unsigned int m_latent_time = 0;
	unsigned int m_infected_time = 0;
	unsigned int m_revovery_time = 0;
	unsigned int m_recovered_time = 0;

	std::string m_house_id;
	std::string m_task_id;
	std::string m_work_id;

	std::string m_agent_id;

	bool m_edit = true;
	std::mutex edit_permission;

public:
#pragma region FINITE_STATE_MACHINE
	//class finite_state_machine;
#pragma endregion
public:

	Agent();
	~Agent();

	Agent::infection_state get_infection_state();
	void set_infection_state(infection_state state);

	unsigned int get_latent_time();
	void set_latent_time(unsigned int time);

	unsigned int get_infected_time();
	void set_infected_time(unsigned int time);

	unsigned int get_recovery_time();
	void set_recovery_time(unsigned int time);

	unsigned int get_recoverd_time();
	void set_recovered_time(unsigned int time);

	Agent::task_state get_task_state();
	void set_task_state(task_state state);

	void set_location(const std::pair<unsigned int, unsigned int>& location);
	std::pair<unsigned int, unsigned int> get_location();

	void set_target_location(const std::pair<unsigned int, unsigned int>& location);
	std::pair<unsigned int, unsigned int> get_target_location();

	void set_task_location(const std::pair<unsigned int, unsigned int>& location);
	std::pair<unsigned int, unsigned int> get_task_location();

	void set_house_id(std::string id) { m_house_id = id; }
	void set_task_id(std::string id) { m_task_id = id; }
	void set_work_id(std::string id) { m_work_id = id; }
	void set_age(int age) { m_age = age; }

	void modify_permission()
	{
		edit_permission.lock();
		m_edit = !m_edit;
		edit_permission.unlock();
	}


public:
	const std::string& work_id = m_work_id;
	const std::string& task_id = m_task_id;
	const std::string& house_id = m_house_id;
	const std::string& agent_id = m_agent_id;
	const int& age = m_age;
	const double& extrovert = m_extrovert;

	const bool& edit = m_edit;

	std::vector<std::string> friends;

};

