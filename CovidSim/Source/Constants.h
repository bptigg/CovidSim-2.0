#pragma once

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

	const unsigned int DAY_LENGTH = 1440;
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