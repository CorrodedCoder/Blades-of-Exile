#include "boe/scenario_extra.hpp"

ScenarioExtra::ScenarioExtra(const scenario_extra_data_type& store)
	: store_(store)
{

}

const char* ScenarioExtra::header_string(short a, short b) const
{
	return store_.scen_header_strs[a][b];
}

const char* ScenarioExtra::name_string(short index) const
{
	return store_.scen_names[index];
}

const item_record_type& ScenarioExtra::item(short index) const
{
	return store_.scen_item_list.scen_items[index];
}

const char* ScenarioExtra::monster_name(short index) const
{
	return store_.scen_item_list.monst_names[index];
}

const char* ScenarioExtra::terrain_name(short index) const
{
	return store_.scen_item_list.ter_names[index];
}
