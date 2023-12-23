#include "boe/item.hpp"

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
