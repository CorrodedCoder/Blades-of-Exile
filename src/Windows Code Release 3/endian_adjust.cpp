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

template< typename T, size_t N >
static void endian_adjust(T(&arr)[N])
{
	for (auto& item : arr)
	{
		endian_adjust(item);
	}
}

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

static void endian_adjust(talking_node_type& talk_node)
{
	endian_adjust(talk_node.personality);
	endian_adjust(talk_node.type);
	endian_adjust(talk_node.extras[0]);
	endian_adjust(talk_node.extras[1]);
	endian_adjust(talk_node.extras[2]);
	endian_adjust(talk_node.extras[3]);
}

void endian_adjust(talking_record_type & talking)
{
	endian_adjust(talking.talk_nodes);
}

static void endian_adjust(preset_item_type& item)
{
	endian_adjust(item.item_code);
	endian_adjust(item.ability);
}

static void endian_adjust(preset_field_type& field)
{
	endian_adjust(field.field_type);
}

void endian_adjust(town_record_type & town)
{
	endian_adjust(town.town_chop_time);
	endian_adjust(town.town_chop_key);
	endian_adjust(town.lighting);
	endian_adjust(town.exit_specs);
	endian_adjust(town.in_town_rect);
	endian_adjust(town.preset_items);
	endian_adjust(town.preset_fields);
	endian_adjust(town.max_num_monst);
	endian_adjust(town.spec_on_entry);
	endian_adjust(town.spec_on_entry_if_dead);
	endian_adjust(town.timer_spec_times);
	endian_adjust(town.timer_specs);
	endian_adjust(town.difficulty);
	endian_adjust(town.specials);
}

static void endian_adjust(creature_start_type& creature)
{
	endian_adjust(creature.spec1);
	endian_adjust(creature.spec2);
	endian_adjust(creature.monster_time);
	endian_adjust(creature.personality);
	endian_adjust(creature.special_on_kill);
	endian_adjust(creature.facial_pic);
}

void endian_adjust(tiny_tr_type& t_d)
{
	endian_adjust(t_d.room_rect);
	endian_adjust(t_d.creatures);
}

void endian_adjust(ave_tr_type& t_d)
{
	endian_adjust(t_d.room_rect);
	endian_adjust(t_d.creatures);
}

void endian_adjust(big_tr_type & t_d)
{
	endian_adjust(t_d.room_rect);
	endian_adjust(t_d.creatures);
}

static void endian_adjust(monster_record_type& monster)
{
	endian_adjust(monster.health);
	endian_adjust(monster.m_health);
	endian_adjust(monster.max_mp);
	endian_adjust(monster.mp);
	endian_adjust(monster.a[1]);
	endian_adjust(monster.a[0]);
	endian_adjust(monster.a[2]);
	endian_adjust(monster.morale);
	endian_adjust(monster.m_morale);
	endian_adjust(monster.corpse_item);
	endian_adjust(monster.corpse_item_chance);
	endian_adjust(monster.picture_num);
}

static void endian_adjust(terrain_type_type& terrain)
{
	endian_adjust(terrain.picture);
}

static void endian_adjust(boat_record_type& boat)
{
	endian_adjust(boat.which_town);
}

static void endian_adjust(horse_record_type& horse)
{
	endian_adjust(horse.which_town);
}

static void endian_adjust(item_storage_shortcut_type& item)
{
	endian_adjust(item.ter_type);
	endian_adjust(item.property);
	endian_adjust(item.item_num);
	endian_adjust(item.item_odds);
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
	endian_adjust(scenario.town_data_size);
	endian_adjust(scenario.town_to_add_to);
	endian_adjust(scenario.flag_to_add_to_town);
	endian_adjust(scenario.out_data_size);
	endian_adjust(scenario.store_item_rects);
	endian_adjust(scenario.store_item_towns);
	endian_adjust(scenario.special_items);
	endian_adjust(scenario.special_item_special);
	endian_adjust(scenario.rating);
	endian_adjust(scenario.uses_custom_graphics);
	endian_adjust(scenario.scen_monsters);
	endian_adjust(scenario.ter_types);
	endian_adjust(scenario.scen_boats);
	endian_adjust(scenario.scen_horses);
	endian_adjust(scenario.scenario_timer_times);
	endian_adjust(scenario.scenario_timer_specs);
	endian_adjust(scenario.scen_specials);
	endian_adjust(scenario.storage_shortcuts);
	endian_adjust(scenario.last_town_edited);
}

static void endian_adjust(item_record_type& item)
{
	endian_adjust(item.variety);
	endian_adjust(item.item_level);
	endian_adjust(item.value);
}

void endian_adjust(scen_item_data_type& scen_item_list)
{
	endian_adjust(scen_item_list.scen_items);
}

static void endian_adjust(out_wandering_type& out)
{
	endian_adjust(out.spec_on_meet);
	endian_adjust(out.spec_on_win);
	endian_adjust(out.spec_on_flee);
	endian_adjust(out.cant_flee);
	endian_adjust(out.end_spec1);
	endian_adjust(out.end_spec2);
}

void endian_adjust(outdoor_record_type& out)
{
	endian_adjust(out.wandering);
	endian_adjust(out.special_enc);
	endian_adjust(out.info_rect);
	endian_adjust(out.specials);
}
