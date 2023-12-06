#include "endian_adjust.hpp"

#include <bit>
#include <array>
#include <algorithm>
#include <concepts>
#include <cassert>

#define BOE_GLOBALS_SKIP_FUNCTIONS
#include "../global.h"

#if __cpp_lib_byteswap

// C++23
using std::byteswap;

#else

// Pre C++23
template<std::integral T>
constexpr T byteswap(T value) noexcept
{
	static_assert(std::has_unique_object_representations_v<T>,
		"T may not have padding bits");
	auto value_representation = std::bit_cast<std::array<std::byte, sizeof(T)>>(value);
	std::ranges::reverse(value_representation);
	return std::bit_cast<T>(value_representation);
}

#endif

static void flip_short(short& s)
{
	s = byteswap(s);
}


static void flip_rect(BoeRect& r)
{
	auto tmp = r.top;
	r.top = byteswap(r.left);
	r.left = byteswap(tmp);
	tmp = r.bottom;
	r.bottom = byteswap(r.right);
	r.right = byteswap(tmp);
}

void port_talk_nodes(talking_record_type & talking)
{
	for (short i = 0; i < 60; i++)
	{
		flip_short(talking.talk_nodes[i].personality);
		flip_short(talking.talk_nodes[i].type);
		flip_short(talking.talk_nodes[i].extras[0]);
		flip_short(talking.talk_nodes[i].extras[1]);
		flip_short(talking.talk_nodes[i].extras[2]);
		flip_short(talking.talk_nodes[i].extras[3]);
	}
}

static void flip_spec_node(special_node_type& spec)
{
	flip_short(spec.type);
	flip_short(spec.sd1);
	flip_short(spec.sd2);
	flip_short(spec.pic);
	flip_short(spec.m1);
	flip_short(spec.m2);
	flip_short(spec.ex1a);
	flip_short(spec.ex1b);
	flip_short(spec.ex2a);
	flip_short(spec.ex2b);
	flip_short(spec.jumpto);
}

void port_town(current_town_type & c_town)
{
	flip_short(c_town.town.town_chop_time);
	flip_short(c_town.town.town_chop_key);
	flip_short(c_town.town.lighting);
	for (short i = 0; i < 4; i++)
		flip_short(c_town.town.exit_specs[i]);
	flip_rect(c_town.town.in_town_rect);
	for (short i = 0; i < 64; i++) {
		flip_short(c_town.town.preset_items[i].item_code);
		flip_short(c_town.town.preset_items[i].ability);
	}
	for (short i = 0; i < 50; i++) {
		flip_short(c_town.town.preset_fields[i].field_type);
	}
	flip_short(c_town.town.max_num_monst);
	flip_short(c_town.town.spec_on_entry);
	flip_short(c_town.town.spec_on_entry_if_dead);
	for (short i = 0; i < 8; i++)
		flip_short(c_town.town.timer_spec_times[i]);
	for (short i = 0; i < 8; i++)
		flip_short(c_town.town.timer_specs[i]);
	flip_short(c_town.town.difficulty);
	for (short i = 0; i < 100; i++)
		flip_spec_node(c_town.town.specials[i]);
}


void port_t_d(big_tr_type & t_d)
{
	for (short i = 0; i < 16; i++)
		flip_rect(t_d.room_rect[i]);
	for (short i = 0; i < 60; i++)
	{
		flip_short(t_d.creatures[i].spec1);
		flip_short(t_d.creatures[i].spec2);
		flip_short(t_d.creatures[i].monster_time);
		flip_short(t_d.creatures[i].personality);
		flip_short(t_d.creatures[i].special_on_kill);
		flip_short(t_d.creatures[i].facial_pic);

	}
}

void port_scenario(scenario_data_type & scenario)
{
	flip_short(scenario.flag_a);
	flip_short(scenario.flag_b);
	flip_short(scenario.flag_c);
	flip_short(scenario.flag_d);
	flip_short(scenario.flag_e);
	flip_short(scenario.flag_f);
	flip_short(scenario.flag_g);
	flip_short(scenario.flag_h);
	flip_short(scenario.flag_i);
	flip_short(scenario.intro_mess_pic);
	flip_short(scenario.intro_mess_len);
	flip_short(scenario.which_town_start);
	for (short i = 0; i < 200; i++)
		for (short j = 0; j < 5; j++)
			flip_short(scenario.town_data_size[i][j]);
	for (short i = 0; i < 10; i++)
		flip_short(scenario.town_to_add_to[i]);
	for (short i = 0; i < 10; i++)
		for (short j = 0; j < 2; j++)
			flip_short(scenario.flag_to_add_to_town[i][j]);
	for (short i = 0; i < 100; i++)
		for (short j = 0; j < 2; j++)
			flip_short(scenario.out_data_size[i][j]);
	for (short i = 0; i < 3; i++)
		flip_rect(scenario.store_item_rects[i]);
	for (short i = 0; i < 3; i++)
		flip_short(scenario.store_item_towns[i]);
	for (short i = 0; i < 50; i++)
		flip_short(scenario.special_items[i]);
	for (short i = 0; i < 50; i++)
		flip_short(scenario.special_item_special[i]);
	flip_short(scenario.rating);
	flip_short(scenario.uses_custom_graphics);
	for (short i = 0; i < 256; i++)
	{
		flip_short(scenario.scen_monsters[i].health);
		flip_short(scenario.scen_monsters[i].m_health);
		flip_short(scenario.scen_monsters[i].max_mp);
		flip_short(scenario.scen_monsters[i].mp);
		flip_short(scenario.scen_monsters[i].a[1]);
		flip_short(scenario.scen_monsters[i].a[0]);
		flip_short(scenario.scen_monsters[i].a[2]);
		flip_short(scenario.scen_monsters[i].morale);
		flip_short(scenario.scen_monsters[i].m_morale);
		flip_short(scenario.scen_monsters[i].corpse_item);
		flip_short(scenario.scen_monsters[i].corpse_item_chance);
		flip_short(scenario.scen_monsters[i].picture_num);
	}

	for (short i = 0; i < 256; i++) {
		flip_short(scenario.ter_types[i].picture);
	}
	for (short i = 0; i < 30; i++) {
		flip_short(scenario.scen_boats[i].which_town);
	}
	for (short i = 0; i < 30; i++) {
		flip_short(scenario.scen_horses[i].which_town);
	}
	for (short i = 0; i < 20; i++)
		flip_short(scenario.scenario_timer_times[i]);
	for (short i = 0; i < 20; i++)
		flip_short(scenario.scenario_timer_specs[i]);
	for (short i = 0; i < 256; i++) {
		flip_spec_node(scenario.scen_specials[i]);
	}
	for (short i = 0; i < 10; i++) {
		flip_short(scenario.storage_shortcuts[i].ter_type);
		flip_short(scenario.storage_shortcuts[i].property);
		for (short j = 0; j < 10; j++) {
			flip_short(scenario.storage_shortcuts[i].item_num[j]);
			flip_short(scenario.storage_shortcuts[i].item_odds[j]);
		}
	}
	flip_short(scenario.last_town_edited);
}


void port_item_list(scen_item_data_type& scen_item_list)
{
	for (short i = 0; i < 400; i++) {
		flip_short(scen_item_list.scen_items[i].variety);
		flip_short(scen_item_list.scen_items[i].item_level);
		flip_short(scen_item_list.scen_items[i].value);
	}
}

void port_out(outdoor_record_type& out)
{
	for (short i = 0; i < 4; i++)
	{
		flip_short(out.wandering[i].spec_on_meet);
		flip_short(out.wandering[i].spec_on_win);
		flip_short(out.wandering[i].spec_on_flee);
		flip_short(out.wandering[i].cant_flee);
		flip_short(out.wandering[i].end_spec1);
		flip_short(out.wandering[i].end_spec2);
		flip_short(out.special_enc[i].spec_on_meet);
		flip_short(out.special_enc[i].spec_on_win);
		flip_short(out.special_enc[i].spec_on_flee);
		flip_short(out.special_enc[i].cant_flee);
		flip_short(out.special_enc[i].end_spec1);
		flip_short(out.special_enc[i].end_spec2);
	}
	for (short i = 0; i < 8; i++)
		flip_rect(out.info_rect[i]);
	for (short i = 0; i < 60; i++)
		flip_spec_node(out.specials[i]);
}
