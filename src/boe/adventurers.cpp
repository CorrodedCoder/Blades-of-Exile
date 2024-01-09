#include "boe/adventurers.hpp"
#include "boe/pc.hpp"
#include <algorithm>
#include <numeric>
#include <ranges>

namespace
{
	auto is_normal = [](const auto& adventurer) { return adventurer.main_status == status_type::Normal; };
}

bool cave_lore_present(const Adventurers& adventurers)
{
	return std::ranges::any_of(adventurers, pc_has_cave_lore);
}

bool woodsman_present(const Adventurers& adventurers)
{
	return std::ranges::any_of(adventurers, pc_has_woodsman);
}

short mage_lore_total(const Adventurers& adventurers)
{
	auto pc_mage_lore = [](const auto& adventurer) { return adventurer.skills[skill::MageLore]; };
	auto v = adventurers | std::views::filter(is_normal) | std::views::transform(pc_mage_lore);
#if defined(__cpp_lib_ranges_fold) && __cpp_lib_ranges_fold
	return static_cast<short>(std::ranges::fold_left(v, 0, std::plus<int>()));
#else
	return static_cast<short>(std::reduce(std::begin(v), std::end(v), 0));
#endif
}

void adventurers_heal(Adventurers& adventurers, short amt)
{
	for (auto& pc : adventurers)
	{
		pc_heal(pc, amt);
	}
}

bool adventurers_cure(Adventurers& adventurers, short amt)
{
	return 0 != std::ranges::count_if(adventurers, [amt](auto& pc) { return pc_cure(pc, amt); });
}

void adventurers_restore_sp(Adventurers& adventurers, short amt)
{
	for (auto& pc : adventurers | std::views::filter(is_normal))
	{
		pc_restore_sp(pc, amt);
	}
}

bool adventurers_has_ability(const Adventurers& adventurers, short abil)
{
	for (const auto& pc : adventurers)
	{
		if ( (pc.main_status == status_type::Normal) && (pc_has_abil(pc, abil) < 24) )
		{
			return true;
		}
	}
	return false;
}

short adventurers_luck_total(const Adventurers& adventurers)
{
	short total = 0;
	for (const auto& pc : adventurers)
	{
		if (pc.main_status == status_type::Normal)
		{
			total += pc_luck(pc);
		}
	}
	return total;
}

short adventurers_level_total(const Adventurers& adventurers)
{
	short total = 0;
	for (const auto& pc : adventurers)
	{
		if (pc.main_status == status_type::Normal)
		{
			total += pc_level(pc);
		}
	}
	return total;
}

bool adventurers_dead(const Adventurers& adventurers)
{
	for (const auto& pc: adventurers)
	{
		if (pc.main_status == status_type::Normal)
		{
			return false;
		}
	}
	return true;
}

short adventurers_count_normal(const Adventurers& adventurers)
{
	return static_cast<short>(std::ranges::count_if(adventurers, is_normal));
}

bool adventurers_anyone_normal(const Adventurers& adventurers)
{
	return std::ranges::any_of(adventurers, is_normal);
}
