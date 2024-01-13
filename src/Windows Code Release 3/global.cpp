#include <Windows.h>
#include "global.h"
#include "boe/utility.hpp"
#include <cmath>
#include <cstdlib>

extern short give_delays;

short s_pow(short x,short y)
{
	if (y == 0)
   	return 1;
	return (short) pow((double) x, (double) y);
}

short a_v(short x)
{
	if (x < 0)
		return (-1 * x);
		else return x;
}
short ex_abs(short x)
{
	if (x < 0)
		return (-1 * x);
		else return x;
}

void pause(short length)
{
	if (give_delays == 0)
		Delay(length);
}

// stuff done legit, i.e. flags are within proper ranges for stuff done flag
Boolean sd_legit(short a, short b)
{
	if ((boe_clamp(a,0,299) == a) && (boe_clamp(b,0,9) == b))
		return TRUE;
	return FALSE;
}
