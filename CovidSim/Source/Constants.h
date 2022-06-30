#pragma once
#include <utility>

namespace CONSTANTS
{
	enum class DAY_OF_THE_WEEK
	{
		MONDAY = 0, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURDAY, SUNDAY
	};

	const unsigned int MIN_lATENT_TIME = 2880; //counts - 1 count = 1 minute
	const unsigned int MAX_LATENT_TIME = 4320;

	const double INFECTION_PROBABILTY = 0.1; //for now until actual data is obtained
	const double RECOVERY_PROBABILITY = 0.05;

	const unsigned int MIN_INFECTION_TIME = 7200;
	const unsigned int INFECTION_SD = 2400;
	const unsigned int AVG_INFECTION_TIME = 14400;
	const unsigned int REINFECTION_TIME = 20160;
	const double REINFECTION_PROBABILITY = 0.4;

	const unsigned int DAY_LENGTH = 10; //1440

	const int MAX_FRIENDS = 50;
	const int MAX_INVITES = 10;

	const int MAX_PATH_LENGTH = 5;

}

namespace TASK_CONSTANTS
{

	const int BASE_MAX_TASKS = 2000; //Based on developers system (8 logical processors), this does scale with number of logical processors
	const int TASK_RECURSION_LIMIT = 5; 
	const int MAX_TASKS_EXTENSION = 10;
	const double GROUP_TASK = 0.3;

	enum class public_building_type
	{
		POW = 0, PARK, CAFE, RESTAURANT, CINEMA, THEATRE, GENERIC_SHOP, SUPERMARKET, SHOPPING_CENTRE, PUB, NIGHTCLUB, ARENA, HOME, DEFUALT_TYPE
	};

	//TOD RANGE
	const std::pair<double, double> POW_RANGE = { 600, 840 };
	const std::pair<double, double> PARK_RANGE = { 420, 1020 };
	const std::pair<double, double> CAFE_RANGE = { 660, 960 };
	const std::pair<double, double> RESTAURANT_RANGE = { 1020, 1320 };
	const std::pair<double, double> CINEMA_RANGE = { 960, 1320 };
	const std::pair<double, double> THEATRE_RANGE = { 960, 1320 };
	const std::pair<double, double> GENERAL_SHOP_RANGE = { 480, 1200 };
	const std::pair<double, double> SUPERMARKET_RANGE = { 480, 1200 };
	const std::pair<double, double> SHOPPING_CENTRE_RANGE = { 600, 1200 };
	const std::pair<double, double> PUB_RANGE = { 1080, 1410 };
	const std::pair<double, double> NIGHTCLUB_RANGE = { 1200, 180 };
	const std::pair<double, double> ARENA_RANGE = { 1080, 1380 };
	const std::pair<double, double> HOME_RANGE = { 600, 1320 };

	//DOTY RANGE
	const std::pair<CONSTANTS::DAY_OF_THE_WEEK, CONSTANTS::DAY_OF_THE_WEEK> POW_DRANGE = { CONSTANTS::DAY_OF_THE_WEEK::MONDAY, CONSTANTS::DAY_OF_THE_WEEK::SUNDAY };
	const std::pair<CONSTANTS::DAY_OF_THE_WEEK, CONSTANTS::DAY_OF_THE_WEEK> PARK_DRANGE = { CONSTANTS::DAY_OF_THE_WEEK::MONDAY, CONSTANTS::DAY_OF_THE_WEEK::SUNDAY };
	const std::pair<CONSTANTS::DAY_OF_THE_WEEK, CONSTANTS::DAY_OF_THE_WEEK> CAFE_DRANGE = { CONSTANTS::DAY_OF_THE_WEEK::MONDAY, CONSTANTS::DAY_OF_THE_WEEK::SUNDAY };
	const std::pair<CONSTANTS::DAY_OF_THE_WEEK, CONSTANTS::DAY_OF_THE_WEEK> RESTAURANT_DRANGE = { CONSTANTS::DAY_OF_THE_WEEK::FRIDAY, CONSTANTS::DAY_OF_THE_WEEK::MONDAY };
	const std::pair<CONSTANTS::DAY_OF_THE_WEEK, CONSTANTS::DAY_OF_THE_WEEK> CINEMA_DRANGE = { CONSTANTS::DAY_OF_THE_WEEK::MONDAY, CONSTANTS::DAY_OF_THE_WEEK::SUNDAY };
	const std::pair<CONSTANTS::DAY_OF_THE_WEEK, CONSTANTS::DAY_OF_THE_WEEK> THEATRE_DRANGE = { CONSTANTS::DAY_OF_THE_WEEK::MONDAY, CONSTANTS::DAY_OF_THE_WEEK::SUNDAY };
	const std::pair<CONSTANTS::DAY_OF_THE_WEEK, CONSTANTS::DAY_OF_THE_WEEK> GENERAL_SHOP_DRANGE = { CONSTANTS::DAY_OF_THE_WEEK::MONDAY, CONSTANTS::DAY_OF_THE_WEEK::SUNDAY };
	const std::pair<CONSTANTS::DAY_OF_THE_WEEK, CONSTANTS::DAY_OF_THE_WEEK> SUPERMARKET_DRANGE = { CONSTANTS::DAY_OF_THE_WEEK::MONDAY, CONSTANTS::DAY_OF_THE_WEEK::SATURDAY };
	const std::pair<CONSTANTS::DAY_OF_THE_WEEK, CONSTANTS::DAY_OF_THE_WEEK> SHOPPING_CENTRE_DRANGE = { CONSTANTS::DAY_OF_THE_WEEK::MONDAY, CONSTANTS::DAY_OF_THE_WEEK::SATURDAY };
	const std::pair<CONSTANTS::DAY_OF_THE_WEEK, CONSTANTS::DAY_OF_THE_WEEK> PUB_DRANGE = { CONSTANTS::DAY_OF_THE_WEEK::FRIDAY, CONSTANTS::DAY_OF_THE_WEEK::SATURDAY };
	const std::pair<CONSTANTS::DAY_OF_THE_WEEK, CONSTANTS::DAY_OF_THE_WEEK> NIGHTCLUB_DRANGE = { CONSTANTS::DAY_OF_THE_WEEK::THURSDAY, CONSTANTS::DAY_OF_THE_WEEK::SATURDAY };
	const std::pair<CONSTANTS::DAY_OF_THE_WEEK, CONSTANTS::DAY_OF_THE_WEEK> ARENA_DRANGE = { CONSTANTS::DAY_OF_THE_WEEK::FRIDAY, CONSTANTS::DAY_OF_THE_WEEK::SATURDAY };
	const std::pair<CONSTANTS::DAY_OF_THE_WEEK, CONSTANTS::DAY_OF_THE_WEEK> HOME_DRANGE = { CONSTANTS::DAY_OF_THE_WEEK::MONDAY, CONSTANTS::DAY_OF_THE_WEEK::SUNDAY };

	//AGE RANGE
	const std::pair<double, double> POW_ARANGE = { 0, 100 };
	const std::pair<double, double> PARK_ARANGE = { 0, 100 };
	const std::pair<double, double> CAFE_ARANGE = { 5, 80 };
	const std::pair<double, double> RESTAURANT_ARANGE = { 15, 80 };
	const std::pair<double, double> CINEMA_ARANGE = { 8, 60 };
	const std::pair<double, double> THEATRE_ARANGE = { 15, 90 };
	const std::pair<double, double> GENERAL_SHOP_ARANGE = { 15, 80 };
	const std::pair<double, double> SUPERMARKET_ARANGE = { 20, 80 };
	const std::pair<double, double> SHOPPING_CENTRE_ARANGE = { 10, 60 };
	const std::pair<double, double> PUB_ARANGE = { 20, 50 };
	const std::pair<double, double> NIGHTCLUB_ARANGE = { 18, 25 };
	const std::pair<double, double> ARENA_ARANGE = { 16, 40 };
	const std::pair<double, double> HOME_ARANGE = { 0, 100 };

	const int MAX_AGE = 100;
	const int MIN_AGE = 0;
}

namespace DEFAULT_CAPACITY
{
	const unsigned int POW = 200;
	const unsigned int PARK = 2000;
	const unsigned int CAFE = 20;
	const unsigned int RESTAURANT = 40;
	const unsigned int CINEMA = 200;
	const unsigned int THEATRE = 300;
	const unsigned int GENERIC_SHOP = 50;
	const unsigned int SUPERMARKET = 500;
	const unsigned int SHOPPING_CENTRE = 500;
	const unsigned int NIGHTCLUB = 400;
	const unsigned int PUB = 60;
	const unsigned int ARENA= 1500;
	const unsigned int GENERIC_WORK = 50;
}

namespace BUILDING_MODIFERS
{
	const double SMALL = 0.5;
	const double MEDIUM = 1;
	const double LARGE = 2;
}

namespace WEATHER_MODIFERS
{
#pragma region BASIC_TYPES
	
	const double CLEAR = 10;
	const double OVERCAST = 9;
	const double LIGHT_RAIN = 7;
	const double MODERATE_RAIN = 4;
	const double HEAVY_RAIN = 1;

#pragma endregion

#pragma region TEMP

	const double FREEZING = 2;
	const double COLD = 5;
	const double WARM = 8;
	const double HOT = 9;

#pragma endregion

#pragma region WIND

	const double CALM = 10;
	const double LIGHT_WIND = 9;
	const double MODERATE_WIND = 7;
	const double STRONG_WIND = 5;

#pragma endregion

}

namespace FINITE_STATE_MACHINE
{
	const int AVG_IDLE_TIME = 50;
	const int IDLE_SD = 10;

	enum class execution_state
	{
		NONE = 0,
		ACTIVE,
		COMPLETE,
		TERMINATED
	};

	enum class FSM_state_type
	{
		IDLE = 0,
		AWAITING_TASK,
		TRANSIT,
		ACTIVE,
		NONE
	};
}
