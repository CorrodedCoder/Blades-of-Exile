#include "boe/registration.hpp"
#include <array>
#include <algorithm>

namespace {
	const std::array<int, 6> c_pow{ 1,10,100,1000,9999,99999 };
}

short wd_to_pwd(std::string_view str)
{
	char pwd[8] = "aaaaaa";

	for (size_t i = 0; i < std::min(static_cast<size_t>(6), str.size()); ++i)
	{
		if ((str[i] >= 65) && (str[i] <= 90))
		{
			pwd[i] = str[i] + 32;
		}
		else if ((str[i] >= 48) && (str[i] <= 57))
		{
			pwd[i] = str[i] + 49;
		}
		else if ((str[i] >= 97) && (str[i] <= 122))
		{
			pwd[i] = str[i];
		}
	}

	int val = 0;
	for (size_t i = 0; i < 6; ++i)
	{
		val += c_pow[i] * static_cast<int>(pwd[i] - 97);
	}
	val %= 30000;

	return static_cast<short>(val);
}
