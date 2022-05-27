#include "hospital.h"

std::vector<std::shared_ptr<Agent>> hospital::get_patients()
{
    return m_patients;
}

void hospital::add_patients(std::shared_ptr<Agent> target_patient)
{
	if (find(target_patient, m_patients).first == true)
	{
		Log::warning("AGENT ALREADY IN HOSPITAL");
	}
	else
	{
		m_patient_lock.lock();
		m_patients.push_back(std::move(target_patient));
		m_patient_lock.unlock();
	}
}

void hospital::remove_patient(std::shared_ptr<Agent>& target_patient)
{
	std::pair<bool, int> occupant = find(target_patient, m_patients);
	if (occupant.first == false)
	{
		Log::warning("AGENT NOT IN HOSPITAL");
		return;
	}
	m_patient_lock.lock();
	m_patients.erase(m_patients.begin() + occupant.second);
	m_patient_lock.unlock();
}

void hospital::set_patient_capacity(int amount)
{
	m_patient_capacity = amount;
}
