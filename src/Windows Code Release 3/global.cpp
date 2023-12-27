#include <Windows.h>
#include "global.h"
#include "boe/utility.hpp"
#include <cmath>
#include <cstdlib>

extern short give_delays;

Boolean same_point(location p1,location p2)
{
	if ((p1.x == p2.x) & (p1.y == p2.y))
		return TRUE;
		else return FALSE;
}

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

void Delay(short val,long *dummy)
{
	long then,now,wait_val;

	wait_val = (long) val;
	wait_val = wait_val * 16;
	then = (long)GetCurrentTime();
	now = then;
	while (now - then < wait_val)   {
		now = (long) GetCurrentTime();
		}
}

void pause(short length)
{
	long dummy,len;
	
	len = (long) length;
	
	if (give_delays == 0)
		Delay(len, &dummy);
}

// stuff done legit, i.e. flags are within proper ranges for stuff done flag
Boolean sd_legit(short a, short b)
{
	if ((boe_clamp(a,0,299) == a) && (boe_clamp(b,0,9) == b))
		return TRUE;
	return FALSE;
}
