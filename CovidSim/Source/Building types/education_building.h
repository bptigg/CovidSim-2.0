#pragma once

#include "../Building.h"
#include "../Constants.h"

#include <map>

class education_building :public base_building
{
public:
	enum class edu_type
	{
		NURSARY = 0, PRIMARY, SECONDARY, COLLEGE, UNI, default_type
	};

	const int& staff_amount = m_staff_amount;
	const int& student_amount = m_student_amount;
	const int& student_capacity = m_student_capacity;
	const edu_type& type = m_type;
	const base_building::SIZE& size = m_size;

private:
	std::map<CONSTANTS::DAY_OF_THE_WEEK, std::pair<int, int>> m_school_hours;

	std::vector<std::shared_ptr<Agent>> m_staff;
	int m_staff_amount;

	std::vector<std::shared_ptr<Agent>> m_students;
	int m_student_amount;
	int m_student_capacity;

	edu_type m_type;
	base_building::SIZE m_size;

	std::mutex m_staff_lock;
	std::mutex m_student_lock;
protected:
	void update_open_status(int time, CONSTANTS::DAY_OF_THE_WEEK key);
	void update_agent_amount() override;
	const unsigned int get_default_capacity(edu_type type, base_building::SIZE bsize);
public:
	education_building();
	~education_building();

	void update_building(int time, CONSTANTS::DAY_OF_THE_WEEK DAY);
	void update_school_hours(std::vector<std::pair<CONSTANTS::DAY_OF_THE_WEEK, std::pair<int, int>>> hours);

	std::vector<std::shared_ptr<Agent>> get_staff();
	std::vector<std::shared_ptr<Agent>> get_students(); 

#pragma region BUILDING INITILAZTION

	void add_staff(std::vector<std::shared_ptr<Agent>>& staff);
	void add_staff(std::shared_ptr<Agent> staff_member);
	void remove_staff(std::shared_ptr<Agent>& staff_member);
	void set_staff_amount(int amount = -1);

	void add_students(std::vector<std::shared_ptr<Agent>>& students);
	void add_students(std::shared_ptr<Agent> students);
	void remove_student(std::shared_ptr<Agent> student);
	void set_student_amount(int amount = -1);

	void set_capacity(int amount = -1);

	void set_type(edu_type type);
	void set_size(base_building::SIZE size);

#pragma endregion

};

