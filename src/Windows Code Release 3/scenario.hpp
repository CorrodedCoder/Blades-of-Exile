#pragma once

#include "boe/game_file_structures.hpp"

[[nodiscard]] unsigned char scenario_out_width(void);
[[nodiscard]] unsigned char scenario_out_height(void);
[[nodiscard]] short scenario_which_town_start(void);
[[nodiscard]] unsigned char scenario_num_towns(void);
[[nodiscard]] unsigned char scenario_town_size(short index);
[[nodiscard]] const terrain_type_type& scenario_ter_type(short index);
[[nodiscard]] unsigned char scenario_intro_pic(void);
[[nodiscard]] const monster_record_type& scenario_monster(short index);
[[nodiscard]] const special_node_type& scenario_specials(short index);
[[nodiscard]] short scenario_special_item(short index);
[[nodiscard]] short scenario_timer_times(short index);
[[nodiscard]] short scenario_timer_specs(short index);
[[nodiscard]] unsigned char scenario_difficulty(void);
[[nodiscard]] unsigned char scenario_difficulty(void);
[[nodiscard]] short scenario_town_to_add_to(short index);
[[nodiscard]] short scenario_flag_to_add_to_town(short a, short b);
[[nodiscard]] short scenario_store_item_town(short index);
[[nodiscard]] const BoeRect& scenario_store_item_rects(short index);
[[nodiscard]] const boat_record_type& scenario_boats(short index);
[[nodiscard]] const horse_record_type& scenario_horses(short index);
[[nodiscard]] const location& scenario_where_start(void);
[[nodiscard]] unsigned char scenario_town_hidden(short index);
[[nodiscard]] const location& scenario_out_sec_start(void);
[[nodiscard]] const location& scenario_out_start(void);

[[nodiscard]] const char* scenario_header_string(short a, short b);
[[nodiscard]] const char* scenario_name_string(short index);
[[nodiscard]] const item_record_type& scenario_item(short index);
[[nodiscard]] const char* scenario_monster_name(short index);
[[nodiscard]] const char* scenario_terrain_name(short index);
