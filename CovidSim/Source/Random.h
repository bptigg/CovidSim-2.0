#pragma once

#include <vector>
#include <random>

#include "Log.h"

namespace random
{
	std::vector<unsigned int> Discrete_distribution(std::vector<double>& weights, unsigned int runs, bool random_device = false);
	int Discrete_distribution(std::vector<double> weights, bool random_device = false);
	unsigned int Random_number(int min, int max, std::vector<int> used = {}, bool random_device = false);
	unsigned int Normal_distribution(unsigned int mean, unsigned int standard_dev);
}
