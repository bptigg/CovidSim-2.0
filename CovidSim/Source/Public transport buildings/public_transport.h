#pragma once

#include "../Building.h"
#include "../Constants.h"

struct line
{
	std::string name;
	
	int frequency;
	int capacity_per_vehicle;
};

class public_transport : public base_building
{
public:
	
	enum class public_transport_type
	{
		BUS = 0, LIGHT_RAIL, RAPID_TRANSIT, TRAINS, DEFAULT_TYPE //AIRPORT
	};

	const int& staff_amount = m_staff_amount;
	const std::vector<public_transport_type>& types = m_types;
	const std::vector<std::shared_ptr<line>>& lines = m_lines; 
	const base_building::SIZE& size = m_size;
	const bool& interchange = m_interchange;
	const int& node = m_node_number;

protected:
	std::unordered_map<CONSTANTS::DAY_OF_THE_WEEK, std::pair<int, int>> m_opening_hours;

	std::vector<std::shared_ptr<Agent>> m_staff;
	int m_staff_amount;

	std::vector<public_transport_type> m_types;
	std::vector<std::shared_ptr<line>> m_lines;
	bool m_interchange;
	base_building::SIZE m_size;
	int m_node_number;

	std::mutex m_staff_lock;
protected:
	void update_open_status(int time, CONSTANTS::DAY_OF_THE_WEEK key);
	void update_agent_amount() override;
	const unsigned int get_default_capacity();
public:
	public_transport();
	~public_transport();

	void update_building(int time, CONSTANTS::DAY_OF_THE_WEEK day);
	void update_opening_hours(std::vector<std::pair<CONSTANTS::DAY_OF_THE_WEEK, std::pair<int, int>>> hours);

	std::vector<std::shared_ptr<Agent>> get_staff();

#pragma region BUILDING INITLIZATION

	void add_staff(std::vector<std::shared_ptr<Agent>>& staff);
	void add_staff(std::shared_ptr<Agent> staff_member);
	void remove_staff(std::shared_ptr<Agent>& staff_member);
	void set_staff_amount(int amount);

	void set_capacity(int amount = -1);

	void set_type(std::vector<public_transport_type> type);
	void set_size(base_building::SIZE size);
#pragma endregion
};

