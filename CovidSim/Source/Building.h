#pragma once
#include <vector>
#include <memory>

#include "Agent.h"
#include "Log.h"
#include "Random.h"

class base_building
{
protected:
	int m_x;
	int m_y;

	bool m_closed;
	int m_agent_amount;

	int m_capacity;

	std::vector<std::shared_ptr<Agent>> m_in_building;

	std::mutex m_closed_lock;
	std::mutex m_agent_amount_lock;

	std::mutex m_occupant_lock;

	std::string m_building_id;
public:
	enum class SIZE
	{
		SMALL = 0, MEDIUM, LARGE, DEFUALT_SIZE
	};

	const std::string& building_id = m_building_id;
	
	const bool& closed = m_closed;
	const int& agent_amount = m_agent_amount;
	const int& capacity = m_capacity;

	const std::pair<int, int>& location = std::make_pair(m_x, m_y);

protected:
	std::pair<bool, int> find(const std::shared_ptr<Agent>& agent, std::vector<std::shared_ptr<Agent>>& target_vector);
	virtual void update_agent_amount() = 0; 
public:
	base_building();
	virtual ~base_building();

	void modify_closed(bool state);

	std::pair<int, int> get_location();
	void set_location(int x, int y);

	std::vector<std::shared_ptr<Agent>> get_occupants();
	std::shared_ptr<Agent> get_occupant(const std::shared_ptr<Agent>& agent);

	virtual void add_to_building(std::shared_ptr<Agent> agent);
	virtual void remove_from_building(std::shared_ptr<Agent>& agent);
};

