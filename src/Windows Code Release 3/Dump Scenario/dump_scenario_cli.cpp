#include "dump_scenario.hpp"

#include <fstream>
#include <iostream>

#ifdef _WIN32
int wmain(int argc, wchar_t* argv[])
#else
int main(int argc, char* argv[])
#endif
{
	int result = false;

	if(3 == argc)
	{
		std::ifstream input(argv[1], std::ios_base::binary);
		std::ofstream output(argv[2]);
		result = dump_scenario(output, input);
	}
	else if(2 == argc)
	{
		std::ifstream input(argv[1], std::ios_base::binary);
		result = dump_scenario(std::cout, input);
	}
	else
	{
		std::cout << "Usage: <filepath-to-scenario>\n";
	}
	return result ? EXIT_SUCCESS : EXIT_FAILURE;
}
