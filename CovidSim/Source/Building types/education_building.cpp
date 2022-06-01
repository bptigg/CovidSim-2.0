#include "education_building.h"

void education_building::update_open_status(int time, CONSTANTS::DAY_OF_THE_WEEK key)
{
	std::pair<int, int> hours = m_school_hours[key];
	if (time < hours.first || time > hours.second)
	{
		m_closed = true;
	}
	else if (time > hours.first && time < hours.second)
	{
		m_closed = false;
	}
}

void education_building::update_agent_amount()
{
	m_agent_amount = m_in_building.size() - m_staff_amount - m_student_amount;
}

const unsigned int education_building::get_default_capacity(edu_type type, base_building::SIZE bsize)
{
	return 0;
}

education_building::education_building()
{
	m_school_hours = {};

	m_student_amount = 0;
	m_student_capacity = 0;
	m_type = edu_type::default_type;
	m_size = base_building::SIZE::DEFUALT_SIZE;

	m_staff = {};
	m_students = {};
}

education_building::~education_building()
{
	m_staff.clear();
	m_students.clear();
}

void education_building::update_building(int time, CONSTANTS::DAY_OF_THE_WEEK DAY)
{
	update_open_status(time, DAY);
	update_agent_amount();
}

void education_building::update_school_hours(std::vector<std::pair<CONSTANTS::DAY_OF_THE_WEEK, std::pair<int, int>>> hours)
{
	for (auto it = hours.begin(); it < hours.end(); it++)
	{
		m_school_hours[it->first] = it->second;
	}
}

std::vector<std::shared_ptr<Agent>> education_building::get_staff()
{
	return m_staff;
}

std::vector<std::shared_ptr<Agent>> education_building::get_students()
{
	return m_students;
}

void education_building::add_staff(std::vector<std::shared_ptr<Agent>>& staff)
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

void education_building::add_staff(std::shared_ptr<Agent> staff_member)
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

void education_building::remove_staff(std::shared_ptr<Agent>& staff_member)
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

void education_building::set_staff_amount(int amount)
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

void education_building::add_students(std::vector<std::shared_ptr<Agent>>& students)
{
	for (int i = 0; i < students.size(); i++)
	{
		if (find(students[i], m_students).first == true)
		{
			Log::warning("AGENT ALREADY STUDENT");
		}
		else
		{
			m_student_lock.lock();
			m_students.push_back(std::move(students[i]));
			m_student_lock.unlock();
		}
	}
}

void education_building::add_students(std::shared_ptr<Agent> students)
{
	if (find(students, m_students).first == true)
	{
		Log::warning("AGENT ALREADY STUDENT");
	}
	else
	{
		m_student_lock.lock();
		m_students.push_back(std::move(students));
		m_student_lock.unlock();
	}
}

void education_building::remove_student(std::shared_ptr<Agent> student)
{
	std::pair<bool, int> occupant = find(student, m_students);
	if (occupant.first == false)
	{
		Log::warning("AGENT NOT STUDENT");
		return;
	}
	m_student_lock.lock();
	m_students.erase(m_staff.begin() + occupant.second);
	m_student_lock.unlock();
}

void education_building::set_student_amount(int amount)
{
	if (amount == -1)
	{
		m_student_amount = m_students.size();
	}
	else
	{
		m_student_amount = amount;
	}
}

void education_building::set_capacity(int amount)
{
	m_capacity = amount;
}

void education_building::set_type(edu_type type)
{
	m_type = type;
}

void education_building::set_size(base_building::SIZE size)
{
	m_size = size;
}
