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

	const double& weather_score = m_weather_score;
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

	double m_bt_value;
	double m_tm_value;
	double m_wm_value;

	double m_weather_score = 0;

public:
	weather(std::shared_ptr<Matrix<double>> bt, std::shared_ptr<Matrix<double>> tm, std::shared_ptr<Matrix<double>> wm);
	
	void update_weather();

};

