#pragma once
#include <vector>
#include <memory>

#include "Agent.h"
#include "Log.h"

class base_building
{
protected:
	int m_x;
	int m_y;

	bool m_closed;
	int m_active_tasks;

	int m_capacity;

	std::vector<std::shared_ptr<Agent>> m_in_building;

	std::mutex m_closed_lock;
	std::mutex m_active_task_lock;

	std::mutex m_occupant_lock;
public:
	enum class SIZE
	{
		SMALL = 0, MEDIUM, LARGE, DEFUALT_SIZE
	};

	const bool& closed = m_closed;
	const int& active_tasks = m_active_tasks;
	const int& capacity = m_capacity;
protected:
	std::pair<bool, int> find(const std::shared_ptr<Agent>& agent, std::vector<std::shared_ptr<Agent>>& target_vector);
public:
	base_building();
	virtual ~base_building();

	void modify_closed(bool state);
	void modify_active_tasks(int tasks);

	std::pair<int, int> get_location();
	void set_location(int x, int y);

	std::vector<std::shared_ptr<Agent>> get_occupants();
	std::shared_ptr<Agent> get_occupant(const std::shared_ptr<Agent>& agent);

	virtual void add_to_building(std::shared_ptr<Agent> agent);
	virtual void remove_from_building(std::shared_ptr<Agent>& agent);
};

