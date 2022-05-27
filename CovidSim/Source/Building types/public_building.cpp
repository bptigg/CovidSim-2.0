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

const unsigned int public_building::get_default_capacity(pb_type type, base_building::SIZE bsize)
{
	unsigned int default_capacity = 0;
	switch (type)
	{
	case public_building::pb_type::POW:
		default_capacity = DEFAULT_CAPACITY::POW;
		break;
	case public_building::pb_type::PARK:
		default_capacity = DEFAULT_CAPACITY::PARK;
		break;
	case public_building::pb_type::CAFE:
		default_capacity = DEFAULT_CAPACITY::CAFE;
		break;
	case public_building::pb_type::RESTAURANT:
		default_capacity = DEFAULT_CAPACITY::RESTAURANT;
		break;
	case public_building::pb_type::CINEMA:
		default_capacity = DEFAULT_CAPACITY::CINEMA;
		break;
	case public_building::pb_type::THEATRE:
		default_capacity = DEFAULT_CAPACITY::THEATRE;
		break;
	case public_building::pb_type::GENERIC_SHOP:
		default_capacity = DEFAULT_CAPACITY::GENERIC_SHOP;
		break;
	case public_building::pb_type::SUPERMARKET:
		default_capacity = DEFAULT_CAPACITY::SUPERMARKET;
		break;
	case public_building::pb_type::SHOPPING_CENTRE:
		default_capacity = DEFAULT_CAPACITY::SHOPPING_CENTRE;
		break;
	case public_building::pb_type::PUB:
		default_capacity = DEFAULT_CAPACITY::PUB;
		break;
	case public_building::pb_type::NIGHTCLUB:
		default_capacity = DEFAULT_CAPACITY::NIGHTCLUB;
		break;
	case public_building::pb_type::ARENA:
		default_capacity = DEFAULT_CAPACITY::ARENA;
		break;
	case public_building::pb_type::DEFUALT_TYPE:
		return 0;
	default:
		break;
	}

	double modifer = 0;
	switch (bsize)
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

public_building::public_building()
{
	m_opening_hours = {};
	m_staff = {};
	m_staff_amount = 0;
	m_type = pb_type::DEFUALT_TYPE;
	m_size = base_building::SIZE::DEFUALT_SIZE;
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

std::vector<std::shared_ptr<Agent>> public_building::get_staff()
{
	return m_staff;
}

void public_building::add_staff(std::vector<std::shared_ptr<Agent>>& staff)
{
	for (int i = 0; i < staff.size(); i++)
	{
		if (find(staff[i]).first == true)
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

void public_building::add_staff(std::shared_ptr<Agent> staff_member)
{
	if (find(staff_member).first == true)
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

void public_building::set_staff_amount(int amount)
{
	if (amount == -1)
	{
		m_staff_amount = m_staff.size();
	}
	else
	{
		m_staff_amount = amount;
	}
}

void public_building::set_capacity(int amount)
{
	if (amount == -1)
	{
		m_capacity = get_default_capacity(m_type, m_size);
	}
	else
	{
		m_staff_amount = amount;
	}
}

void public_building::set_type(pb_type type)
{
	m_type = type;
}

void public_building::set_size(base_building::SIZE size)
{
	m_size = size;
}
