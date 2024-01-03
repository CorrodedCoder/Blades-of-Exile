#pragma once

#include "boe/game_file_structures.hpp"

struct scenario_extra_data_type {
	char scen_header_strs[25][3][80];
	char scen_names[25][256];
	scen_item_data_type scen_item_list;
};

class ScenarioExtra
{
	const scenario_extra_data_type& store_;
public:
	ScenarioExtra(const scenario_extra_data_type& store);

	[[nodiscard]] const char* header_string(short a, short b) const;
	[[nodiscard]] const char* name_string(short index) const;
	[[nodiscard]] const item_record_type& item(short index) const;
	[[nodiscard]] const char* monster_name(short index) const;
	[[nodiscard]] const char* terrain_name(short index) const;
};
