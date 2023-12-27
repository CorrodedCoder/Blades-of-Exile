#include "global.h"
#include "itemdata.h"
#include "scenario.hpp"
#include <cstdio>
#include <cstring>

static const short loot_min[5] = {0,0,5,50,400};
static const short loot_max[5] = {3,8,40,800,4000};

static const item_record_type null_item{};

const item_record_type& get_stored_item(short which)
{
	if ((which >= 400) || (which < 0)) {
		return null_item;
	}
	return scenario_item(which);
}

item_record_type get_food()
{
	item_record_type food = 
		{ item_variety::Food,0, 0,0,0,0,0,0, 62,0,0,0,0,0, 0, 0,0, {0,0},"Food", "Food",0,0,0,0};
	food.graphic_num += get_ran(1,0,2);
	food.item_level = get_ran(1,5,10);
	if (get_ran(1,0,9) == 5)
		food.graphic_num = 113;
	if (get_ran(1,0,9) == 5)
		food.graphic_num = 114;
	// food doesn't always appear
	if (get_ran(1,0,2) != 1)
		food.variety = item_variety::None;
	return food;
}


const item_record_type& pull_item_of_type(short loot_max,short min_val,short max_val, item_variety t1, item_variety t2, item_variety t3)
{
	// occasionally get nice item
	if (get_ran(1,0,160) == 80)
	{
		loot_max += 2;
		max_val += 2000;
	}
	for (short i = 0; i < 80; i++)
	{
		const auto j = get_ran(1,0,399);
		const auto& temp_i = get_stored_item(j);
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
	return null_item;
}

const item_record_type& get_weapon(short loot,short level)
{
	if (loot == 0)
		return null_item;
	return pull_item_of_type(loot,loot_min[loot],loot_max[loot], item_variety::OneHandedWeapon, item_variety::TwoHandedWeapon, item_variety::Invalid);
}

const item_record_type& get_armor(short loot,short level)
{
	if (loot == 0)
		return null_item;
	return pull_item_of_type(loot,loot_min[loot],loot_max[loot], item_variety::Armor, item_variety::Invalid, item_variety::Invalid);
}

const item_record_type& get_helm(short loot)
{
	return pull_item_of_type(loot,loot_min[loot],loot_max[loot], item_variety::Helm, item_variety::Invalid, item_variety::Invalid);
}

const item_record_type& get_gloves(short loot)
{
	return pull_item_of_type(loot,loot_min[loot],loot_max[loot], item_variety::Gloves, item_variety::Invalid, item_variety::Invalid);
}

const item_record_type& get_boots(short loot)
{
	return pull_item_of_type(loot,loot_min[loot],loot_max[loot], item_variety::Boots, item_variety::Invalid, item_variety::Invalid);
}

const item_record_type& get_shield(short loot)
{
	return pull_item_of_type(loot,loot_min[loot],loot_max[loot], item_variety::Shield, item_variety::Invalid, item_variety::Invalid);
}

const item_record_type& get_potion(short loot)
{
	if (get_ran(1,0,80) < 20 * (4 - loot))
		return pull_item_of_type(loot,loot_min[loot],loot_max[loot] / 2, item_variety::PotionOrMagicItem, item_variety::Invalid, item_variety::Invalid);
	return pull_item_of_type(loot,loot_min[loot],loot_max[loot], item_variety::PotionOrMagicItem, item_variety::Invalid, item_variety::Invalid);
}

const item_record_type& get_scroll(short loot)
{
	return pull_item_of_type(loot,loot_min[loot],loot_max[loot], item_variety::ScrollOrMagicItem, item_variety::Invalid, item_variety::Invalid);
}

const item_record_type& get_missile(short loot)
{
	if (get_ran(1,0,2) < 2)
		return pull_item_of_type(loot,loot_min[loot],loot_max[loot], item_variety::Arrows, item_variety::ThrownMissile, item_variety::Bow);
	return pull_item_of_type(loot,loot_min[loot],loot_max[loot], item_variety::Crossbow, item_variety::Bolts, item_variety::MissileWeapon);
}

const item_record_type& get_poison(short loot,short level)
{
	return pull_item_of_type(loot,loot_min[loot],loot_max[loot], item_variety::WeaponPoison, item_variety::Invalid, item_variety::Invalid);
}

const item_record_type& get_wand(short loot)
{
	return pull_item_of_type(loot,loot_min[loot],loot_max[loot], item_variety::Wand, item_variety::Invalid, item_variety::Invalid);
}

const item_record_type& get_ring(short loot)
{
	return pull_item_of_type(loot,loot_min[loot],loot_max[loot], item_variety::Ring, item_variety::Invalid, item_variety::Invalid);
}

const item_record_type& get_necklace(short loot)
{
	return pull_item_of_type(loot,loot_min[loot],loot_max[loot], item_variety::Necklace, item_variety::Invalid, item_variety::Invalid);
} 
