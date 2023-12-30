#include "boe/utility.hpp"
#include <algorithm>
#include <cstdlib>

short boe_clamp(short k, short low, short high)
{
	return std::clamp(k, low, high);
}

short move_to_zero(short val)
{
	if (val < 0)
		return val + 1;
	if (val > 0)
		return val - 1;
	return val;
}

short get_ran(short times, short low, short high)
{
	short to_ret = 0;

#ifdef __APPLE__
	for (short i = 1; i < times + 1; ++i)
	{
		const long int store = Random();
		to_ret = to_ret + low + (((store + 32767) * (high - low + 1)) / 65536);
	}
#else
	if ((high - low + 1) == 0)
		return 0;

	for (short i = 1; i < times + 1; ++i)
	{
		const short store = std::rand() % (high - low + 1);
		to_ret = to_ret + low + store;
	}
#endif

	return to_ret;
}

short rand_short(short low, short high)
{
	return get_ran(1, low, high);
}
