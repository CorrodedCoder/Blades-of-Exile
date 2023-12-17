#pragma once

#include <istream>
#include <ostream>
#include <iterator>

bool dump_scenario(std::ostream_iterator<char> strm, std::istream& input);
