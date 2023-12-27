
#include <Windows.h>
#include "global.h"
#include <cmath>
#include <cstdlib>

extern char scen_strs[160][256];
extern char scen_strs2[110][256];
extern char talk_strs[170][256];
extern char town_strs[180][256];
extern short give_delays;

short s_pow(short x,short y)
{
	return (short) pow((double) x, (double) y);
}

short dist(location p1,location p2)
{
	return (short) sqrt((double)((p1.x - p2.x) * (p1.x - p2.x) +
							(p1.y - p2.y) * (p1.y - p2.y)));
}

short abs(short x)
{
	if (x < 0)
		return x * -1;
	return x;
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

void beep()
{
	long dummy;

	MessageBeep(MB_OK);
	Delay(200,&dummy);

	}

void SysBeep(short a)
{
	MessageBeep(MB_ICONQUESTION);

}
