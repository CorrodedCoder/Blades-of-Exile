#include "boe/pc.hpp"
#include <algorithm>
#include <ranges>
#include <array>
#include <string_view>

namespace
{
	const std::array c_debug_names{
		"Gunther",
		"Yanni",
		"Mandolin",
		"Pete",
		"Vraiment",
		"Goo"
	};

	struct PcPrefab
	{
		const char* name;
		std::array<short, 19> skills;
		short health;
		short sp;
		short graphics;
		std::array<Boolean, 15> traits;
		short race;
	};

	constinit const std::array<PcPrefab, 6> c_prefabs{ {
		{
			.name = "Jenneke",
			.skills = {8,6,2, 6,0,0,0,0,0, 0,0,0,0,1, 0,0,2,0,0},
			.health = 22,
			.sp = 0,
			.graphics = 3,
			.traits = {0,0,1,0,0,0,1,0,0,0, 0,1,0,0,0},
			.race = 0,
		},
		{
			.name = "Thissa",
			.skills = {8,7,2, 0,0,6,3,0,3, 0,0,0,0,0, 0,0,0,2,0},
			.health = 24,
			.sp = 0,
			.graphics = 32,
			.traits = {1,0,0,0,0,1,0,0,0,0, 1,0,0,0,0},
			.race = 2,
		},
		{
			.name = "Frrrrrr",
			.skills = {8,6,2, 3,3,0,0,2,0, 0,0,0,0,0, 4,4,0,2,1},
			.health = 24,
			.sp = 0,
			.graphics = 29,
			.traits = {0,0,0,1,0,0,0,0,0,0, 0,0,1,0,0},
			.race = 1,
		},
		{
			.name = "Adrianna",
			.skills = {3,2,6, 2,0,0,2,0,0, 3,0,3,0,1, 0,0,0,0,0},
			.health = 16,
			.sp = 20,
			.graphics = 16,
			.traits = {0,1,0,0,0,0,0,0,0,0, 0,0,0,0,0},
			.race = 0,
		},
		{
			.name = "Feodoric",
			.skills = {2,2,6, 3,0,0,2,0,0,  2,1,4,0,0, 0,0,0,0,1},
			.health = 16,
			.sp = 20,
			.graphics = 23,
			.traits = {0,0,0,0,1,0,1,1,0,0, 0,0,0,0,1},
			.race = 0,
		},
		{
			.name = "Michael",
			.skills = {2,2,6, 0,2,0,2,0,1, 0,3,3,2,0, 0,0,0,0,0},
			.health = 18,
			.sp = 21,
			.graphics = 14,
			.traits = {0,1,0,0,0,0,0,0,0,0, 0,0,0,0,0},
			.race = 0,
		}
	} };

} // unnamed namespace

void pc_heal(pc_record_type& pc, short amt)
{
	if ((pc.main_status == 1) && (pc.cur_health < pc.max_health))
	{
		pc.cur_health = std::min(static_cast<short>(pc.cur_health + amt), pc.max_health);
	}
}

bool pc_cure(pc_record_type& pc, short amt)
{
	if (pc.main_status != 1)
	{
		return false;
	}
	pc.status[2] = std::max(static_cast<short>(pc.status[2] - amt), static_cast<short>(0));
	return true;
}

void pc_restore_sp(pc_record_type& pc, short amt)
{
	if (pc.cur_sp < pc.max_sp)
	{
		pc.cur_sp = std::min(static_cast<short>(pc.cur_sp + amt), pc.max_sp);
	}
}

void pc_setup_blank(pc_record_type& pc)
{
	pc = pc_record_type{};
	pc.name[0] = '\n';
	pc.skills[0] = 1;
	pc.skills[1] = 1;
	pc.skills[2] = 1;
	pc.cur_health = 6;
	pc.max_health = 6;
	pc.skill_pts = 60;
	pc.level = 1;
	pc.weap_poisoned = 24;
	pc.exp_adj = 100;
	std::fill(std::begin(pc.priest_spells), std::begin(pc.priest_spells) + 30, BOE_TRUE);
	std::fill(std::begin(pc.mage_spells), std::begin(pc.mage_spells) + 30, BOE_TRUE);
}

void pc_setup_debug(pc_record_type& pc, short num)
{
	pc = pc_record_type{};
	pc.main_status = 1;
	std::ranges::copy(std::string_view(c_debug_names.at(num)), pc.name);
	std::ranges::fill(pc.skills, static_cast<short>(8));
	pc.skills[0] = 20;
	pc.skills[1] = 20;
	pc.skills[2] = 20;
	pc.cur_health = 60;
	pc.max_health = 60;
	pc.cur_sp = 90;
	pc.max_sp = 90;
	pc.skill_pts = 60;
	pc.level = 1;
	//	pc.which_graphic = num * 3 + 1;
	pc.which_graphic = num + 4;
	pc.weap_poisoned = 16;
	pc.exp_adj = 100;
	std::ranges::fill(pc.priest_spells, BOE_TRUE);
	std::ranges::fill(pc.mage_spells, BOE_TRUE);
}

void pc_setup_prefab(pc_record_type& pc, short num)
{
	pc = pc_record_type{};
	pc.main_status = 1;
	pc.level = 1;
	pc.exp_adj = 100;
	std::fill(std::begin(pc.priest_spells), std::begin(pc.priest_spells) + 30, BOE_TRUE);
	std::fill(std::begin(pc.mage_spells), std::begin(pc.mage_spells) + 30, BOE_TRUE);

	const auto& prefab{ c_prefabs.at(num) };

	std::ranges::copy(std::string_view(prefab.name), pc.name);
	std::ranges::copy(prefab.skills, pc.skills);
	pc.race = prefab.race;
	pc.max_health = pc.cur_health = prefab.health;
	pc.max_sp = pc.cur_sp = prefab.sp;
	std::ranges::copy(prefab.traits, pc.traits);
	pc.which_graphic = prefab.graphics;
}
