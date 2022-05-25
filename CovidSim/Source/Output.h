#pragma once
#include <fstream>

#include "Infection.h"
#include "Log.h"

class Output_Lock_Guard
{
	bool m_write = false;
	bool m_kill = false;
	bool m_data_collected = true;

	std::mutex m_output_lock;
public:
	const bool& write = m_write;
	const bool& kill = m_kill;
	const bool& data_collected = m_data_collected;

	void modify_write(bool state)
	{
		m_output_lock.lock();

		m_write = state;

		m_output_lock.unlock();
	};

	void modify_kill(bool state)
	{
		m_output_lock.lock();

		m_kill = state;

		m_output_lock.unlock();
	}

	void modify_dc(bool state)
	{
		m_output_lock.lock();

		m_data_collected = state;

		m_output_lock.unlock();
	}
};

class output
{
public:
	enum class DATA
	{
		OFF = 0, ON
	};
private:
	std::string m_filepath;

	DATA m_suceptible;
	DATA m_latent;
	DATA m_infected;
	DATA m_recovered;

	Infection* m_infection;
private:
	void suceptible(unsigned int num);
	void latent(unsigned int num);
	void infected(unsigned int num);
	void recovered(unsigned int num);
public:
	void output_data(Output_Lock_Guard* olg);
	
	output(DATA s, DATA l, DATA i, DATA r, Infection* infection);
};