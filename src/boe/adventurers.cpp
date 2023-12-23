#include "boe/adventurers.hpp"
#include "boe/pc.hpp"
#include <algorithm>
#include <numeric>
#include <ranges>

namespace
{
	auto is_active = [](const auto& adventurer) { return adventurer.main_status == status::Normal; };
}

bool cave_lore_present(const Adventurers& adven)
{
	auto has_cavelore = [](const auto& adventurer) { return adventurer.traits[trait::CaveLore] > 0; };
	auto v = adven | std::views::filter(is_active);
	return std::ranges::any_of(v, has_cavelore);
}

bool woodsman_present(const Adventurers& adven)
{
	auto has_woodsman = [](const auto& adventurer) { return adventurer.traits[trait::Woodsman] > 0; };
	auto v = adven | std::views::filter(is_active);
	return std::ranges::any_of(v, has_woodsman);
}

short mage_lore_total(const Adventurers& adven)
{
	auto pc_mage_lore = [](const auto& adventurer) { return adventurer.skills[11]; };
	auto v = adven | std::views::filter(is_active) | std::views::transform(pc_mage_lore);
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
	for (auto& pc : adventurers | std::views::filter(is_active))
	{
		pc_restore_sp(pc, amt);
	}
}

bool adventurers_has_ability(const Adventurers& adventurers, short abil)
{
	for (const auto& pc : adventurers)
		if (pc.main_status == status::Normal)
			if (pc_has_abil(pc, abil) < 24)
				return true;
	return false;
}

short adventurers_luck_total(const Adventurers& adventurers)
{
	short total = 0;
	size_t i = 0;
	for (i = 0; i < 6; i++)
		if (adventurers[i].main_status == status::Normal)
			total += pc_luck(adventurers[i]);
	return total;
}
