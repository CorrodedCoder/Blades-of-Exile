#include "boe/item_source.hpp"
#include "boe/utility.hpp"
#include <array>
#include <cassert>

namespace {
	const std::array<short, 5> c_loot_min{ 0,0,5,50,400 };
	const std::array<short, 5> c_loot_max{ 3,8,40,800,4000 };
	
	const std::array<short, 48> which_treas_chart{
		1,1,1,1,1,2,2,2,2,2,
		3,3,3,3,3,2,2,2,4,4,
		4,4,5,5,5,6,6,6,7,7,
		7,8,8,9,9,10,11,12,12,13,
		13,14, 9,10,11,9,10,11
	};

	const item_record_type c_null_item{};
}

ItemSource::ItemSource(const ScenarioExtra& scenario_extra)
	: scenario_extra_(scenario_extra)
{
}

const item_record_type& ItemSource::stored_item(short which) const
{
	if ((which >= 400) || (which < 0)) {
		return c_null_item;
	}
	return scenario_extra_.item(which);
}

item_record_type ItemSource::food(void) const
{
	item_record_type food = 
		{ item_variety::Food,0, 0,0,0,0,0,0, 62,0,0,0,0,0, 0, 0,0, {0,0},"Food", "Food",0,0,0,0};
	food.graphic_num += static_cast<unsigned char>(rand_short(0,2));
	food.item_level = rand_short(5,10);
	if (rand_short(0,9) == 5)
		food.graphic_num = 113;
	if (rand_short(0,9) == 5)
		food.graphic_num = 114;
	// food doesn't always appear
	if (rand_short(0,2) != 1)
		food.variety = item_variety::None;
	return food;
}


const item_record_type& ItemSource::item_of_type(short loot_max,short min_val,short max_val, item_variety t1, item_variety t2, item_variety t3) const
{
	// occasionally get nice item
	if (rand_short(0,160) == 80)
	{
		loot_max += 2;
		max_val += 2000;
	}
	for (short i = 0; i < 80; i++)
	{
		const auto j = rand_short(0,399);
		const auto& temp_i = stored_item(j);
		if ((temp_i.variety == t1) || (temp_i.variety == t2) || (temp_i.variety == t3))
		{
			const short val = (temp_i.charges > 0) ? temp_i.charges * temp_i.value : temp_i.value;
			if ((val >= min_val) && (val <= max_val) && (temp_i.treas_class != 0) &&
				(temp_i.treas_class <= loot_max))
			{
				return temp_i;
			}
		}
	}
	return c_null_item;
}

const item_record_type& ItemSource::item_of_type(short loot, item_variety t1, item_variety t2, item_variety t3) const
{
	assert(loot >= 0);
	return item_of_type(loot, c_loot_min[static_cast<size_t>(loot)], c_loot_max[static_cast<size_t>(loot)], t1, t2, t3);
}

const item_record_type& ItemSource::weapon(short loot,short level) const
{
	(void)level; // Unreferenced parameter
	if (loot == 0)
		return c_null_item;
	return item_of_type(loot, item_variety::OneHandedWeapon, item_variety::TwoHandedWeapon, item_variety::Invalid);
}

const item_record_type& ItemSource::armor(short loot,short level) const
{
	(void)level; // Unreferenced parameter
	if (loot == 0)
		return c_null_item;
	return item_of_type(loot, item_variety::Armor, item_variety::Invalid, item_variety::Invalid);
}

const item_record_type& ItemSource::helm(short loot) const
{
	return item_of_type(loot, item_variety::Helm, item_variety::Invalid, item_variety::Invalid);
}

const item_record_type& ItemSource::gloves(short loot) const
{
	return item_of_type(loot, item_variety::Gloves, item_variety::Invalid, item_variety::Invalid);
}

const item_record_type& ItemSource::boots(short loot) const
{
	return item_of_type(loot, item_variety::Boots, item_variety::Invalid, item_variety::Invalid);
}

const item_record_type& ItemSource::shield(short loot) const
{
	return item_of_type(loot, item_variety::Shield, item_variety::Invalid, item_variety::Invalid);
}

const item_record_type& ItemSource::potion(short loot) const
{
	assert(loot >= 0);
	if (rand_short(0,80) < 20 * (4 - loot))
		return item_of_type(loot, c_loot_min[static_cast<size_t>(loot)], c_loot_max[static_cast<size_t>(loot)] / 2, item_variety::PotionOrMagicItem, item_variety::Invalid, item_variety::Invalid);
	return item_of_type(loot, item_variety::PotionOrMagicItem, item_variety::Invalid, item_variety::Invalid);
}

const item_record_type& ItemSource::scroll(short loot) const
{
	return item_of_type(loot, item_variety::ScrollOrMagicItem, item_variety::Invalid, item_variety::Invalid);
}

const item_record_type& ItemSource::missile(short loot) const
{
	if (rand_short(0,2) < 2)
		return item_of_type(loot, item_variety::Arrows, item_variety::ThrownMissile, item_variety::Bow);
	return item_of_type(loot, item_variety::Crossbow, item_variety::Bolts, item_variety::MissileWeapon);
}

const item_record_type& ItemSource::poison(short loot,short level) const
{
	(void)level; // Unreferenced parameter
	return item_of_type(loot, item_variety::WeaponPoison, item_variety::Invalid, item_variety::Invalid);
}

const item_record_type& ItemSource::wand(short loot) const
{
	return item_of_type(loot, item_variety::Wand, item_variety::Invalid, item_variety::Invalid);
}

const item_record_type& ItemSource::ring(short loot) const
{
	return item_of_type(loot, item_variety::Ring, item_variety::Invalid, item_variety::Invalid);
}

const item_record_type& ItemSource::necklace(short loot) const
{
	return item_of_type(loot, item_variety::Necklace, item_variety::Invalid, item_variety::Invalid);
} 

item_record_type ItemSource::treasure(short loot, short level, short mode) const
//short mode; // 0 - normal  1 - force
{
	(void)mode;
	short r1 = rand_short(0, 41);
	if (loot >= 3)
	{
		r1 += 3;
	}
	if (loot == 4)
	{
		r1 += 3;
	}
	item_record_type treas;
	treas.variety = item_variety::None;
	switch (which_treas_chart.at(static_cast<size_t>(r1)))
	{
	case 1: treas = food(); break;
	case 2: treas = weapon(loot, level); break;
	case 3: treas = armor(loot, level); break;
	case 4: treas = shield(loot); break;
	case 5: treas = helm(loot); break;
	case 6: treas = missile(loot); break;
	case 7: treas = potion(loot); break;
	case 8: treas = scroll(loot); break;
	case 9: treas = wand(loot); break;
	case 10: treas = ring(loot); break;
	case 11: treas = necklace(loot); break;
	case 12: treas = poison(loot, level); break;
	case 13: treas = gloves(loot); break;
	case 14: treas = boots(loot); break;
	}
	if (treas.variety == item_variety::None)
	{
		treas.value = 0;
	}
	return treas;

}
