#include "public_building.h"

void public_building::update_open_status(int time, CONSTANTS::DAY_OF_THE_WEEK key)
{
	std::pair<int, int> hours = m_opening_hours[key];
	if (time < hours.first || time > hours.second)
	{
		m_closed = true;
	}
	else if (time > hours.first && time < hours.second)
	{
		m_closed = false;
	}
}

public_building::public_building()
{
	m_opening_hours = {};
	m_staff = {};
}

public_building::~public_building()
{
	m_staff.clear();
}

void public_building::update_building(int time, CONSTANTS::DAY_OF_THE_WEEK DAY)
{
	update_open_status(time, DAY);
}

void public_building::update_opening_hours(std::vector<std::pair<CONSTANTS::DAY_OF_THE_WEEK, std::pair<int,int>>> hours)
{
	for (auto it = hours.begin(); it < hours.end(); it++)
	{
		m_opening_hours[it->first] = it->second;
	}
}
