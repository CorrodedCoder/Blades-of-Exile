#pragma once

#include <istream>
#include <ostream>

bool dump_savegame(std::ostream_iterator<char> strm, std::istream& input);
