
#include <OSUtils.h>
#include "global.h"
#include <cmath>

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
