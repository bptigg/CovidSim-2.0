#pragma once
#include <fstream>

#include "Infection.h"
#include "Log.h"

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
	void output_data(bool* write, bool* kill);
	
	output(DATA s, DATA l, DATA i, DATA r, Infection* infection);
};