#include "boe/adventurers.hpp"
#include "boe/pc.hpp"
#include <algorithm>
#include <numeric>

bool cave_lore_present(const Adventurers& adven)
{
	return std::ranges::any_of(adven, [](const auto& adventurer) { return (adventurer.main_status == 1) && (adventurer.traits[4] > 0); });
}

bool woodsman_present(const Adventurers& adven)
{
	return std::ranges::any_of(adven, [](const auto& adventurer) { return (adventurer.main_status == 1) && (adventurer.traits[5] > 0); });
}

short mage_lore_total(const Adventurers& adven)
{
	auto pc_mage_lore = [](short total, const auto& adventurer) { return (adventurer.main_status == 1) ? adventurer.skills[11] + total : total; };
#if __cpp_lib_ranges_fold
	return std::ranges::fold_left(adven, static_cast<short>(0), pc_mage_lore);
#else
	return std::reduce(std::begin(adven), std::end(adven), static_cast<short>(0), pc_mage_lore);
#endif
}

void heal_party(Adventurers& adventurers, short amt)
{
	for (auto& pc : adventurers)
	{
		heal_pc(pc, amt);
	}
}
