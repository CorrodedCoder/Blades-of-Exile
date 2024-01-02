#include "scenario_ext.hpp"
#include "global_structs.hpp"

scenario_data_type scenariodata;
piles_of_stuff_dumping_type2 data_store2;

const Scenario scenario(scenariodata);

const char* scenario_header_string(short a, short b)
{
	return data_store2.scen_header_strs[a][b];
}

const char* scenario_name_string(short index)
{
	return data_store2.scen_names[index];
}

const item_record_type& scenario_item(short index)
{
	return data_store2.scen_item_list.scen_items[index];
}

const char* scenario_monster_name(short index)
{
	return data_store2.scen_item_list.monst_names[index];
}

const char* scenario_terrain_name(short index)
{
	return data_store2.scen_item_list.ter_names[index];
}
