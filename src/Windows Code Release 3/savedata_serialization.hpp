#pragma once

#include "game_file_structures.hpp"
#include <istream>
#include <ostream>
#include <exception>
#include <array>

class boe_error
	: public std::exception
{
	short error_code_;

public:
	boe_error(short error_code);
	short error_code() const { return error_code_; }
};

void savedata_read_all(
	std::istream& file_id,
	bool& town_restore,
	bool& in_scen,
	party_record_type& party,
	setup_save_type& setup_save,
	std::array<pc_record_type, 6>& adven,
	unsigned char(&out_e)[96][96],
	current_town_type& c_town,
	big_tr_type& t_d,
	town_item_list& t_i,
	stored_items_list_type(&stored_items)[3],
	stored_town_maps_type& town_maps,
	stored_town_maps_type& town_maps2,
	stored_outdoor_maps_type& o_maps,
	unsigned char(&sfx)[64][64],
	unsigned char(&misc_i)[64][64]
);

void savedata_write_all(
	std::ostream& file_id,
	bool town_save,
	bool in_startup_mode,
	bool save_maps,
	party_record_type& party,
	const setup_save_type& setup_save,
	std::array<pc_record_type, 6>& adven,
	const unsigned char(&out_e)[96][96],
	const current_town_type& c_town,
	const big_tr_type& t_d,
	const town_item_list& t_i,
	const stored_items_list_type(&stored_items)[3],
	const stored_town_maps_type& town_maps,
	const stored_town_maps_type& town_maps2,
	const stored_outdoor_maps_type& o_maps,
	const unsigned char(&sfx)[64][64],
	const unsigned char(&misc_i)[64][64]
);
