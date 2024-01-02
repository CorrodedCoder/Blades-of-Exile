#pragma once

#include "boe/game_file_structures.hpp"

class Scenario
{
	const scenario_data_type& scenariodata_;

public:
	Scenario(const scenario_data_type& scene);

	[[nodiscard]] unsigned char out_width(void) const;
	[[nodiscard]] unsigned char out_height(void) const;
	[[nodiscard]] short which_town_start(void) const;
	[[nodiscard]] unsigned char num_towns(void) const;
	[[nodiscard]] unsigned char town_size(short index) const;
	[[nodiscard]] const terrain_type_type& ter_type(short index) const;
	[[nodiscard]] unsigned char intro_pic(void) const;
	[[nodiscard]] const monster_record_type& monster(short index) const;
	[[nodiscard]] const special_node_type& specials(short index) const;
	[[nodiscard]] short special_item(short index) const;
	[[nodiscard]] short timer_times(short index) const;
	[[nodiscard]] short timer_specs(short index) const;
	[[nodiscard]] unsigned char difficulty(void) const;
	[[nodiscard]] short town_to_add_to(short index) const;
	[[nodiscard]] short flag_to_add_to_town(short a, short b) const;
	[[nodiscard]] short store_item_town(short index) const;
	[[nodiscard]] const BoeRect& store_item_rects(short index) const;
	[[nodiscard]] const boat_record_type& boats(short index) const;
	[[nodiscard]] const horse_record_type& horses(short index) const;
	[[nodiscard]] const location& where_start(void) const;
	[[nodiscard]] unsigned char town_hidden(short index) const;
	[[nodiscard]] const location& out_sec_start(void) const;
	[[nodiscard]] const location& out_start(void) const;
};
