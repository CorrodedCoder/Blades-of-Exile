#include "boe/adventurers.hpp"
#include "boe/pc.hpp"
#include <algorithm>
#include <numeric>

bool cave_lore_present(const Adventurers& adven)
{
	return std::ranges::any_of(adven, [](const auto& adventurer) { return (adventurer.main_status == 1) && (adventurer.traits[trait::CaveLore] > 0); });
}

bool woodsman_present(const Adventurers& adven)
{
	return std::ranges::any_of(adven, [](const auto& adventurer) { return (adventurer.main_status == 1) && (adventurer.traits[trait::Woodsman] > 0); });
}

short mage_lore_total(const Adventurers& adven)
{
	auto pc_mage_lore = [](short total, const auto& adventurer) { return (adventurer.main_status == 1) ? static_cast<short>(adventurer.skills[11] + total) : total; };
#if __cpp_lib_ranges_fold
	return std::ranges::fold_left(adven, static_cast<short>(0), pc_mage_lore);
#else
	return std::reduce(std::begin(adven), std::end(adven), static_cast<short>(0), pc_mage_lore);
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
	for (auto& pc : adventurers)
	{
		if (pc.main_status == 1)
		{
			pc_restore_sp(pc, amt);
		}
	}
}

