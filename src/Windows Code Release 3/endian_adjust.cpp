#include "endian_adjust.hpp"

#include <bit>

#if __cpp_lib_byteswap

// C++23
using std::byteswap;

#else

#include <array>
#include <algorithm>
#include <concepts>

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

void endian_adjust(short& value)
{
	value = byteswap(value);
}


static void endian_adjust(BoeRect& r)
{
	auto tmp = r.top;
	r.top = byteswap(r.left);
	r.left = byteswap(tmp);
	tmp = r.bottom;
	r.bottom = byteswap(r.right);
	r.right = byteswap(tmp);
}


static void endian_adjust(special_node_type& spec)
{
	endian_adjust(spec.type);
	endian_adjust(spec.sd1);
	endian_adjust(spec.sd2);
	endian_adjust(spec.pic);
	endian_adjust(spec.m1);
	endian_adjust(spec.m2);
	endian_adjust(spec.ex1a);
	endian_adjust(spec.ex1b);
	endian_adjust(spec.ex2a);
	endian_adjust(spec.ex2b);
	endian_adjust(spec.jumpto);
}


void endian_adjust(talking_record_type & talking)
{
	for (short i = 0; i < 60; i++)
	{
		endian_adjust(talking.talk_nodes[i].personality);
		endian_adjust(talking.talk_nodes[i].type);
		endian_adjust(talking.talk_nodes[i].extras[0]);
		endian_adjust(talking.talk_nodes[i].extras[1]);
		endian_adjust(talking.talk_nodes[i].extras[2]);
		endian_adjust(talking.talk_nodes[i].extras[3]);
	}
}


void endian_adjust(town_record_type & town)
{
	endian_adjust(town.town_chop_time);
	endian_adjust(town.town_chop_key);
	endian_adjust(town.lighting);
	for (short i = 0; i < 4; i++)
		endian_adjust(town.exit_specs[i]);
	endian_adjust(town.in_town_rect);
	for (short i = 0; i < 64; i++) {
		endian_adjust(town.preset_items[i].item_code);
		endian_adjust(town.preset_items[i].ability);
	}
	for (short i = 0; i < 50; i++) {
		endian_adjust(town.preset_fields[i].field_type);
	}
	endian_adjust(town.max_num_monst);
	endian_adjust(town.spec_on_entry);
	endian_adjust(town.spec_on_entry_if_dead);
	for (short i = 0; i < 8; i++)
		endian_adjust(town.timer_spec_times[i]);
	for (short i = 0; i < 8; i++)
		endian_adjust(town.timer_specs[i]);
	endian_adjust(town.difficulty);
	for (short i = 0; i < 100; i++)
		endian_adjust(town.specials[i]);
}


void endian_adjust(tiny_tr_type& t_d)
{
	for (short i = 0; i < 16; i++)
		endian_adjust(t_d.room_rect[i]);
	for (short i = 0; i < 30; i++) {
		endian_adjust(t_d.creatures[i].spec1);
		endian_adjust(t_d.creatures[i].spec2);
		endian_adjust(t_d.creatures[i].monster_time);
		endian_adjust(t_d.creatures[i].personality);
		endian_adjust(t_d.creatures[i].special_on_kill);
		endian_adjust(t_d.creatures[i].facial_pic);
	}
}


void endian_adjust(ave_tr_type& t_d)
{
	for (short i = 0; i < 16; i++)
		endian_adjust(t_d.room_rect[i]);
	for (short i = 0; i < 40; i++) {
		endian_adjust(t_d.creatures[i].spec1);
		endian_adjust(t_d.creatures[i].spec2);
		endian_adjust(t_d.creatures[i].monster_time);
		endian_adjust(t_d.creatures[i].personality);
		endian_adjust(t_d.creatures[i].special_on_kill);
		endian_adjust(t_d.creatures[i].facial_pic);
	}
}


void endian_adjust(big_tr_type & t_d)
{
	for (short i = 0; i < 16; i++)
		endian_adjust(t_d.room_rect[i]);
	for (short i = 0; i < 60; i++)
	{
		endian_adjust(t_d.creatures[i].spec1);
		endian_adjust(t_d.creatures[i].spec2);
		endian_adjust(t_d.creatures[i].monster_time);
		endian_adjust(t_d.creatures[i].personality);
		endian_adjust(t_d.creatures[i].special_on_kill);
		endian_adjust(t_d.creatures[i].facial_pic);

	}
}


void endian_adjust(scenario_data_type & scenario)
{
	endian_adjust(scenario.flag_a);
	endian_adjust(scenario.flag_b);
	endian_adjust(scenario.flag_c);
	endian_adjust(scenario.flag_d);
	endian_adjust(scenario.flag_e);
	endian_adjust(scenario.flag_f);
	endian_adjust(scenario.flag_g);
	endian_adjust(scenario.flag_h);
	endian_adjust(scenario.flag_i);
	endian_adjust(scenario.intro_mess_pic);
	endian_adjust(scenario.intro_mess_len);
	endian_adjust(scenario.which_town_start);
	for (short i = 0; i < 200; i++)
		for (short j = 0; j < 5; j++)
			endian_adjust(scenario.town_data_size[i][j]);
	for (short i = 0; i < 10; i++)
		endian_adjust(scenario.town_to_add_to[i]);
	for (short i = 0; i < 10; i++)
		for (short j = 0; j < 2; j++)
			endian_adjust(scenario.flag_to_add_to_town[i][j]);
	for (short i = 0; i < 100; i++)
		for (short j = 0; j < 2; j++)
			endian_adjust(scenario.out_data_size[i][j]);
	for (short i = 0; i < 3; i++)
		endian_adjust(scenario.store_item_rects[i]);
	for (short i = 0; i < 3; i++)
		endian_adjust(scenario.store_item_towns[i]);
	for (short i = 0; i < 50; i++)
		endian_adjust(scenario.special_items[i]);
	for (short i = 0; i < 50; i++)
		endian_adjust(scenario.special_item_special[i]);
	endian_adjust(scenario.rating);
	endian_adjust(scenario.uses_custom_graphics);
	for (short i = 0; i < 256; i++)
	{
		endian_adjust(scenario.scen_monsters[i].health);
		endian_adjust(scenario.scen_monsters[i].m_health);
		endian_adjust(scenario.scen_monsters[i].max_mp);
		endian_adjust(scenario.scen_monsters[i].mp);
		endian_adjust(scenario.scen_monsters[i].a[1]);
		endian_adjust(scenario.scen_monsters[i].a[0]);
		endian_adjust(scenario.scen_monsters[i].a[2]);
		endian_adjust(scenario.scen_monsters[i].morale);
		endian_adjust(scenario.scen_monsters[i].m_morale);
		endian_adjust(scenario.scen_monsters[i].corpse_item);
		endian_adjust(scenario.scen_monsters[i].corpse_item_chance);
		endian_adjust(scenario.scen_monsters[i].picture_num);
	}

	for (short i = 0; i < 256; i++) {
		endian_adjust(scenario.ter_types[i].picture);
	}
	for (short i = 0; i < 30; i++) {
		endian_adjust(scenario.scen_boats[i].which_town);
	}
	for (short i = 0; i < 30; i++) {
		endian_adjust(scenario.scen_horses[i].which_town);
	}
	for (short i = 0; i < 20; i++)
		endian_adjust(scenario.scenario_timer_times[i]);
	for (short i = 0; i < 20; i++)
		endian_adjust(scenario.scenario_timer_specs[i]);
	for (short i = 0; i < 256; i++) {
		endian_adjust(scenario.scen_specials[i]);
	}
	for (short i = 0; i < 10; i++) {
		endian_adjust(scenario.storage_shortcuts[i].ter_type);
		endian_adjust(scenario.storage_shortcuts[i].property);
		for (short j = 0; j < 10; j++) {
			endian_adjust(scenario.storage_shortcuts[i].item_num[j]);
			endian_adjust(scenario.storage_shortcuts[i].item_odds[j]);
		}
	}
	endian_adjust(scenario.last_town_edited);
}


void endian_adjust(scen_item_data_type& scen_item_list)
{
	for (short i = 0; i < 400; i++) {
		endian_adjust(scen_item_list.scen_items[i].variety);
		endian_adjust(scen_item_list.scen_items[i].item_level);
		endian_adjust(scen_item_list.scen_items[i].value);
	}
}


void endian_adjust(outdoor_record_type& out)
{
	for (short i = 0; i < 4; i++)
	{
		endian_adjust(out.wandering[i].spec_on_meet);
		endian_adjust(out.wandering[i].spec_on_win);
		endian_adjust(out.wandering[i].spec_on_flee);
		endian_adjust(out.wandering[i].cant_flee);
		endian_adjust(out.wandering[i].end_spec1);
		endian_adjust(out.wandering[i].end_spec2);
		endian_adjust(out.special_enc[i].spec_on_meet);
		endian_adjust(out.special_enc[i].spec_on_win);
		endian_adjust(out.special_enc[i].spec_on_flee);
		endian_adjust(out.special_enc[i].cant_flee);
		endian_adjust(out.special_enc[i].end_spec1);
		endian_adjust(out.special_enc[i].end_spec2);
	}
	for (short i = 0; i < 8; i++)
		endian_adjust(out.info_rect[i]);
	for (short i = 0; i < 60; i++)
		endian_adjust(out.specials[i]);
}
