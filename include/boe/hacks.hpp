#pragma once

#include "boe/game_file_structures.hpp"
#include "boe/compatibility.hpp"
#include <cassert>

static inline void hacks_adventurer_set_split_status(pc_record_type& pc)
{
	const auto value = to_underlying(pc.main_status);
	assert(value < 10);
	pc.main_status = static_cast<status_type>(value + 10);
}

static inline void hacks_adventurer_reset_split_status(pc_record_type& pc)
{
	const auto value = to_underlying(pc.main_status);
	if (value >= 10)
	{
		pc.main_status = static_cast<status_type>(value - 10);
	}
}


static inline status_type hacks_adventurer_without_split_status(const pc_record_type& pc)
{
	return static_cast<status_type>(to_underlying(pc.main_status) % 10);
}

static inline bool hacks_adventure_has_split_status(const pc_record_type& pc)
{
	const auto value = to_underlying(pc.main_status);
	return value < 10;
}
