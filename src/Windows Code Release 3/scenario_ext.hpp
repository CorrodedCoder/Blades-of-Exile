#pragma once

#include "boe/scenario.hpp"

const char* scenario_header_string(short a, short b);
const char* scenario_name_string(short index);
const item_record_type& scenario_item(short index);
const char* scenario_monster_name(short index);
const char* scenario_terrain_name(short index);

extern Scenario scenario;
