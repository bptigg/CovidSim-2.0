#include "Output.h"

void output::suceptible(unsigned int num)
{
	std::string filepath = m_filepath + "/suceptible.CVO";
	
	std::ofstream sucept;
	sucept.open(filepath, std::ios::app);
	sucept << num << std::endl;
	sucept.close();
}

void output::latent(unsigned int num)
{
	std::string filepath = m_filepath + "/latent.CVO";

	std::ofstream latent;
	latent.open(filepath, std::ios::app);
	latent << num << std::endl;
	latent.close();
}

void output::infected(unsigned int num)
{
	std::string filepath = m_filepath + "/infected.CVO";

	std::ofstream infected;
	infected.open(filepath, std::ios::app);
	infected << num << std::endl;
	infected.close();
}

void output::recovered(unsigned int num)
{
	std::string filepath = m_filepath + "/recovered.CVO";

	std::ofstream recovered;
	recovered.open(filepath, std::ios::app);
	recovered << num << std::endl;
	recovered.close();
}

void output::output_data(Output_Lock_Guard* olg)
{
	while (olg->kill == false)
	{
		while (olg->write == false);

		unsigned int suc = m_infection->num_sucept();
		unsigned int lat = m_infection->num_latent();
		unsigned int inf = m_infection->num_infected();
		unsigned int rec = m_infection->num_recovered();
		olg->modify_dc(true);
		
		auto [previous, current] = m_infection->get_infected_numbers();

		if (m_suceptible == output::DATA::ON)
		{
			suceptible(suc);
		}
		if (m_latent == output::DATA::ON)
		{
			latent(lat);
		}
		if (m_infected == output::DATA::ON)
		{
			infected(inf);
		}
		if (m_recovered == output::DATA::ON)
		{
			recovered(rec);
		}

		olg->modify_write(false);

	}
}

output::output(DATA s, DATA l, DATA i, DATA r, std::shared_ptr<Infection> infection)
{
	m_filepath = "Output/" + Log::current_time();
	m_suceptible = s;
	m_latent = l;
	m_infected = i;
	m_recovered = r;
	
	m_infection = infection;

	if (std::filesystem::exists("Output") == false)
	{
		Log::warning("No output dictonary exists");
		Log::info("Creating one");
		std::filesystem::create_directories("Output");
	}

	std::filesystem::create_directory(m_filepath);
}
