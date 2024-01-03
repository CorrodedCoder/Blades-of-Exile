#pragma once

#include "boe/game_file_structures.hpp"

struct piles_of_stuff_dumping_type2 {
	char scen_header_strs[25][3][80];
	char scen_names[25][256];
	scen_item_data_type scen_item_list;
};

class ScenarioExtra
{
	const piles_of_stuff_dumping_type2& data_store2_;
public:
	ScenarioExtra(const piles_of_stuff_dumping_type2& store);

	[[nodiscard]] const char* header_string(short a, short b) const;
	[[nodiscard]] const char* name_string(short index) const;
	[[nodiscard]] const item_record_type& item(short index) const;
	[[nodiscard]] const char* monster_name(short index) const;
	[[nodiscard]] const char* terrain_name(short index) const;
};
