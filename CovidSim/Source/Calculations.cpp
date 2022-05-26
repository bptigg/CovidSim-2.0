#include "Calculations.h"

double calculations::basic_reproductive_number(unsigned int previous, unsigned int now)
{
	if (now > 0 && previous == 0)
	{
		Log::error("VALUE OF R_0 UNDEFINED, DUE TO INFINITE INFECTION", __FILE__, __LINE__);
		return 0;
	}
	else if(now > 0 && previous > 0)
	{
		return now / previous;
	}

	return 0;

}

double calculations::effective_reproductive_number(double basic_r, double sucept_frac)
{
	return basic_r * sucept_frac;
}
