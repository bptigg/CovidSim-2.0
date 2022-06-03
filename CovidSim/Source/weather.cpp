#include "weather.h"

weather::weather(std::shared_ptr<Matrix<double>> bt, std::shared_ptr<Matrix<double>> tm, std::shared_ptr<Matrix<double>> wm)
{
	m_bt_transition = std::move(bt);
	m_tm_transition = std::move(tm);
	m_wm_transition = std::move(wm);

	m_bt = 0;
	m_tm = 0;
	m_wm = 0;
}

void weather::update_weather()
{
	m_bt = random::Discrete_distribution( (*m_bt_transition.get())(m_bt), 1, true)[0];
	m_tm = random::Discrete_distribution((*m_tm_transition.get())(m_tm), 1, true)[0];
	m_wm = random::Discrete_distribution((*m_wm_transition.get())(m_wm), 1, true)[0];

	switch (m_bt)
	{
	case 0:
		b_type = BASIC_TYPES::CLEAR;
		break;
	case 1:
		b_type = BASIC_TYPES::OVERCAST;
		break;
	case 2:
		b_type = BASIC_TYPES::LIGHT_RAIN;
		break;
	case 3:
		b_type = BASIC_TYPES::MODERATE_RAIN;
		break;
	case 4:
		b_type = BASIC_TYPES::HEAVY_RAIN;
		break;
	default:
		break;
	}

	switch (m_tm)
	{
	case 0:
		t_mod = TEMP_MODIFERS::FREEZING;
		break;
	case 1:
		t_mod = TEMP_MODIFERS::COLD;
		break;
	case 2:
		t_mod = TEMP_MODIFERS::WARM;
		break;
	case 3:
		t_mod = TEMP_MODIFERS::HOT;
		break;
	default:
		break;
	}

	switch (m_wm)
	{
	case 0:
		w_mod = WIND_MODIFERS::CALM;
		break;
	case 1:
		w_mod = WIND_MODIFERS::LIGHT_WIND;
		break;
	case 2:
		w_mod = WIND_MODIFERS::MODERATE_WIND;
		break;
	case 3:
		w_mod = WIND_MODIFERS::STRONG_WIND;
		break;
	default:
		break;
	}

	//do some stuff with constants
	//weather_score = std::sqrt(m_bt_value + m_tm_value + m_wm_value);
}


