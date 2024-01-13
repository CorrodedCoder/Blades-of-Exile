#include <Windows.h>
#include "../global.h"
#include <cmath>
#include <cstdlib>

extern short give_delays;

short s_pow(short x,short y)
{
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
