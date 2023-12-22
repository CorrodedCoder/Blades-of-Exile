#include "boe/dump_game_structures.hpp"
#include <format>
#include <ranges>
#include <algorithm>

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const char& t)
{
	std::format_to(strm, "{}.{}={:d} ({:x})\n", parent, item, t, t);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const unsigned char& t)
{
	std::format_to(strm, "{}.{}={:d} ({:x})\n", parent, item, t, t);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const short& t)
{
	std::format_to(strm, "{}.{}={:d} ({:x})\n", parent, item, t, t);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const int& t)
{
	std::format_to(strm, "{}.{}={:d} ({:x})\n", parent, item, t, t);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const unsigned int& t)
{
	std::format_to(strm, "{}.{}={:d} ({:x})\n", parent, item, t, t);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const long& t)
{
	std::format_to(strm, "{}.{}={:d} ({:x})\n", parent, item, t, t);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const unsigned long& t)
{
	std::format_to(strm, "{}.{}={:d} ({:x})\n", parent, item, t, t);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const char* t, size_t size)
{
	const std::span text(t, t + size);
	const auto first_not_printable = std::find_if(std::begin(text), std::end(text), [](auto c) { return !isprint(static_cast<int>(static_cast<unsigned char>(c))); });
	std::format_to(strm, "{}.{}=\"", parent, item);
	std::copy(std::begin(text), first_not_printable, strm);
	std::format_to(strm, "\"");
	if (std::find_if(first_not_printable, std::end(text), [](auto c) { return c != 0; }) != std::end(text))
	{
		for (const auto c : std::ranges::subrange(first_not_printable, std::end(text)))
		{
			std::format_to(strm, "({:d})", c);
		}
	}
	std::format_to(strm, "\n");
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const unsigned char* t, size_t size)
{
	std::format_to(strm, "{}.{}={{", parent, item);
	std::format_to(strm, "{:02X}", t[0]);
	for (size_t index = 1; index < size; ++index)
	{
		std::format_to(strm, " {:02X}", t[index]);
	}
	std::format_to(strm, "}};\n");
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const status& t)
{
#if defined(__cpp_lib_to_underlying) && __cpp_lib_to_underlying
	dump_to(strm, parent, item, std::to_underlying(t));
#else
	dump_to(strm, parent, item, static_cast<short>(t));
#endif
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
	dump_to(strm, path, "left", static_cast<unsigned int>(t.left));
	dump_to(strm, path, "top", static_cast<unsigned int>(t.top));
	dump_to(strm, path, "right", static_cast<unsigned int>(t.right));
	dump_to(strm, path, "bottom", static_cast<unsigned int>(t.bottom));
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
	dump_to(strm, path, "status", t.mstatus);
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

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const talk_save_type& t)
{
	const auto path{ std::format("{}.{}", parent, item) };
	dump_to(strm, path, "personality", t.personality);
	dump_to(strm, path, "town_num", t.town_num);
	dump_to(strm, path, "str1", t.str1);
	dump_to(strm, path, "str2", t.str2);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const outdoor_creature_type& t)
{
	const auto path{ std::format("{}.{}", parent, item) };
	dump_to(strm, path, "exists", t.exists);
	dump_to(strm, path, "direction", t.direction);
	dump_to(strm, path, "what_monst", t.what_monst);
	dump_to(strm, path, "which_sector", t.which_sector);
	dump_to(strm, path, "m_loc", t.m_loc);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const creature_start_type& t)
{
	const auto path{ std::format("{}.{}", parent, item) };
	dump_to(strm, path, "number", t.number);
	dump_to(strm, path, "start_attitude", t.start_attitude);
	dump_to(strm, path, "start_loc", t.start_loc);
	dump_to(strm, path, "mobile", t.mobile);
	dump_to(strm, path, "time_flag", t.time_flag);
	dump_to(strm, path, "extra1", t.extra1);
	dump_to(strm, path, "extra2", t.extra2);
	dump_to(strm, path, "spec1", t.spec1);
	dump_to(strm, path, "spec2", t.spec2);
	dump_to(strm, path, "spec_enc_code", t.spec_enc_code);
	dump_to(strm, path, "time_code", t.time_code);
	dump_to(strm, path, "monster_time", t.monster_time);
	dump_to(strm, path, "personality", t.personality);
	dump_to(strm, path, "special_on_kill", t.special_on_kill);
	dump_to(strm, path, "facial_pic", t.facial_pic);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const creature_data_type& t)
{
	const auto path{ std::format("{}.{}", parent, item) };
	dump_to(strm, path, "active", t.active);
	dump_to(strm, path, "attitude", t.attitude);
	dump_to(strm, path, "number", t.number);
	dump_to(strm, path, "m_loc", t.m_loc);
	dump_to(strm, path, "m_d", t.m_d);
	dump_to(strm, path, "mobile", t.mobile);
	dump_to(strm, path, "summoned", t.summoned);
	dump_to(strm, path, "monst_start", t.monst_start);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const creature_list_type& t)
{
	const auto path{ std::format("{}.{}", parent, item) };
	dump_to(strm, path, "dudes", t.dudes);
	dump_to(strm, path, "which_town", t.which_town);
	dump_to(strm, path, "friendly", t.friendly);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const party_record_type& t)
{
	const auto path{ std::format("{}.{}", parent, item) };
	dump_to(strm, path, "age", t.age);
	dump_to(strm, path, "gold", t.gold);
	dump_to(strm, path, "food", t.food);
	dump_to(strm, path, "stuff_done", t.stuff_done);
	dump_to(strm, path, "item_taken", t.item_taken);
	dump_to(strm, path, "light_level", t.light_level);
	dump_to(strm, path, "outdoor_corner", t.outdoor_corner);
	dump_to(strm, path, "i_w_c", t.i_w_c);
	dump_to(strm, path, "p_loc", t.p_loc);
	dump_to(strm, path, "loc_in_sec", t.loc_in_sec);
	dump_to(strm, path, "boats", t.boats);
	dump_to(strm, path, "horses", t.horses);
	dump_to(strm, path, "creature_save", t.creature_save);
	dump_to(strm, path, "in_boat", t.in_boat);
	dump_to(strm, path, "in_horse", t.in_horse);
	dump_to(strm, path, "out_c", t.out_c);
	dump_to(strm, path, "magic_store_items", t.magic_store_items);
	dump_to(strm, path, "imprisoned_monst", t.imprisoned_monst);
	dump_to(strm, path, "m_seen", t.m_seen);
	dump_to(strm, path, "journal_str", t.journal_str);
	dump_to(strm, path, "journal_day", t.journal_day);
	dump_to(strm, path, "special_notes_str", t.special_notes_str);
	dump_to(strm, path, "talk_save", t.talk_save);
	dump_to(strm, path, "direction", t.direction);
	dump_to(strm, path, "at_which_save_slot", t.at_which_save_slot);
	dump_to(strm, path, "alchemy", t.alchemy);
	dump_to(strm, path, "can_find_town", t.can_find_town);
	dump_to(strm, path, "key_times", t.key_times);
	dump_to(strm, path, "party_event_timers", t.party_event_timers);
	dump_to(strm, path, "global_or_town", t.global_or_town);
	dump_to(strm, path, "node_to_call", t.node_to_call);
	dump_to(strm, path, "spec_items", t.spec_items);
	dump_to(strm, path, "help_received", t.help_received);
	dump_to(strm, path, "m_killed", t.m_killed);

	dump_to(strm, path, "total_m_killed", t.total_m_killed);
	dump_to(strm, path, "total_dam_done", t.total_dam_done);
	dump_to(strm, path, "total_xp_gained", t.total_xp_gained);
	dump_to(strm, path, "total_dam_taken", t.total_dam_taken);
	dump_to(strm, path, "scen_name", t.scen_name);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const setup_save_type& t)
{
	const auto path{ std::format("{}.{}", parent, item) };
	dump_to(strm, path, "setup", t.setup);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const pc_record_type& t)
{
	const auto path{ std::format("{}.{}", parent, item) };
	dump_to(strm, path, "main_status", t.main_status);
	dump_to(strm, path, "name", t.name);
	dump_to(strm, path, "skills", t.skills);
	dump_to(strm, path, "max_health", t.max_health);
	dump_to(strm, path, "cur_health", t.cur_health);
	dump_to(strm, path, "max_sp", t.max_sp);
	dump_to(strm, path, "cur_sp", t.cur_sp);
	dump_to(strm, path, "experience", t.experience);
	dump_to(strm, path, "skill_pts", t.skill_pts);
	dump_to(strm, path, "level", t.level);
	dump_to(strm, path, "status", t.status);
	dump_to(strm, path, "items", t.items);
	dump_to(strm, path, "equip", t.equip);
	dump_to(strm, path, "priest_spells", t.priest_spells);
	dump_to(strm, path, "mage_spells", t.mage_spells);
	dump_to(strm, path, "which_graphic", t.which_graphic);
	dump_to(strm, path, "weap_poisoned", t.weap_poisoned);
	dump_to(strm, path, "advan", t.advan);
	dump_to(strm, path, "traits", t.traits);
	dump_to(strm, path, "race", t.race);
	dump_to(strm, path, "exp_adj", t.exp_adj);
	dump_to(strm, path, "direction", t.direction);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const preset_item_type& t)
{
	const auto path{ std::format("{}.{}", parent, item) };
	dump_to(strm, path, "item_loc", t.item_loc);
	dump_to(strm, path, "item_code", t.item_code);
	dump_to(strm, path, "ability", t.ability);
	dump_to(strm, path, "charges", t.charges);
	dump_to(strm, path, "always_there", t.always_there);
	dump_to(strm, path, "property", t.property);
	dump_to(strm, path, "contained", t.contained);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const preset_field_type& t)
{
	const auto path{ std::format("{}.{}", parent, item) };
	dump_to(strm, path, "field_loc", t.field_loc);
	dump_to(strm, path, "field_type", t.field_type);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const wandering_type& t)
{
	const auto path{ std::format("{}.{}", parent, item) };
	dump_to(strm, path, "monst", t.monst);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const town_record_type& t)
{
	const auto path{ std::format("{}.{}", parent, item) };
	dump_to(strm, path, "town_chop_time", t.town_chop_time);
	dump_to(strm, path, "town_chop_key", t.town_chop_key);
	dump_to(strm, path, "wandering", t.wandering);
	dump_to(strm, path, "wandering_locs", t.wandering_locs);
	dump_to(strm, path, "special_locs", t.special_locs);
	dump_to(strm, path, "spec_id", t.spec_id);
	dump_to(strm, path, "sign_locs", t.sign_locs);
	dump_to(strm, path, "lighting", t.lighting);
	dump_to(strm, path, "start_locs", t.start_locs);
	dump_to(strm, path, "exit_locs", t.exit_locs);
	dump_to(strm, path, "exit_specs", t.exit_specs);
	dump_to(strm, path, "in_town_rect", t.in_town_rect);
	dump_to(strm, path, "preset_items", t.preset_items);
	dump_to(strm, path, "max_num_monst", t.max_num_monst);
	dump_to(strm, path, "preset_fields", t.preset_fields);
	dump_to(strm, path, "spec_on_entry", t.spec_on_entry);
	dump_to(strm, path, "spec_on_entry_if_dead", t.spec_on_entry_if_dead);
	dump_to(strm, path, "timer_spec_times", t.timer_spec_times);
	dump_to(strm, path, "timer_specs", t.timer_specs);
	dump_to(strm, path, "strlens", t.strlens);
	dump_to(strm, path, "specials", t.specials);
	dump_to(strm, path, "specials1", t.specials1);
	dump_to(strm, path, "specials2", t.specials2);
	dump_to(strm, path, "res1", t.res1);
	dump_to(strm, path, "res2", t.res2);
	dump_to(strm, path, "difficulty", t.difficulty);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const current_town_type& t)
{
	const auto path{ std::format("{}.{}", parent, item) };
	dump_to(strm, path, "town_num", t.town_num);
	dump_to(strm, path, "difficulty", t.difficulty);
	dump_to(strm, path, "town", t.town);
	dump_to(strm, path, "explored", t.explored);
	dump_to(strm, path, "hostile", t.hostile);
	dump_to(strm, path, "monst", t.monst);
	dump_to(strm, path, "in_boat", t.in_boat);
	dump_to(strm, path, "p_loc", t.p_loc);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const big_tr_type& t)
{
	const auto path{ std::format("{}.{}", parent, item) };
	dump_to(strm, path, "terrain", t.terrain);
	dump_to(strm, path, "room_rect", t.room_rect);
	dump_to(strm, path, "creatures", t.creatures);
	dump_to(strm, path, "lighting", t.lighting);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const town_item_list& t)
{
	const auto path{ std::format("{}.{}", parent, item) };
	dump_to(strm, path, "items", t.items);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const stored_items_list_type& t)
{
	const auto path{ std::format("{}.{}", parent, item) };
	dump_to(strm, path, "items", t.items);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const stored_town_maps_type& t)
{
	const auto path{ std::format("{}.{}", parent, item) };
	dump_to(strm, path, "town_maps", t.town_maps);
}

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const stored_outdoor_maps_type& t)
{
	const auto path{ std::format("{}.{}", parent, item) };
	dump_to(strm, path, "outdoor_maps", t.outdoor_maps);
}
