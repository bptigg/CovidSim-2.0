#pragma once

#include "Matrix.h"
#include "Random.h"
#include "Constants.h"

class weather
{
public:
	enum class BASIC_TYPES
	{
		CLEAR = 0, OVERCAST, LIGHT_RAIN, MODERATE_RAIN, HEAVY_RAIN
	};

	enum class TEMP_MODIFERS
	{
		FREEZING = 0, COLD, WARM, HOT
	};

	enum class WIND_MODIFERS
	{
		CALM = 0, LIGHT_WIND, MODERATE_WIND, STRONG_WIND
	};

	const double weather_score = 0;
private:
	std::shared_ptr<Matrix<double>> m_bt_transition;
	std::shared_ptr<Matrix<double>> m_tm_transition;
	std::shared_ptr<Matrix<double>> m_wm_transition;

	BASIC_TYPES b_type;
	TEMP_MODIFERS t_mod;
	WIND_MODIFERS w_mod; 

	int m_bt;
	int m_tm;
	int m_wm;

	int m_bt_value;
	int m_tm_value;
	int m_wm_value;

public:
	weather(std::shared_ptr<Matrix<double>> bt, std::shared_ptr<Matrix<double>> tm, std::shared_ptr<Matrix<double>> wm);
	
	void update_weather();

};

