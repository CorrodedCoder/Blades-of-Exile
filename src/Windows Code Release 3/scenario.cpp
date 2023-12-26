#include "scenario.hpp"

scenario_data_type scenario;


unsigned char scenario_out_width(void)
{
	return scenario.out_width;
}

unsigned char scenario_out_height(void)
{
	return scenario.out_height;
}

short scenario_which_town_start(void)
{
	return scenario.which_town_start;
}

unsigned char scenario_num_towns(void)
{
	return scenario.num_towns;
}

unsigned char scenario_town_size(short index)
{
	return scenario.town_size[index];
}

const terrain_type_type& scenario_ter_type(short index)
{
	return scenario.ter_types[index];
}
