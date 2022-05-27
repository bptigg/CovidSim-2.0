#pragma once

#include "../Building.h"

#include "../Constants.h"
#include <map>

class public_building : public base_building
{
public:
	enum class pb_type
	{
		POW = 0, PARK, CAFE, RESTAURANT, CINEMA, THEATRE, GENERIC_SHOP, SUPERMARKET, SHOPPING_CENTRE, PUB, NIGHTCLUB, ARENA, DEFUALT_TYPE
	};

	const int& staff_amount = m_staff_amount;
	const pb_type& type = m_type;
	const base_building::SIZE& size = m_size;

private:
	std::map<CONSTANTS::DAY_OF_THE_WEEK, std::pair<int, int>> m_opening_hours;

	std::vector<std::shared_ptr<Agent>> m_staff;
	int m_staff_amount;

	pb_type m_type;
	base_building::SIZE m_size;

	std::mutex m_staff_lock;
private:
	void update_open_status(int time, CONSTANTS::DAY_OF_THE_WEEK key);
	const unsigned int get_default_capacity(pb_type type, base_building::SIZE bsize);
public:
	public_building();
	~public_building();

	void update_building(int time, CONSTANTS::DAY_OF_THE_WEEK DAY);
	void update_opening_hours(std::vector<std::pair<CONSTANTS::DAY_OF_THE_WEEK, std::pair<int, int>>> hours);

	std::vector<std::shared_ptr<Agent>> get_staff();

#pragma region BUILDING INITLIZATION

	void add_staff(std::vector<std::shared_ptr<Agent>>& staff);
	void add_staff(std::shared_ptr<Agent> staff_member);
	void set_staff_amount(int amount = -1);

	void set_capacity(int amount = -1);

	void set_type(pb_type type);
	void set_size(base_building::SIZE size);
#pragma endregion
};