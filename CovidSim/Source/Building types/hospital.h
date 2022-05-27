#pragma once
#include "public_building.h"
class hospital : public public_building
{
public:
	const int& patient_amount = m_patient_amount;
	const int& patient_capacity = m_patient_capacity;
protected:
	std::vector<std::shared_ptr<Agent>> m_patients;
	int m_patient_amount;
	int m_patient_capacity;

	std::mutex m_patient_lock;
public:
	std::vector<std::shared_ptr<Agent>> get_patients();
	void add_patients(std::shared_ptr<Agent> target_patient);
	void remove_patient(std::shared_ptr<Agent>& target_patient);
	void set_patient_capacity(int amount);
};

