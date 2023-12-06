#include <print>
#include <format>
#include <vector>
#include <fstream>
#include <bit>
#include <iostream>

#define BOE_GLOBALS_SKIP_FUNCTIONS
#include "../global.h"
#include "endian_adjust.hpp"

template< typename T >
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, T t)
{
	std::format_to(strm, "{}.{}={}\n", parent, item, t);
}

template< size_t N >
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const char(&arr)[N])
{
	dump_to(strm, parent, item, std::format("\"{}\"", arr));
}

template< typename T, size_t N >
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const T(&arr)[N])
{
	for (size_t index=0; index < N; ++index)
	{
		dump_to(strm, parent, std::format("{}[{}]", item, index), arr[index]);
	}
}

template<>
void dump_to<unsigned char>(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, unsigned char t)
{
	dump_to(strm, parent, item, (unsigned int)t);
}

template<>
void dump_to<char>(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, char t)
{
	dump_to(strm, parent, item, (int)t);
}

template<>
void dump_to<short>(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, short t)
{
	std::format_to(strm, "{}.{}={} {:x}\n", parent, item, t, t);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const location& t)
{
	const auto path{ std::format("{}.{}", parent, item) };
	dump_to(strm, path, "x", t.x);
	dump_to(strm, path, "y", t.y);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const out_wandering_type& t)
{
	const auto path{ std::format("{}.{}", parent, item) };
	dump_to(strm, path, "monst", t.monst);
	dump_to(strm, path, "friendly", t.friendly);

	dump_to(strm, path, "spec_on_meet", t.spec_on_meet);
	dump_to(strm, path, "spec_on_win", t.spec_on_win);
	dump_to(strm, path, "spec_on_flee", t.spec_on_flee);
	dump_to(strm, path, "cant_flee", t.cant_flee);
	dump_to(strm, path, "end_spec1", t.end_spec1);
	dump_to(strm, path, "end_spec2", t.end_spec2);
}


void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const outdoor_record_type& t)
{
	const auto path{ std::format("{}.{}", parent, item) };
	dump_to(strm, path, "terrain", t.terrain);
	dump_to(strm, path, "special_locs", t.special_locs);
	dump_to(strm, path, "special_id", t.special_id);

	dump_to(strm, path, "exit_locs", t.exit_locs);
	dump_to(strm, path, "exit_dests", t.exit_dests);
	dump_to(strm, path, "sign_locs", t.sign_locs);
	dump_to(strm, path, "wandering", t.wandering);
	dump_to(strm, path, "special_enc", t.special_enc);
	dump_to(strm, path, "wandering_locs", t.wandering_locs);
	dump_to(strm, path, "info_rect", t.info_rect);
	dump_to(strm, path, "strlens", t.strlens);
	dump_to(strm, path, "specials", t.specials);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const item_record_type& t)
{
	const auto path{ std::format("{}.{}", parent, item) };
	dump_to(strm, path, "variety", t.variety);
	dump_to(strm, path, "item_level", t.item_level);

	dump_to(strm, path, "awkward", t.awkward);
	dump_to(strm, path, "bonus", t.bonus);
	dump_to(strm, path, "protection", t.protection);
	dump_to(strm, path, "charges", t.charges);
	dump_to(strm, path, "type", t.type);
	dump_to(strm, path, "magic_use_type", t.magic_use_type);
	dump_to(strm, path, "graphic_num", t.graphic_num);
	dump_to(strm, path, "ability", t.ability);
	dump_to(strm, path, "ability_strength", t.ability_strength);
	dump_to(strm, path, "type_flag", t.type_flag);
	dump_to(strm, path, "is_special", t.is_special);
	dump_to(strm, path, "a", t.a);

	dump_to(strm, path, "value", t.value);

	dump_to(strm, path, "weight", t.weight);
	dump_to(strm, path, "special_class", t.special_class);
	dump_to(strm, path, "item_loc", t.item_loc);
	dump_to(strm, path, "full_name", t.full_name);
	dump_to(strm, path, "name", t.name);
	dump_to(strm, path, "treas_class", t.treas_class);
	dump_to(strm, path, "item_properties", t.item_properties);
	dump_to(strm, path, "reserved1", t.reserved1);
	dump_to(strm, path, "reserved2", t.reserved2);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const scen_item_data_type& t)
{
	const auto path{ std::format("{}.{}", parent, item) };
	dump_to(strm, path, "scen_items", t.scen_items);
	dump_to(strm, path, "monst_names", t.monst_names);
	dump_to(strm, path, "ter_names", t.ter_names);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const BoeRect& t)
{
	const auto path{ std::format("{}.{}", parent, item) };
	dump_to(strm, path, "left", (unsigned int)t.left);
	dump_to(strm, path, "top", (unsigned int)t.top);
	dump_to(strm, path, "right", (unsigned int)t.right);
	dump_to(strm, path, "bottom", (unsigned int)t.bottom);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const horse_record_type& t)
{
	const auto path{ std::format("{}.{}", parent, item) };
	dump_to(strm, path, "horse_loc", t.horse_loc);
	dump_to(strm, path, "horse_loc_in_sec", t.horse_loc_in_sec);
	dump_to(strm, path, "horse_sector", t.horse_sector);
	dump_to(strm, path, "which_town", t.which_town);
	dump_to(strm, path, "exists", t.exists);
	dump_to(strm, path, "property", t.property);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const boat_record_type& t)
{
	const auto path{ std::format("{}.{}", parent, item) };
	dump_to(strm, path, "boat_loc", t.boat_loc);
	dump_to(strm, path, "boat_loc_in_sec", t.boat_loc_in_sec);
	dump_to(strm, path, "boat_sector", t.boat_sector);
	dump_to(strm, path, "which_town", t.which_town);
	dump_to(strm, path, "exists", t.exists);
	dump_to(strm, path, "property", t.property);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const special_node_type& t)
{
	const auto path{ std::format("{}.{}", parent, item) };
	dump_to(strm, path, "type", t.type);
	dump_to(strm, path, "sd1", t.sd1);
	dump_to(strm, path, "sd2", t.sd2);
	dump_to(strm, path, "pic", t.pic);
	dump_to(strm, path, "m1", t.m1);
	dump_to(strm, path, "m2", t.m2);
	dump_to(strm, path, "ex1a", t.ex1a);
	dump_to(strm, path, "ex1b", t.ex1b);
	dump_to(strm, path, "ex2a", t.ex2a);
	dump_to(strm, path, "ex2b", t.ex2b);
	dump_to(strm, path, "jumpto", t.jumpto);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const item_storage_shortcut_type& t)
{
	const auto path{ std::format("{}.{}", parent, item) };
	dump_to(strm, path, "ter_type", t.ter_type);
	dump_to(strm, path, "item_num", t.item_num);
	dump_to(strm, path, "item_odds", t.item_odds);
	dump_to(strm, path, "property", t.property);
}



void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const terrain_type_type& t)
{
	const auto path{ std::format("{}.{}", parent, item) };
	dump_to(strm, path, "picture", t.picture);
	dump_to(strm, path, "blockage", t.blockage);
	dump_to(strm, path, "flag1", t.flag1);
	dump_to(strm, path, "flag2", t.flag2);
	dump_to(strm, path, "special", t.special);
	dump_to(strm, path, "trans_to_what", t.trans_to_what);
	dump_to(strm, path, "fly_over", t.fly_over);
	dump_to(strm, path, "boat_over", t.boat_over);
	dump_to(strm, path, "block_horse", t.block_horse);
	dump_to(strm, path, "light_radius", t.light_radius);
	dump_to(strm, path, "step_sound", t.step_sound);
	dump_to(strm, path, "shortcut_key", t.shortcut_key);
	dump_to(strm, path, "res1", t.res1);
	dump_to(strm, path, "res2", t.res2);
	dump_to(strm, path, "res3", t.res3);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const monster_record_type& t)
{
	const auto path{ std::format("{}.{}", parent, item) };
	dump_to(strm, path, "m_num", t.m_num);
	dump_to(strm, path, "level", t.level);
	dump_to(strm, path, "m_name", t.m_name);
	dump_to(strm, path, "health", t.health);
	dump_to(strm, path, "m_health", t.m_health);
	dump_to(strm, path, "mp", t.mp);
	dump_to(strm, path, "max_mp", t.max_mp);
	dump_to(strm, path, "armor", t.armor);
	dump_to(strm, path, "skill", t.skill);
	dump_to(strm, path, "a", t.a);
	dump_to(strm, path, "a1_type", t.a1_type);
	dump_to(strm, path, "a23_type", t.a23_type);
	dump_to(strm, path, "m_type", t.m_type);
	dump_to(strm, path, "speed", t.speed);
	dump_to(strm, path, "ap", t.ap);
	dump_to(strm, path, "mu", t.mu);
	dump_to(strm, path, "cl", t.cl);
	dump_to(strm, path, "breath", t.breath);
	dump_to(strm, path, "breath_type", t.breath_type);
	dump_to(strm, path, "treasure", t.treasure);
	dump_to(strm, path, "spec_skill", t.spec_skill);
	dump_to(strm, path, "poison", t.poison);
	dump_to(strm, path, "morale", t.morale);
	dump_to(strm, path, "m_morale", t.m_morale);
	dump_to(strm, path, "corpse_item", t.corpse_item);
	dump_to(strm, path, "corpse_item_chance", t.corpse_item_chance);
	dump_to(strm, path, "status", t.status);
	dump_to(strm, path, "direction", t.direction);
	dump_to(strm, path, "immunities", t.immunities);
	dump_to(strm, path, "x_width", t.x_width);
	dump_to(strm, path, "y_width", t.y_width);
	dump_to(strm, path, "radiate_1", t.radiate_1);
	dump_to(strm, path, "radiate_2", t.radiate_2);
	dump_to(strm, path, "default_attitude", t.default_attitude);
	dump_to(strm, path, "summon_type", t.summon_type);
	dump_to(strm, path, "default_facial_pic", t.default_facial_pic);
	dump_to(strm, path, "res1", t.res1);
	dump_to(strm, path, "res2", t.res2);
	dump_to(strm, path, "res3", t.res3);
	dump_to(strm, path, "picture_num", t.picture_num);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const scenario_data_type& t)
{
	const auto path{ std::format("{}.{}", parent, item) };
	dump_to(strm, path, "flag1", t.flag1);
	dump_to(strm, path, "flag2", t.flag2);
	dump_to(strm, path, "flag3", t.flag3);
	dump_to(strm, path, "flag4", t.flag4);
	dump_to(strm, path, "ver", t.ver);
	dump_to(strm, path, "min_run_ver", t.min_run_ver);
	dump_to(strm, path, "prog_make_ver", t.prog_make_ver);
	dump_to(strm, path, "num_towns", t.num_towns);
	dump_to(strm, path, "out_width", t.out_width);
	dump_to(strm, path, "out_height", t.out_height);
	dump_to(strm, path, "difficulty", t.difficulty);
	dump_to(strm, path, "intro_pic", t.intro_pic);
	dump_to(strm, path, "default_ground", t.default_ground);
	dump_to(strm, path, "town_size", t.town_size);
	dump_to(strm, path, "town_hidden", t.town_hidden);
	dump_to(strm, path, "a", t.a);
	dump_to(strm, path, "flag_a", t.flag_a);
	dump_to(strm, path, "intro_mess_pic", t.intro_mess_pic);
	dump_to(strm, path, "intro_mess_len", t.intro_mess_len);
	dump_to(strm, path, "where_start", t.where_start);
	dump_to(strm, path, "out_sec_start", t.out_sec_start);
	dump_to(strm, path, "out_start", t.out_start);
	dump_to(strm, path, "which_town_start", t.which_town_start);
	dump_to(strm, path, "flag_b", t.flag_b);
	dump_to(strm, path, "town_data_size", t.town_data_size);
	dump_to(strm, path, "town_to_add_to", t.town_to_add_to);
	dump_to(strm, path, "flag_to_add_to_town", t.flag_to_add_to_town);
	dump_to(strm, path, "flag_c", t.flag_c);
	dump_to(strm, path, "out_data_size", t.out_data_size);
	dump_to(strm, path, "store_item_rects", t.store_item_rects);
	dump_to(strm, path, "store_item_towns", t.store_item_towns);
	dump_to(strm, path, "flag_e", t.flag_e);
	dump_to(strm, path, "special_items", t.special_items);
	dump_to(strm, path, "special_item_special", t.special_item_special);
	dump_to(strm, path, "rating", t.rating);
	dump_to(strm, path, "uses_custom_graphics", t.uses_custom_graphics);
	dump_to(strm, path, "flag_f", t.flag_f);
	dump_to(strm, path, "scen_monsters", t.scen_monsters);
	dump_to(strm, path, "scen_boats", t.scen_boats);
	dump_to(strm, path, "scen_horses", t.scen_horses);
	dump_to(strm, path, "flag_g", t.flag_g);
	dump_to(strm, path, "ter_types", t.ter_types);
	dump_to(strm, path, "scenario_timer_times", t.scenario_timer_times);
	dump_to(strm, path, "scenario_timer_specs", t.scenario_timer_specs);
	dump_to(strm, path, "flag_h", t.flag_h);
	dump_to(strm, path, "scen_specials", t.scen_specials);
	dump_to(strm, path, "storage_shortcuts", t.storage_shortcuts);
	dump_to(strm, path, "flag_d", t.flag_d);
	dump_to(strm, path, "scen_str_len", t.scen_str_len);
	dump_to(strm, path, "flag_i", t.flag_i);
	dump_to(strm, path, "last_out_edited", t.last_out_edited);
	dump_to(strm, path, "last_town_edited", t.last_town_edited);
}


int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::println("Failed, I have :(");
		return EXIT_FAILURE;
	}
	std::ifstream input(argv[1], std::ios_base::in | std::ios_base::binary);
	std::println("scenario_data_type: {:x} {:d}", sizeof(scenario_data_type), sizeof(scenario_data_type));
	std::println("scen_item_data_type: {:x} {:d}", sizeof(scen_item_data_type), sizeof(scen_item_data_type));
	std::println("scenario_both: {:x} {:d}", sizeof(scenario_data_type) + sizeof(scen_item_data_type), sizeof(scenario_data_type) + sizeof(scen_item_data_type));
	std::println("outdoor_record_type: {:x} {:d}", sizeof(outdoor_record_type), sizeof(outdoor_record_type));
	std::endian scenario_endian{std::endian::native};
	scenario_data_type scenario;
	input.read(reinterpret_cast<char*>(&scenario), sizeof(scenario));
	if ((scenario.flag1 == 10) && (scenario.flag2 == 20)
		&& (scenario.flag3 == 30)
		&& (scenario.flag4 == 40))
	{
		std::println("scenario is big endian");
		scenario_endian = std::endian::big;
	}
	else if ((scenario.flag1 == 20) && (scenario.flag2 == 40)
		&& (scenario.flag3 == 60)
		&& (scenario.flag4 == 80))
	{
		std::println("scenario is little endian");
		scenario_endian = std::endian::little;
	}
	else
	{
		std::println("scenario flags not recognised");
		return EXIT_FAILURE;
	}
	auto strm = std::ostream_iterator<char>(std::cout);
	if(std::endian::native != scenario_endian)
	{
		port_scenario(scenario);
	}
	dump_to(strm, "", "scenario_data_type", scenario);

	scen_item_data_type scenario_items;
	input.read(reinterpret_cast<char*>(&scenario_items), sizeof(scenario_items));
	if (std::endian::native != scenario_endian)
	{
		port_item_list(scenario_items);
	}
	dump_to(strm, "", "scen_item_data_type", scenario_items);

	// Note that the original code only checks for 270
	std::vector<char> storage;
	for(size_t index = 0; index < 300; ++index)
	{
		const auto len = scenario.scen_str_len[index];
		storage.clear();
		storage.resize(len + 1, '\0');
		input.read(storage.data(), len);
		std::println("scen_str[{}]: ({}) \"{}\"", index, len, storage.data());
	}

	for(size_t outdoors_index = 0; outdoors_index < (scenario.out_width * scenario.out_height); ++outdoors_index)
	{
		std::println("Start position: {} {:x}", outdoors_index, static_cast<std::size_t>(input.tellg()));
		outdoor_record_type outdoors;
		input.read(reinterpret_cast<char*>(&outdoors), sizeof(outdoors));
		if (std::endian::native != scenario_endian)
		{
			port_out(outdoors);
		}
		//std::println("Output position: {:x}", static_cast<std::size_t>(input.tellg()));
		dump_to(strm, "", "outdoor_record_type", outdoors);
		const auto string_start_pos{ input.tellg() };
		std::vector<char> storage;
		for (size_t index = 0; index < std::size(outdoors.strlens); ++index)
		{
			const auto len = outdoors.strlens[index];
			storage.clear();
			storage.resize(len + 1, '\0');
			std::println("Current position: {:x}", static_cast<std::size_t>(input.tellg()));
			input.read(storage.data(), (unsigned int)len);
			std::println("scen_str[{}]: ({}) \"{}\"", index, len, storage.data());
		}
		const auto string_end_pos{ input.tellg() };
		std::println("String section size: {} {:x}", static_cast<std::size_t>(string_end_pos - string_start_pos), static_cast<std::size_t>(string_end_pos - string_start_pos));
		std::println("scenario.out_data_size: {} {:x}", scenario.out_data_size[outdoors_index][1], scenario.out_data_size[outdoors_index][1]);
		std::println("End position: {} {:x}", outdoors_index, static_cast<std::size_t>(string_end_pos));
	}
	std::println("Final position: {:x}", static_cast<std::size_t>(input.tellg()));

	return EXIT_SUCCESS;
}

#if 0
// 0x150e4 + 0xE134 = 0x00023218
// 23518 looked better
//load_spec_graphics();
//set_up_ter_pics();

4146 2586 }{ 4146 3633 } { 4146 2565 } { 4146 2023 } { 4146 3089 } { 4146 3109 } { 4146 4711 } { 4146 2768 } { 4146 1817

1032 0A1A	1A4C
1032 0E31	184B
1032 0A05	141F
1032 07E7	1201
1032 0C11	162B
1032 0C25	163F
1032 1267	1C81
1032 0AD0	14EA
1032 0719	1133



3210 0A1A
3210 0E31
3210 0A05
3210 07E7
3210 0C11
3210 0C25
3210 1267
3210 0AD0
3210 0719





1032


150e4






+1B02 =



B68C
C7BF

00000000	000150E4		Scenario and scenario item headers
150E4 Start of outdoor record type + 1032 = 16116 Start of strings for outdoor

16E30 Start of outdoor record type +


00016116	00016b30		16E30
00017b62	00018993		18C93
000199c5	0001a3ca		1A6CA
0001b3fc	0001bbe3		1BEE3
0001cc15	0001d826		1DB26
0001e858	0001f47d		1F77D
000204af	00021716		21A16
00022748	00023218		23518
24C63

a3d6
9920
13cf6
1032
#endif
