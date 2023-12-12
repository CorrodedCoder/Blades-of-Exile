#include "dump_savegame.hpp"
#include "../dump_game_structures.hpp"
#include "../savedata_serialization.hpp"

#include <format>
#include <vector>
#include <bit>
#include <array>

bool dump_savegame(std::ostream_iterator<char> strm, std::istream & input)
{
	bool town_restore = false;
	bool in_scen = false;
	party_record_type party{};
	setup_save_type setup_save{};
	std::array<pc_record_type, 6> adven{};
	unsigned char out_e[96][96]{};
	current_town_type c_town{};
	big_tr_type t_d{};
	town_item_list t_i{};
	stored_items_list_type stored_items[3]{};
	stored_town_maps_type town_maps{};
	stored_town_maps_type town_maps2{};
	stored_outdoor_maps_type o_maps{};
	unsigned char sfx[64][64]{};
	unsigned char misc_i[64][64]{};
	savedata_read_all(input,
		town_restore, in_scen, party, setup_save, adven, out_e, c_town,
		t_d, t_i, stored_items, town_maps, town_maps2, o_maps, sfx, misc_i);

	std::format_to(strm, "town_restore: {}\n", town_restore);
	std::format_to(strm, "in_scen: {}\n", in_scen);

	dump_to(strm, "", "party_record_type", party);
	dump_to(strm, "", "setup_save_type", setup_save);
	dump_to(strm, "", "pc_record_type", adven);
	dump_to(strm, "", "out_e", out_e);
	dump_to(strm, "", "current_town_type", c_town);
	dump_to(strm, "", "big_tr_type", t_d);
	dump_to(strm, "", "town_item_list", t_i);
	dump_to(strm, "", "stored_items_list_type", stored_items);
	dump_to(strm, "", "stored_items_list_type", stored_items);
	dump_to(strm, "", "stored_town_maps_type", town_maps);
	dump_to(strm, "", "stored_town_maps_type", town_maps2);
	dump_to(strm, "", "stored_outdoor_maps_type", o_maps);
	dump_to(strm, "", "sfx", sfx);
	dump_to(strm, "", "misc_i", misc_i);
	std::format_to(strm, "Final position: {:x}\n", static_cast<std::size_t>(input.tellg()));
	return true;
}
