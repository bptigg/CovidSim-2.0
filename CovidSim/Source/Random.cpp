#include "Random.h"

std::vector<unsigned int> random::Discrete_distribution(std::vector<double>& weights, unsigned int runs, bool random_device)
{
	std::vector<unsigned int> results = {};

	std::string time = Log::current_time();
	std::seed_seq stime(time.begin(), time.end());

	std::discrete_distribution<int> dist(std::begin(weights), std::end(weights));
	std::mt19937 gen;

	if (random_device == false)
	{
		gen.seed(stime);
	}
	else
	{
		gen.seed(std::random_device{}());
	}


	for (unsigned int i = 0; i < runs; i++)
	{
		unsigned int result = dist(gen);
		results.push_back(result);
	}

	return results;
}

unsigned int random::Random_number(int min, int max, std::vector<int> used)
{
	std::default_random_engine gen;
	std::uniform_int_distribution<int> random(min, max-1);
	bool number_found = false;

	std::string time = Log::current_time();
	std::seed_seq stime(time.begin(), time.end());
	gen.seed(stime);

	while (number_found == false)
	{
		if (used.empty() == false)
		{
			unsigned int random_number = random(gen);
			if (std::find(used.begin(), used.end(), random_number) == used.end())
			{
				number_found = true;
				return random_number;
			}
		}
		else
		{
			return random(gen);
		}
	}

	return 0;
}

unsigned int random::Normal_distribution(unsigned int mean, unsigned int standard_dev)
{
	std::default_random_engine ngen;
	std::normal_distribution<double> dist(mean, standard_dev);
	ngen.seed(std::random_device{}());
	unsigned int result = (unsigned int)dist(ngen);
	return result;
}




