#pragma once

#include "../Building.h"

#include "../Constants.h"
#include <map>

class public_building : public base_building
{
private:
	std::map<CONSTANTS::DAY_OF_THE_WEEK, std::pair<int, int>> m_opening_hours;

	std::vector<std::shared_ptr<Agent>> m_staff;

private:
	void update_open_status(int time, CONSTANTS::DAY_OF_THE_WEEK key);
public:
	public_building();
	~public_building();

	void update_building(int time, CONSTANTS::DAY_OF_THE_WEEK DAY);
	void update_opening_hours(std::vector<std::pair<CONSTANTS::DAY_OF_THE_WEEK, std::pair<int, int>>> hours);

};