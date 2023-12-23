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
