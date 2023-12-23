#include "boe/item.hpp"

short item_weight(const item_record_type& item)
{
	if (item.variety == 0)
	{
		return 0;
	}
	if ((item.variety == 5) || (item.variety == 6) || (item.variety == 24) || (item.variety == 7)
		|| ((item.variety == 21) && (item.charges > 0)))
	{
		return static_cast<short>(item.charges) * static_cast<short>(item.weight);
	}
	return static_cast<short>(item.weight);
}

bool item_is_weapon(const item_record_type& item)
{
	return (item.variety == 1) || (item.variety == 2) || (item.variety == 5) || (item.variety == 24);
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
