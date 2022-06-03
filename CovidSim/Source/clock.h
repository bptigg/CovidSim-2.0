#pragma once

#include "Constants.h"

class world_clock
{
private:
	int m_day_count;
	CONSTANTS::DAY_OF_THE_WEEK m_day; 
public:
	const int& day_count = m_day_count;
	const CONSTANTS::DAY_OF_THE_WEEK& day = m_day;
public:
	world_clock();

	bool update_clock();
	void set_up_clock(CONSTANTS::DAY_OF_THE_WEEK Day);
};

