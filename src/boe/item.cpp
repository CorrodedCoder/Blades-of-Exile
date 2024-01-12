#include "boe/item.hpp"
#include <array>
#include <format>

namespace {

const std::array<bool,26> c_equippable{ false,true,true,false,true,true,true,false,false,false,
						true,false,true,true,true, true,true,true,true,true,
						false,false,true,true,true,true };
const std::array<short,26> c_num_hands_to_use{ 0,1,2,0,0, 0,0,0,0,0 ,0,0,1,0,0, 0,1,0,0,0, 0,0,0,0,0, 0 };
const std::array<short,26> c_num_that_can_equip{ 0,2,1,0,1, 1,1,0,0,0, 1,0,1,1,1, 1,1,1,2,1, 0,0,1,1,1, 1 };

// For following, if an item of type n is equipped, no other items of type n can be equipped,
// if n > 0
const std::array<short,26> c_excluding_types{ 0,0,0,0,2, 1,1,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,2,1, 2 };

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

bool equippable(item_variety variety)
{
	return c_equippable.at(static_cast<size_t>(variety));
}

short num_hands_to_use(item_variety variety)
{
	return c_num_hands_to_use.at(static_cast<size_t>(variety));
}

short num_that_can_equip(item_variety variety)
{
	return c_num_that_can_equip.at(static_cast<size_t>(variety));
}

short excluding_types(item_variety variety)
{
	return c_excluding_types.at(static_cast<size_t>(variety));
}

void enchant_weapon(item_record_type& item, short enchant_type, short new_val)
{
	std::string store_name;

	if (is_magic(item) || (item.ability != 0))
	{
		return;
	}
	item.item_properties |= 4;
	switch (enchant_type)
	{
	case 0:
		store_name = std::format("{} (+1)", item.full_name);
		item.bonus++;
		item.value = new_val;
		break;
	case 1:
		store_name = std::format("{} (+2)", item.full_name);
		item.bonus += 2;
		item.value = new_val;
		break;
	case 2:
		store_name = std::format("{} (+3)", item.full_name);
		item.bonus += 3;
		item.value = new_val;
		break;
	case 3:
		store_name = std::format("{} (F)", item.full_name);
		item.ability = 110;
		item.ability_strength = 5;
		item.charges = 8;
		break;
	case 4:
		store_name = std::format("{} (F!)", item.full_name);
		item.value = new_val;
		item.ability = 1;
		item.ability_strength = 5;
		break;
	case 5:
		store_name = std::format("{} (+5)", item.full_name);
		item.value = new_val;
		item.bonus += 5;
		break;
	case 6:
		store_name = std::format("{} (B)", item.full_name);
		item.bonus++;
		item.ability = 71;
		item.ability_strength = 5;
		item.magic_use_type = 0;
		item.charges = 8;
		break;
	default:
		store_name = item.full_name;
		break;
	}
	if ((item.value > 15000) || (item.value < 0))
	{
		item.value = 15000;
	}
	strcpy(item.full_name, store_name.c_str());
}
