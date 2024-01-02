#include "scenario.hpp"

Scenario::Scenario(const scenario_data_type& scene)
	: scenariodata_(scene)
{
}

unsigned char Scenario::out_width(void)
{
	return scenariodata_.out_width;
}

unsigned char Scenario::out_height(void)
{
	return scenariodata_.out_height;
}

short Scenario::which_town_start(void)
{
	return scenariodata_.which_town_start;
}

unsigned char Scenario::num_towns(void)
{
	return scenariodata_.num_towns;
}

unsigned char Scenario::town_size(short index)
{
	return scenariodata_.town_size[index];
}

const terrain_type_type& Scenario::ter_type(short index)
{
	return scenariodata_.ter_types[index];
}

unsigned char Scenario::intro_pic(void)
{
	return scenariodata_.intro_pic;
}

const monster_record_type& Scenario::monster(short index)
{
	return scenariodata_.scen_monsters[index];
}

const special_node_type& Scenario::specials(short index)
{
	return scenariodata_.scen_specials[index];
}

short Scenario::special_item(short index)
{
	return scenariodata_.special_item_special[index];
}

short Scenario::timer_times(short index)
{
	return scenariodata_.scenario_timer_times[index];
}

short Scenario::timer_specs(short index)
{
	return scenariodata_.scenario_timer_specs[index];
}

unsigned char Scenario::difficulty(void)
{
	return scenariodata_.difficulty;
}

short Scenario::town_to_add_to(short index)
{
	return scenariodata_.town_to_add_to[index];
}

short Scenario::flag_to_add_to_town(short a, short b)
{
	return scenariodata_.flag_to_add_to_town[a][b];
}

short Scenario::store_item_town(short index)
{
	return scenariodata_.store_item_towns[index];
}

const BoeRect& Scenario::store_item_rects(short index)
{
	return scenariodata_.store_item_rects[index];
}

const boat_record_type& Scenario::boats(short index)
{
	return scenariodata_.scen_boats[index];
}

const horse_record_type& Scenario::horses(short index)
{
	return scenariodata_.scen_horses[index];
}

const location& Scenario::where_start(void)
{
	return scenariodata_.where_start;
}

unsigned char Scenario::town_hidden(short index)
{
	return scenariodata_.town_hidden[index];
}

const location& Scenario::out_sec_start(void)
{
	return scenariodata_.out_sec_start;
}

const location& Scenario::out_start(void)
{
	return scenariodata_.out_start;
}
