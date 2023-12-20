#pragma once

#include "boe/game_file_structures.hpp"
#include <cassert>

static inline void hacks_adventurer_set_split_status(pc_record_type& pc)
{
	const auto value = static_cast<short>(pc.main_status);
	assert(value < 10);
	pc.main_status = static_cast<status>(value + 10);
}

static inline void hacks_adventurer_reset_split_status(pc_record_type& pc)
{
	const auto value = static_cast<short>(pc.main_status);
	if (value >= 10)
	{
		pc.main_status = static_cast<status>(value - 10);
	}
}


static inline status hacks_adventurer_without_split_status(const pc_record_type& pc)
{
	return static_cast<status>(static_cast<short>(pc.main_status) % 10);
}

static inline bool hacks_adventure_has_split_status(const pc_record_type& pc)
{
	const auto value = static_cast<short>(pc.main_status);
	return value < 10;
}
