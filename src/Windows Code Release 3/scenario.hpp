#pragma once

#include "boe/game_file_structures.hpp"

[[nodiscard]] unsigned char scenario_out_width(void);
[[nodiscard]] unsigned char scenario_out_height(void);
[[nodiscard]] short scenario_which_town_start(void);
[[nodiscard]] unsigned char scenario_num_towns(void);
[[nodiscard]] unsigned char scenario_town_size(short index);
[[nodiscard]] const terrain_type_type& scenario_ter_type(short index);
