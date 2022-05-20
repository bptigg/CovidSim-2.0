#pragma once

#include "Log.h"

namespace calculations
{
	double basic_reproductive_number(unsigned int previous, unsigned int now);
	double effective_reproductive_number(double basic_r, double sucept_frac);
}