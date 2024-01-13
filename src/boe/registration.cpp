#include "boe/registration.hpp"
#include <array>
#include <algorithm>

namespace {
	const std::array<int, 6> c_pow{ 1,10,100,1000,9999,99999 };
}

short wd_to_pwd(std::string_view str)
{
	char pwd[8] = "aaaaaa";

	for (int i = 0; i < std::min(6, static_cast<int>(str.size())); ++i)
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
	for (int i = 0; i < 6; ++i)
	{
		val += c_pow[i] * (int)(pwd[i] - 97);
	}
	val %= 30000;

	return (short)val;
}
