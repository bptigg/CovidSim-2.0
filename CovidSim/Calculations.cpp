#include "Calculations.h"

double calculations::basic_reproductive_number(unsigned int previous, unsigned int now)
{
	return now / previous;
}

double calculations::effective_reproductive_number(double basic_r, double sucept_frac)
{
	return basic_r * sucept_frac;
}
