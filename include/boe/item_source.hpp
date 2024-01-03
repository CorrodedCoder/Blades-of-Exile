#pragma once

#include "boe/game_file_structures.hpp"
#include "boe/scenario_extra.hpp"

class ItemSource
{
	const ScenarioExtra& scenario_extra_;

	const item_record_type& item_of_type(short loot, item_variety t1, item_variety t2, item_variety t3) const;
public:
	ItemSource(const ScenarioExtra& scenario_extra);
	[[nodiscard]] const item_record_type& stored_item(short which) const;
	[[nodiscard]] item_record_type food(void) const;
	[[nodiscard]] const item_record_type& weapon(short loot, short level) const;
	[[nodiscard]] const item_record_type& armor(short loot, short level) const;
	[[nodiscard]] const item_record_type& helm(short loot) const;
	[[nodiscard]] const item_record_type& gloves(short loot) const;
	[[nodiscard]] const item_record_type& boots(short loot) const;
	[[nodiscard]] const item_record_type& shield(short loot) const;
	[[nodiscard]] const item_record_type& potion(short loot) const;
	[[nodiscard]] const item_record_type& scroll(short loot) const;
	[[nodiscard]] const item_record_type& missile(short loot) const;
	[[nodiscard]] const item_record_type& poison(short loot, short level) const;
	[[nodiscard]] const item_record_type& wand(short loot) const;
	[[nodiscard]] const item_record_type& ring(short loot) const;
	[[nodiscard]] const item_record_type& necklace(short loot) const;
	[[nodiscard]] const item_record_type& item_of_type(short loot_max, short min_val, short max_val, item_variety t1, item_variety t2, item_variety t3) const;
};
