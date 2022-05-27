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
}

void education_building::update_building(int time, CONSTANTS::DAY_OF_THE_WEEK DAY)
{
}

void education_building::update_school_hours(std::vector<std::pair<CONSTANTS::DAY_OF_THE_WEEK, std::pair<int, int>>> hours)
{
}

std::vector<std::shared_ptr<Agent>> education_building::get_staff()
{
	return std::vector<std::shared_ptr<Agent>>();
}

std::vector<std::shared_ptr<Agent>> education_building::get_students()
{
	return std::vector<std::shared_ptr<Agent>>();
}

void education_building::add_stuff(std::vector<std::shared_ptr<Agent>>& staff)
{
}

void education_building::add_staff(std::shared_ptr<Agent> staff_member)
{
}

void education_building::remove_staff(std::shared_ptr<Agent>& staff_member)
{
}

void education_building::set_staff_amount(int amount)
{
}

void education_building::add_students(std::vector<std::shared_ptr<Agent>>& students)
{
}

void education_building::add_students(std::shared_ptr<Agent> students)
{
}

void education_building::remove_student(std::shared_ptr<Agent> student)
{
}

void education_building::set_student_amount(int amount)
{
}

void education_building::set_capacity(int amount)
{
}

void education_building::set_type(edu_type type)
{
}

void education_building::set_size(base_building::SIZE size)
{
}
