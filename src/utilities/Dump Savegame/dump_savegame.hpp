#pragma once

#include <istream>
#include <ostream>
#include <iterator>

bool dump_savegame(std::ostream_iterator<char> strm, std::istream& input);
