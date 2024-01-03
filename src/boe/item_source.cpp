#include "boe/item_source.hpp"
#include "boe/utility.hpp"

static const short c_loot_min[5] = {0,0,5,50,400};
static const short c_loot_max[5] = {3,8,40,800,4000};

static const item_record_type c_null_item{};

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

const item_record_type& ItemSource::weapon(short loot,short level) const
{
	(void)level; // Unreferenced parameter
	if (loot == 0)
		return c_null_item;
	return item_of_type(loot,c_loot_min[loot], c_loot_max[loot], item_variety::OneHandedWeapon, item_variety::TwoHandedWeapon, item_variety::Invalid);
}

const item_record_type& ItemSource::armor(short loot,short level) const
{
	(void)level; // Unreferenced parameter
	if (loot == 0)
		return c_null_item;
	return item_of_type(loot, c_loot_min[loot], c_loot_max[loot], item_variety::Armor, item_variety::Invalid, item_variety::Invalid);
}

const item_record_type& ItemSource::helm(short loot) const
{
	return item_of_type(loot, c_loot_min[loot], c_loot_max[loot], item_variety::Helm, item_variety::Invalid, item_variety::Invalid);
}

const item_record_type& ItemSource::gloves(short loot) const
{
	return item_of_type(loot, c_loot_min[loot], c_loot_max[loot], item_variety::Gloves, item_variety::Invalid, item_variety::Invalid);
}

const item_record_type& ItemSource::boots(short loot) const
{
	return item_of_type(loot, c_loot_min[loot], c_loot_max[loot], item_variety::Boots, item_variety::Invalid, item_variety::Invalid);
}

const item_record_type& ItemSource::shield(short loot) const
{
	return item_of_type(loot, c_loot_min[loot], c_loot_max[loot], item_variety::Shield, item_variety::Invalid, item_variety::Invalid);
}

const item_record_type& ItemSource::potion(short loot) const
{
	if (rand_short(0,80) < 20 * (4 - loot))
		return item_of_type(loot, c_loot_min[loot], c_loot_max[loot] / 2, item_variety::PotionOrMagicItem, item_variety::Invalid, item_variety::Invalid);
	return item_of_type(loot, c_loot_min[loot], c_loot_max[loot], item_variety::PotionOrMagicItem, item_variety::Invalid, item_variety::Invalid);
}

const item_record_type& ItemSource::scroll(short loot) const
{
	return item_of_type(loot, c_loot_min[loot], c_loot_max[loot], item_variety::ScrollOrMagicItem, item_variety::Invalid, item_variety::Invalid);
}

const item_record_type& ItemSource::missile(short loot) const
{
	if (rand_short(0,2) < 2)
		return item_of_type(loot, c_loot_min[loot], c_loot_max[loot], item_variety::Arrows, item_variety::ThrownMissile, item_variety::Bow);
	return item_of_type(loot, c_loot_min[loot], c_loot_max[loot], item_variety::Crossbow, item_variety::Bolts, item_variety::MissileWeapon);
}

const item_record_type& ItemSource::poison(short loot,short level) const
{
	(void)level; // Unreferenced parameter
	return item_of_type(loot, c_loot_min[loot], c_loot_max[loot], item_variety::WeaponPoison, item_variety::Invalid, item_variety::Invalid);
}

const item_record_type& ItemSource::wand(short loot) const
{
	return item_of_type(loot, c_loot_min[loot], c_loot_max[loot], item_variety::Wand, item_variety::Invalid, item_variety::Invalid);
}

const item_record_type& ItemSource::ring(short loot) const
{
	return item_of_type(loot, c_loot_min[loot], c_loot_max[loot], item_variety::Ring, item_variety::Invalid, item_variety::Invalid);
}

const item_record_type& ItemSource::necklace(short loot) const
{
	return item_of_type(loot, c_loot_min[loot], c_loot_max[loot], item_variety::Necklace, item_variety::Invalid, item_variety::Invalid);
} 
