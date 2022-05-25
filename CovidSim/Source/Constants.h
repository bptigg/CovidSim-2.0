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