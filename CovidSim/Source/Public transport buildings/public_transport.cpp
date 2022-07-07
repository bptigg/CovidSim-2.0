#include "public_transport.h"

void public_transport::update_open_status(int time, CONSTANTS::DAY_OF_THE_WEEK key)
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

void public_transport::update_agent_amount()
{
	m_agent_amount = m_in_building.size() - m_staff_amount;
}

const unsigned int public_transport::get_default_capacity()
{
	unsigned int default_capacity = 0;

	for (int i = 0; i < m_types.size(); i++)
	{
		switch (m_types[i])
		{
		case public_transport::public_transport_type::BUS:
			default_capacity = default_capacity + DEFAULT_CAPACITY::BUS;
			break;
		case public_transport::public_transport_type::LIGHT_RAIL:
			default_capacity = default_capacity + DEFAULT_CAPACITY::LIGHT_RAIL;
			break;
		case public_transport::public_transport_type::RAPID_TRANSIT:
			default_capacity = default_capacity + DEFAULT_CAPACITY::RAPID_TRANSIT;
			break;
		case public_transport::public_transport_type::TRAINS:
			default_capacity = default_capacity + DEFAULT_CAPACITY::TRAINS;
			break;
		default:
			break;
		}
	}

	double modifer = 0;
	switch (m_size)
	{
	case base_building::SIZE::SMALL:
		modifer = BUILDING_MODIFERS::SMALL;
		break;
	case base_building::SIZE::MEDIUM:
		modifer = BUILDING_MODIFERS::MEDIUM;
		break;
	case base_building::SIZE::LARGE:
		modifer = BUILDING_MODIFERS::LARGE;
		break;
	case base_building::SIZE::DEFUALT_SIZE:
		modifer = BUILDING_MODIFERS::MEDIUM;
		break;
	default:
		return 0;
		break;
	}

	return std::floor(modifer * (double)default_capacity);
}

public_transport::public_transport()
{
	m_opening_hours = {};
	m_staff = {};
	m_staff_amount = 0;
	m_types = {};
	m_size = base_building::SIZE::DEFUALT_SIZE;
}

public_transport::~public_transport()
{
	m_staff.clear();
	m_lines.clear();
}

void public_transport::update_building(int time, CONSTANTS::DAY_OF_THE_WEEK day)
{
	update_open_status(time, day);
	update_agent_amount();
}

void public_transport::update_opening_hours(std::vector<std::pair<CONSTANTS::DAY_OF_THE_WEEK, std::pair<int, int>>> hours)
{
	for (auto it = hours.begin(); it < hours.end(); it++)
	{
		m_opening_hours[it->first] = it->second;
	}
}

std::vector<std::shared_ptr<Agent>> public_transport::get_staff()
{
	return m_staff;
}

void public_transport::add_staff(std::vector<std::shared_ptr<Agent>>& staff)
{
	for (int i = 0; i < staff.size(); i++)
	{
		if (find(staff[i], m_staff).first == true)
		{
			Log::warning("AGENT ALREADY STAFF MEMBER");
		}
		else
		{
			m_staff_lock.lock();
			m_staff.push_back(std::move(staff[i]));
			m_staff_lock.unlock();
		}
	}
}

void public_transport::add_staff(std::shared_ptr<Agent> staff_member)
{
	if (find(staff_member, m_staff).first == true)
	{
		Log::warning("AGENT ALREADY STAFF MEMBER");
	}
	else
	{
		m_staff_lock.lock();
		m_staff.push_back(std::move(staff_member));
		m_staff_lock.unlock();
	}
}

void public_transport::remove_staff(std::shared_ptr<Agent>& staff_member)
{
	std::pair<bool, int> occupant = find(staff_member, m_staff);
	if (occupant.first == false)
	{
		Log::warning("AGENT NOT STAFF");
		return;
	}
	m_staff_lock.lock();
	m_staff.erase(m_staff.begin() + occupant.second);
	m_staff_lock.unlock();
}

void public_transport::set_staff_amount(int amount)
{
	m_staff_amount = amount;
}

void public_transport::set_capacity(int amount)
{
	if (amount == -1)
	{
		m_capacity = get_default_capacity();
	}
	else
	{
		m_capacity = amount;
	}
}

void public_transport::set_type(std::vector<public_transport_type> types)
{
	m_types = types;
}

void public_transport::set_size(base_building::SIZE size)
{
	m_size = size;
}
