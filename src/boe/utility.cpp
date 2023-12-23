#include "boe/utility.hpp"
#include <algorithm>

short boe_clamp(short k, short min, short max)
{
	return std::clamp(k, min, max);
}

short move_to_zero(short val)
{
	if (val < 0)
		return val + 1;
	if (val > 0)
		return val - 1;
	return val;
}
