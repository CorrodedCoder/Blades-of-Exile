#include "boe/pc.hpp"
#include "boe/item.hpp"
#include <algorithm>
#include <ranges>
#include <utility>
#include <array>
#include <string_view>
#include <numeric>

namespace
{
	const std::array<short, 3> c_rp{ 0,12,20 };
	const std::array<short, 15> c_ap{ 10,20,8,10,4, 6,10,7,12,15, -10,-8,-8,-20,-8 };
	const std::array<short, 26> c_item_priority{ 20,8,8,20,9, 9,3,2,1,0, 7,20,10,10,10, 10,10,10,5,6, 4,11,12,9,9, 9 };
	const std::array<short, 51> c_hit_chance{ 20,30,40,45,50,55,60,65,69,73,
								77,81,84,87,90,92,94,96,97,98,99
								,99,99,99,99,99,99,99,99,99,99
								,99,99,99,99,99,99,99,99,99,99,
								99,99,99,99,99,99,99,99,99,99 };

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

bool pc_has_cave_lore(const pc_record_type& pc)
{
	return (pc.main_status == status::Normal) && (pc.traits[trait::CaveLore] > 0);
}

bool pc_has_woodsman(const pc_record_type& pc)
{
	return (pc.main_status == status::Normal) && (pc.traits[trait::Woodsman] > 0);
}

void pc_heal(pc_record_type& pc, short amt)
{
	if ((pc.main_status == status::Normal) && (pc.cur_health < pc.max_health))
	{
		pc.cur_health = std::min(static_cast<short>(pc.cur_health + amt), pc.max_health);
	}
}

bool pc_cure(pc_record_type& pc, short amt)
{
	if (pc.main_status != status::Normal)
	{
		return false;
	}
	pc.gaffect(affect::Poisoned) = std::max(static_cast<short>(pc.gaffect(affect::Poisoned) - amt), static_cast<short>(0));
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
	pc.skills[skill::Strength] = 1;
	pc.skills[skill::Dexterity] = 1;
	pc.skills[skill::Intelligence] = 1;
	pc.cur_health = 6;
	pc.max_health = 6;
	pc.skill_pts = 60;
	pc.level = 1;
	pc.weap_poisoned = 24;
	pc.exp_adj = 100;
	std::ranges::fill(std::views::counted(pc.priest_spells, 30), BOE_TRUE);
	std::ranges::fill(std::views::counted(pc.mage_spells, 30), BOE_TRUE);
}

void pc_setup_debug(pc_record_type& pc, short num)
{
	pc = pc_record_type{};
	pc.main_status = status::Normal;
	std::ranges::copy(std::string_view(c_debug_names.at(static_cast<size_t>(num))), pc.name);
	std::ranges::fill(pc.skills, static_cast<short>(8));
	pc.skills[skill::Strength] = 20;
	pc.skills[skill::Dexterity] = 20;
	pc.skills[skill::Intelligence] = 20;
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
	pc.main_status = status::Normal;
	pc.level = 1;
	pc.exp_adj = 100;
	std::ranges::fill(std::views::counted(pc.priest_spells, 30), BOE_TRUE);
	std::ranges::fill(std::views::counted(pc.mage_spells, 30), BOE_TRUE);

	const auto& prefab{ c_prefabs.at(static_cast<size_t>(num)) };

	std::ranges::copy(std::string_view(prefab.name), pc.name);
	std::ranges::copy(prefab.skills, pc.skills);
	pc.race = prefab.race;
	pc.max_health = pc.cur_health = prefab.health;
	pc.max_sp = pc.cur_sp = prefab.sp;
	std::ranges::copy(prefab.traits, pc.traits);
	pc.which_graphic = prefab.graphics;
}

short pc_encumberance(const pc_record_type& pc)
{
#if defined(__cpp_lib_ranges_zip) && __cpp_lib_ranges_zip
	auto v = std::views::zip(pc.equip, pc.items)
		| std::views::filter([](const auto& pair) { return std::get<0>(pair) == BOE_TRUE; })
		| std::views::transform([](const auto& pair) { return std::get<1>(pair).awkward; });
#else
	auto v = std::views::iota(0, static_cast<int>(std::size(pc.equip)))
		| std::views::filter([pc](int i) { return pc.equip[i] == BOE_TRUE; })
		| std::views::transform([pc](int i) { return static_cast<short>(pc.items[i].awkward); });
#endif

#if defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold
	return static_cast<short>(std::ranges::fold_left(v, 0, std::plus<int>()));
#else
	return static_cast<short>(std::reduce(std::begin(v), std::end(v), 0));
#endif
}

short pc_get_tnl(const pc_record_type& pc)
{
#if defined(__cpp_lib_ranges_zip) && __cpp_lib_ranges_zip
	auto v = std::views::zip(pc.traits, c_ap)
		| std::views::filter([](const auto& pair) { return std::get<0>(pair) == BOE_TRUE; })
		| std::views::transform([](const auto& pair) { return std::get<1>(pair); });
#else
	auto v = std::views::iota(0, static_cast<int>(std::size(pc.traits)))
		| std::views::filter([pc](int i) { return pc.traits[i] == BOE_TRUE; })
		| std::views::transform([](int i) { return c_ap[static_cast<size_t>(i)]; });
#endif

#if defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold
	const short store_per = static_cast<short>(std::ranges::fold_left(v, 100, std::plus<int>()));
#else
	const short store_per = static_cast<short>(std::reduce(std::begin(v), std::end(v), 100));
#endif
	return ((100 + c_rp.at(static_cast<size_t>(pc.race))) * store_per) / 100;
}

short pc_has_space(const pc_record_type& pc)
{
	auto it = std::ranges::find_if(pc.items, [](const auto& item) { return item.variety == item_variety::None; });
	return static_cast<short>(std::distance(std::begin(pc.items), it));
}

// returnes equipped protection level of specified abil, or -1 if no such abil is equipped
short pc_prot_level(const pc_record_type& pc, short abil)
{
	static_assert(std::size(pc_record_type().items) == std::size(pc_record_type().equip));
	for (size_t i = 0; i < std::size(pc.items); i++)
	{
		const auto equip = pc.equip[i];
		const auto& item = pc.items[i];
		if ((equip == BOE_TRUE) && (item.variety != item_variety::None) && (item.ability == abil))
		{
			return item.ability_strength;
		}
	}
	return -1;
}

short pc_has_abil_equip(const pc_record_type& pc, short abil)
{
	short i = 0;

	while (((pc.items[i].variety == item_variety::None) || (pc.items[i].ability != abil)
		|| (pc.equip[i] == BOE_FALSE)) && (i < 24))
		i++;
	return i;

}

short pc_has_abil(const pc_record_type& pc, short abil)
{
	short i = 0;

	while (((pc.items[i].variety == item_variety::None) || (pc.items[i].ability != abil)
		) && (i < 24))
		i++;
	return i;
}

short pc_amount_can_carry(const pc_record_type& pc)
{
	return 100 + (15 * std::min(pc.skills[skill::Strength], static_cast<short>(20))) + ((pc.traits[trait::ExceptionalStr] == 0) ? 0 : 30)
		+ ((pc.traits[trait::BadBack] == 0) ? 0 : -50);
}

void pc_sort_items(pc_record_type& pc)
{
	// This is pretty much the same algorithm as std::sort, but the way
	// the poisoned weapon index is recorded makes it hard to substitute
	// without paying a performance penalty.
	bool items_swapped = false;
	do
	{
		items_swapped = false;
		for (size_t index = 0; index < std::size(pc.items) - 1; ++index)
		{
			if (c_item_priority[static_cast<size_t>(pc.items[index + 1].variety)] < c_item_priority[static_cast<size_t>(pc.items[index].variety)])
			{
				items_swapped = true;
				std::swap(pc.items[index], pc.items[index + 1]);
				std::swap(pc.equip[index], pc.equip[index + 1]);
				if (pc.weap_poisoned == static_cast<short>(index + 1))
				{
					--pc.weap_poisoned;
				}
				else if (pc.weap_poisoned == static_cast<short>(index))
				{
					++pc.weap_poisoned;
				}
			}
		}
	} while(items_swapped);
}

bool pc_affect(pc_record_type& pc, affect type, short how_much)
//type; // which status to affect
{
	if (pc.main_status != status::Normal)
	{
		return false;
	}
	pc.gaffect(type) = std::clamp<short>(pc.gaffect(type) + how_much, -8, 8);
	if (((type >= affect::Invulnerable) && (type <= affect::MartyrsShield)) || (type == affect::Paralyzed) || (type == affect::Acid))
	{
		pc.gaffect(type) = std::max(pc.gaffect(type), static_cast<short>(0));
	}
	return true;
}

short pc_carry_weight(const pc_record_type& pc)
{
	short i, storage = 0;
	Boolean airy = BOE_FALSE, heavy = BOE_FALSE;

	for (i = 0; i < 24; i++)
		if (pc.items[i].variety > item_variety::None) {
			storage += item_weight(pc.items[i]);
			if (pc.items[i].ability == 44)
				airy = BOE_TRUE;
			if (pc.items[i].ability == 45)
				heavy = BOE_TRUE;
		}
	if (airy == BOE_TRUE)
		storage -= 30;
	if (heavy == BOE_TRUE)
		storage += 30;
	if (storage < 0)
		storage = 0;
	return storage;
}

short pc_luck(const pc_record_type& pc)
{
	return pc.skills[skill::Luck];
}

short pc_level(const pc_record_type& pc)
{
	return pc.level;
}

bool pc_curse(pc_record_type& pc, short how_much)
{
	pc.gaffect(affect::CursedBlessed) = static_cast<short>(std::max(pc.gaffect(affect::CursedBlessed) - how_much, -8));
	return true;
}

bool pc_dumbfound(pc_record_type& pc, short how_much, short modifier)
{
	if (const short r1 = rand_short(0, 90) + modifier; r1 < pc.level)
	{
		how_much -= 2;
	}
	if (how_much <= 0)
	{
		return false;
	}
	pc.gaffect(affect::Dumbfounded) = static_cast<short>(std::min(pc.gaffect(affect::Dumbfounded) + how_much, 8));
	return true;
}

bool pc_disease(pc_record_type& pc, short how_much)
{
	if (const short r1 = rand_short(0, 100); r1 < pc.level * 2)
	{
		how_much -= 2;
	}
	if (how_much <= 0)
	{
		return false;
	}
	if (const short level = pc_prot_level(pc, 62); level > 0)
	{
		how_much -= level / 2;
	}
	if ((pc.traits[trait::Frail] == BOE_TRUE) && (how_much > 1))
	{
		how_much++;
	}
	if ((pc.traits[trait::Frail] == BOE_TRUE) && (how_much == 1) && (rand_short(0, 1) == 0))
	{
		how_much++;
	}
	pc.gaffect(affect::Diseased) = static_cast<short>(std::min(pc.gaffect(affect::Diseased) + how_much, 8));
	return true;
}

bool pc_sleep(pc_record_type& pc, short how_much, short adjust)
{
	if (const short level = pc_prot_level(pc, 53); level > 0)
	{
		how_much -= level / 2;
	}
	if (const short level = pc_prot_level(pc, 54); level > 0)
	{
		how_much -= level;
	}
	if (const short r1 = rand_short(0, 100) + adjust; r1 < 30 + pc.level * 2)
	{
		how_much = -1;
	}
	if ((pc.traits[trait::HighlyAlert] > 0) || (pc.gaffect(affect::Asleep) < 0))
	{
		how_much = -1;
	}
	if (how_much <= 0)
	{
		return false;
	}
	pc.gaffect(affect::Asleep) = how_much;
	return true;
}

bool pc_paralyze(pc_record_type& pc, short how_much, short adjust)
{
	if (const short level = pc_prot_level(pc, 53); level > 0)
	{
		how_much -= level / 2;
	}
	if (const short level = pc_prot_level(pc, 54); level > 0)
	{
		how_much -= level * 300;
	}
	if (const short r1 = rand_short(0, 100) + adjust; r1 < 30 + pc.level * 2)
	{
		how_much = -1;
	}
	if (how_much <= 0)
	{
		return false;
	}
	pc.gaffect(affect::Paralyzed) = how_much;
	return true;
}

bool pc_slow(pc_record_type& pc, short how_much)
{
	pc.gaffect(affect::Speed) = boe_clamp(pc.gaffect(affect::Speed) - how_much, -8, 8);
	return true;
}

bool pc_web(pc_record_type& pc, short how_much)
{
	pc.gaffect(affect::Webbed) = static_cast<short>(std::min(pc.gaffect(affect::Webbed) + how_much, 8));
	return true;
}

bool pc_acid(pc_record_type& pc, short how_much)
{
	if (pc_has_abil_equip(pc, 122) < 24)
	{
		return false;
	}
	pc.gaffect(affect::Acid) += how_much;
	return true;
}

short pc_damage_adjust(const pc_record_type& pc, short how_much, damage_type type, short type_of_attacker, short parry_modifier, short party_adjust)
{
	// armor	
	if ((type == damage_type::Weapon) || (type == damage_type::DemonAttack) || (type == damage_type::UndeadAttack))
	{
		how_much -= boe_clamp(pc.gaffect(affect::CursedBlessed), -5, 5);
		for (short i = 0; i < 24; i++)
		{
			const auto& item{ pc.items[i] };
			if ((item.variety != item_variety::None) && (pc.equip[i] == BOE_TRUE))
			{
				if ((item.variety >= item_variety::Shield) && (item.variety <= item_variety::Boots))
				{
					how_much -= rand_short(1, item.item_level);

					// bonus for magical items
					if (item.bonus > 0)
					{
						how_much -= rand_short(1, item.bonus);
						how_much -= item.bonus / 2;
					}
					if (item.bonus < 0)
					{
						how_much -= item.bonus;
					}
					if (rand_short(0, 100) < skill_hit_chance(pc.skills[skill::Defense]) - 20)
					{
						--how_much;
					}
				}
				if (item.protection > 0)
				{
					how_much -= rand_short(1, item.protection);
				}
				if (item.protection < 0)
				{
					how_much += rand_short(1, -1 * item.protection);
				}
			}
		}
	}

	// parry
	if ((type < damage_type::Poison) && (parry_modifier < 100))
	{
		how_much -= parry_modifier / 4;
	}

	how_much -= party_adjust;

	if (type != damage_type::MarkedDamage)
	{
		// toughness
		if (pc.traits[trait::Toughness] == BOE_TRUE)
		{
			--how_much;
		}
		// luck
		if (rand_short(0, 100) < 2 * (skill_hit_chance(pc.skills[skill::Luck]) - 20))
		{
			--how_much;
		}
	}

	short level = 0;
	if ((type == damage_type::Weapon) && ((level = pc_prot_level(pc, 30)) > 0))
	{
		how_much -= level;
	}
	if ((type == damage_type::DemonAttack) && ((level = pc_prot_level(pc, 57)) > 0))
	{
		how_much /= ((level >= 7) ? 4 : 2);
	}
	if ((type == damage_type::UndeadAttack) && ((level = pc_prot_level(pc, 58)) > 0))
	{
		how_much /= ((level >= 7) ? 4 : 2);
	}
	if ((type_of_attacker == 6) && ((level = pc_prot_level(pc, 59)) > 0))
	{
		how_much /= ((level >= 7) ? 4 : 2);
	}
	if ((type_of_attacker == 1) && ((level = pc_prot_level(pc, 60)) > 0))
	{
		how_much /= ((level >= 7) ? 4 : 2);
	}
	if ((type_of_attacker == 9) && ((level = pc_prot_level(pc, 61)) > 0))
	{
		how_much /= ((level >= 7) ? 4 : 2);
	}

	// invuln
	if (pc.gaffect(affect::Invulnerable) > 0)
	{
		how_much = 0;
	}

	// magic resistance
	if ((type == damage_type::GeneralMagic) && ((level = pc_prot_level(pc, 35)) > 0))
	{
		how_much /= ((level >= 7) ? 4 : 2);
	}

	// Mag. res helps w. fire and cold
	if (((type == damage_type::Fire) || (type == damage_type::Cold)) &&	(pc.gaffect(affect::MagicResistant) > 0))
	{
		how_much /= 2;
	}

	// fire res.
	if ((type == damage_type::Fire) && ((level = pc_prot_level(pc, 32)) > 0))
	{
		how_much /= ((level >= 7) ? 4 : 2);
	}

	// cold res.
	if ((type == damage_type::Cold) && ((level = pc_prot_level(pc, 33)) > 0))
	{
		how_much /= ((level >= 7) ? 4 : 2);
	}

	// major resistance
	if (((type == damage_type::Fire) || (type == damage_type::Poison) || (type == damage_type::GeneralMagic) || (type == damage_type::Cold))
		&& ((level = pc_prot_level(pc, 31)) > 0))
	{
		how_much /= ((level >= 7) ? 4 : 2);
	}

	return how_much;
}

short skill_hit_chance(short type)
{
	return c_hit_chance[static_cast<size_t>(type)];
}
