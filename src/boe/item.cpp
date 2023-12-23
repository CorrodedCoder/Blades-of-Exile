#include "boe/item.hpp"

namespace {

const Boolean c_equippable[26] = { BOE_FALSE,BOE_TRUE,BOE_TRUE,BOE_FALSE,BOE_TRUE, BOE_TRUE,BOE_TRUE,BOE_FALSE,BOE_FALSE,BOE_FALSE,
						BOE_TRUE,BOE_FALSE,BOE_TRUE,BOE_TRUE,BOE_TRUE, BOE_TRUE,BOE_TRUE,BOE_TRUE,BOE_TRUE,BOE_TRUE,
						BOE_FALSE,BOE_FALSE,BOE_TRUE,BOE_TRUE,BOE_TRUE,BOE_TRUE };
const short c_num_hands_to_use[26] = { 0,1,2,0,0, 0,0,0,0,0 ,0,0,1,0,0, 0,1,0,0,0, 0,0,0,0,0, 0 };
const short c_num_that_can_equip[26] = { 0,2,1,0,1, 1,1,0,0,0, 1,0,1,1,1, 1,1,1,2,1, 0,0,1,1,1, 1 };

// For following, if an item of type n is equipped, no other items of type n can be equipped,
// if n > 0
const short c_excluding_types[26] = { 0,0,0,0,2, 1,1,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,2,1, 2 };

}

short item_weight(const item_record_type& item)
{
	if (item.variety == item_variety::None)
	{
		return 0;
	}
	if ((item.variety == item_variety::Arrows) || (item.variety == item_variety::ThrownMissile) || (item.variety == item_variety::Bolts) || (item.variety == item_variety::PotionOrMagicItem)
		|| ((item.variety == item_variety::GemStoneEtc) && (item.charges > 0)))
	{
		return static_cast<short>(item.charges) * static_cast<short>(item.weight);
	}
	return static_cast<short>(item.weight);
}

short item_val(const item_record_type& item)
{
	if (item.charges == 0)
		return item.value;
	return item.charges * item.value;
}

bool item_is_weapon(const item_record_type& item)
{
	return (item.variety == item_variety::OneHandedWeapon) || (item.variety == item_variety::TwoHandedWeapon) || (item.variety == item_variety::Arrows) || (item.variety == item_variety::Bolts);
}

bool is_ident(const item_record_type& item)
{
	return item.item_properties & 1;
}

bool is_magic(const item_record_type& item)
{
	return item.item_properties & 4;
}

bool is_contained(const item_record_type& item)
{
	return item.item_properties & 8;
}
bool is_cursed(const item_record_type& item)
{
	return item.item_properties & 16;
}
bool is_property(const item_record_type& item)
{
	return item.item_properties & 2;
}

Boolean equippable(item_variety variety)
{
	return c_equippable[static_cast<short>(variety)];
}

short num_hands_to_use(item_variety variety)
{
	return c_num_hands_to_use[static_cast<short>(variety)];
}

short num_that_can_equip(item_variety variety)
{
	return c_num_that_can_equip[static_cast<short>(variety)];
}

short excluding_types(item_variety variety)
{
	return c_excluding_types[static_cast<short>(variety)];
}

