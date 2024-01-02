#include "scenario.hpp"
#include "global_structs.hpp"

scenario_data_type scenariodata;
piles_of_stuff_dumping_type2 data_store2;

unsigned char scenario_out_width(void)
{
	return scenariodata.out_width;
}

unsigned char scenario_out_height(void)
{
	return scenariodata.out_height;
}

short scenario_which_town_start(void)
{
	return scenariodata.which_town_start;
}

unsigned char scenario_num_towns(void)
{
	return scenariodata.num_towns;
}

unsigned char scenario_town_size(short index)
{
	return scenariodata.town_size[index];
}

const terrain_type_type& scenario_ter_type(short index)
{
	return scenariodata.ter_types[index];
}

unsigned char scenario_intro_pic(void)
{
	return scenariodata.intro_pic;
}

const monster_record_type& scenario_monster(short index)
{
	return scenariodata.scen_monsters[index];
}

const special_node_type& scenario_specials(short index)
{
	return scenariodata.scen_specials[index];
}

short scenario_special_item(short index)
{
	return scenariodata.special_item_special[index];
}

short scenario_timer_times(short index)
{
	return scenariodata.scenario_timer_times[index];
}

short scenario_timer_specs(short index)
{
	return scenariodata.scenario_timer_specs[index];
}

unsigned char scenario_difficulty(void)
{
	return scenariodata.difficulty;
}

short scenario_town_to_add_to(short index)
{
	return scenariodata.town_to_add_to[index];
}

short scenario_flag_to_add_to_town(short a, short b)
{
	return scenariodata.flag_to_add_to_town[a][b];
}

short scenario_store_item_town(short index)
{
	return scenariodata.store_item_towns[index];
}

const BoeRect& scenario_store_item_rects(short index)
{
	return scenariodata.store_item_rects[index];
}

const boat_record_type& scenario_boats(short index)
{
	return scenariodata.scen_boats[index];
}

const horse_record_type& scenario_horses(short index)
{
	return scenariodata.scen_horses[index];
}

const location& scenario_where_start(void)
{
	return scenariodata.where_start;
}

unsigned char scenario_town_hidden(short index)
{
	return scenariodata.town_hidden[index];
}

const location& scenario_out_sec_start(void)
{
	return scenariodata.out_sec_start;
}

const location& scenario_out_start(void)
{
	return scenariodata.out_start;
}

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
