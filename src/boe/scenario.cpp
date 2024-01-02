#include "boe/scenario.hpp"

Scenario::Scenario(const scenario_data_type& scene)
	: scenariodata_(scene)
{
}

unsigned char Scenario::out_width(void) const
{
	return scenariodata_.out_width;
}

unsigned char Scenario::out_height(void) const
{
	return scenariodata_.out_height;
}

short Scenario::which_town_start(void) const
{
	return scenariodata_.which_town_start;
}

unsigned char Scenario::num_towns(void) const
{
	return scenariodata_.num_towns;
}

unsigned char Scenario::town_size(short index) const
{
	return scenariodata_.town_size[index];
}

const terrain_type_type& Scenario::ter_type(short index) const
{
	return scenariodata_.ter_types[index];
}

unsigned char Scenario::intro_pic(void) const
{
	return scenariodata_.intro_pic;
}

const monster_record_type& Scenario::monster(short index) const
{
	return scenariodata_.scen_monsters[index];
}

const special_node_type& Scenario::specials(short index) const
{
	return scenariodata_.scen_specials[index];
}

short Scenario::special_item(short index) const
{
	return scenariodata_.special_item_special[index];
}

short Scenario::timer_times(short index) const
{
	return scenariodata_.scenario_timer_times[index];
}

short Scenario::timer_specs(short index) const
{
	return scenariodata_.scenario_timer_specs[index];
}

unsigned char Scenario::difficulty(void) const
{
	return scenariodata_.difficulty;
}

short Scenario::town_to_add_to(short index) const
{
	return scenariodata_.town_to_add_to[index];
}

short Scenario::flag_to_add_to_town(short a, short b) const
{
	return scenariodata_.flag_to_add_to_town[a][b];
}

short Scenario::store_item_town(short index) const
{
	return scenariodata_.store_item_towns[index];
}

const BoeRect& Scenario::store_item_rects(short index) const
{
	return scenariodata_.store_item_rects[index];
}

const boat_record_type& Scenario::boats(short index) const
{
	return scenariodata_.scen_boats[index];
}

const horse_record_type& Scenario::horses(short index) const
{
	return scenariodata_.scen_horses[index];
}

const location& Scenario::where_start(void) const
{
	return scenariodata_.where_start;
}

unsigned char Scenario::town_hidden(short index) const
{
	return scenariodata_.town_hidden[index];
}

const location& Scenario::out_sec_start(void) const
{
	return scenariodata_.out_sec_start;
}

const location& Scenario::out_start(void) const
{
	return scenariodata_.out_start;
}
