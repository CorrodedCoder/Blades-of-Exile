#include "boe/endian_adjust.hpp"

#include <bit>
#include <utility>
#include <cstddef>

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

static void endian_adjust(auto &&... args)
{
	(endian_adjust(std::forward<decltype(args)>(args)), ...);
}

static void endian_adjust(special_node_type& spec)
{
	endian_adjust(
		spec.type,
		spec.sd1,
		spec.sd2,
		spec.pic,
		spec.m1,
		spec.m2,
		spec.ex1a,
		spec.ex1b,
		spec.ex2a,
		spec.ex2b,
		spec.jumpto
	);
}

static void endian_adjust(talking_node_type& talk_node)
{
	endian_adjust(
		talk_node.personality,
		talk_node.type,
		talk_node.extras[0],
		talk_node.extras[1],
		talk_node.extras[2],
		talk_node.extras[3]
		);
}

void endian_adjust(talking_record_type & talking)
{
	endian_adjust(talking.talk_nodes);
}

static void endian_adjust(preset_item_type& item)
{
	endian_adjust(
		item.item_code,
		item.ability
		);
}

static void endian_adjust(preset_field_type& field)
{
	endian_adjust(field.field_type);
}

void endian_adjust(town_record_type & town)
{
	endian_adjust(
		town.town_chop_time,
		town.town_chop_key,
		town.lighting,
		town.exit_specs,
		town.in_town_rect,
		town.preset_items,
		town.preset_fields,
		town.max_num_monst,
		town.spec_on_entry,
		town.spec_on_entry_if_dead,
		town.timer_spec_times,
		town.timer_specs,
		town.difficulty,
		town.specials
	);
}

static void endian_adjust(creature_start_type& creature)
{
	endian_adjust(
		creature.spec1,
		creature.spec2,
		creature.monster_time,
		creature.personality,
		creature.special_on_kill,
		creature.facial_pic
	);
}

void endian_adjust(tiny_tr_type& t_d)
{
	endian_adjust(
		t_d.room_rect,
		t_d.creatures
	);
}

void endian_adjust(ave_tr_type& t_d)
{
	endian_adjust(
		t_d.room_rect,
		t_d.creatures
	);
}

void endian_adjust(big_tr_type & t_d)
{
	endian_adjust(
		t_d.room_rect,
		t_d.creatures
	);
}

static void endian_adjust(monster_record_type& monster)
{
	endian_adjust(
		monster.health,
		monster.m_health,
		monster.max_mp,
		monster.mp,
		monster.a[1],
		monster.a[0],
		monster.a[2],
		monster.morale,
		monster.m_morale,
		monster.corpse_item,
		monster.corpse_item_chance,
		monster.picture_num
		);
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
	endian_adjust(
		item.ter_type,
		item.property,
		item.item_num,
		item.item_odds
	);
}

void endian_adjust(scenario_data_type & scenario)
{
	endian_adjust(
		scenario.flag_a,
		scenario.flag_b,
		scenario.flag_c,
		scenario.flag_d,
		scenario.flag_e,
		scenario.flag_f,
		scenario.flag_g,
		scenario.flag_h,
		scenario.flag_i,
		scenario.intro_mess_pic,
		scenario.intro_mess_len,
		scenario.which_town_start,
		scenario.town_data_size,
		scenario.town_to_add_to,
		scenario.flag_to_add_to_town,
		scenario.out_data_size,
		scenario.store_item_rects,
		scenario.store_item_towns,
		scenario.special_items,
		scenario.special_item_special,
		scenario.rating,
		scenario.uses_custom_graphics,
		scenario.scen_monsters,
		scenario.ter_types,
		scenario.scen_boats,
		scenario.scen_horses,
		scenario.scenario_timer_times,
		scenario.scenario_timer_specs,
		scenario.scen_specials,
		scenario.storage_shortcuts,
		scenario.last_town_edited
		);
}

static void endian_adjust(item_record_type& item)
{
	endian_adjust(
		item.variety,
		item.item_level,
		item.value
	);
}

void endian_adjust(scen_item_data_type& scen_item_list)
{
	endian_adjust(scen_item_list.scen_items);
}

static void endian_adjust(out_wandering_type& out)
{
	endian_adjust(
		out.spec_on_meet,
		out.spec_on_win,
		out.spec_on_flee,
		out.cant_flee,
		out.end_spec1,
		out.end_spec2
	);
}

void endian_adjust(outdoor_record_type& out)
{
	endian_adjust(
		out.wandering,
		out.special_enc,
		out.info_rect,
		out.specials
	);
}
