#pragma once

#include "game_file_structures.hpp"

void endian_adjust(scenario_data_type& scenario);
void endian_adjust(scen_item_data_type& scen_item_list);
void endian_adjust(outdoor_record_type& out);
void endian_adjust(tiny_tr_type& t_d);
void endian_adjust(ave_tr_type& t_d);
void endian_adjust(big_tr_type& t_d);
void endian_adjust(town_record_type& town);
void endian_adjust(talking_record_type& talking);
void endian_adjust(short& value);
