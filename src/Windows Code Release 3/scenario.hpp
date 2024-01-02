#pragma once

#include "boe/game_file_structures.hpp"

class Scenario
{
	const scenario_data_type& scenariodata_;

public:
	Scenario(const scenario_data_type& scene);

	[[nodiscard]] unsigned char out_width(void);
	[[nodiscard]] unsigned char out_height(void);
	[[nodiscard]] short which_town_start(void);
	[[nodiscard]] unsigned char num_towns(void);
	[[nodiscard]] unsigned char town_size(short index);
	[[nodiscard]] const terrain_type_type& ter_type(short index);
	[[nodiscard]] unsigned char intro_pic(void);
	[[nodiscard]] const monster_record_type& monster(short index);
	[[nodiscard]] const special_node_type& specials(short index);
	[[nodiscard]] short special_item(short index);
	[[nodiscard]] short timer_times(short index);
	[[nodiscard]] short timer_specs(short index);
	[[nodiscard]] unsigned char difficulty(void);
	[[nodiscard]] short town_to_add_to(short index);
	[[nodiscard]] short flag_to_add_to_town(short a, short b);
	[[nodiscard]] short store_item_town(short index);
	[[nodiscard]] const BoeRect& store_item_rects(short index);
	[[nodiscard]] const boat_record_type& boats(short index);
	[[nodiscard]] const horse_record_type& horses(short index);
	[[nodiscard]] const location& where_start(void);
	[[nodiscard]] unsigned char town_hidden(short index);
	[[nodiscard]] const location& out_sec_start(void);
	[[nodiscard]] const location& out_start(void);
};

const char* scenario_header_string(short a, short b);
const char* scenario_name_string(short index);
const item_record_type& scenario_item(short index);
const char* scenario_monster_name(short index);
const char* scenario_terrain_name(short index);

extern Scenario scenario;
