#include "boe/utility.hpp"
#include <algorithm>

short boe_clamp(short k, short min, short max)
{
	return std::clamp(k, min, max);
}
