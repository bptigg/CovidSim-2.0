#pragma once

#include <vector>
#include <random>

#include "Log.h"

namespace random
{
	std::vector<unsigned int> Discrete_distribution(std::vector<double>& weights, unsigned int runs, bool random_device = false);
	unsigned int Random_number(int min, int max, std::vector<int> used = {});
}
