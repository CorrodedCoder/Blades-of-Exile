
#include <Windows.h>

#include "global.h"
#include "itemdata.h"
#include <cstdio>
#include <cstring>


extern piles_of_stuff_dumping_type2 data_store2;

short loot_min[5] = {0,0,5,50,400};
short loot_max[5] = {3,8,40,800,4000};
//// whole file

item_record_type get_stored_item(short which)
{
	if ((which >= 400) || (which < 0)) {
		return item_record_type{};
	}
	return data_store2.scen_item_list.scen_items[which];
}

item_record_type get_food()
{
	item_record_type food = 
		{11,0, 0,0,0,0,0,0, 62,0,0,0,0,0, 0, 0,0, {0,0},"Food", "Food",0,0,0,0};
	food.graphic_num += get_ran(1,0,2);
	food.item_level = get_ran(1,5,10);
	if (get_ran(1,0,9) == 5)
		food.graphic_num = 113;
	if (get_ran(1,0,9) == 5)
		food.graphic_num = 114;
	// food doesn't always appear
	if (get_ran(1,0,2) != 1)
		food.variety = 0;
	return food;
}


item_record_type pull_item_of_type(short loot_max,short min_val,short max_val,short t1, short t2, short t3)
{
	short i,j,val;
	item_record_type temp_i;
	
	// occasionally get nice item
	if (get_ran(1,0,160) == 80) {
		loot_max += 2;
		max_val += 2000;
		}
	for (i = 0; i < 80; i++) {
		j = get_ran(1,0,399);
		temp_i = get_stored_item(j);
		if ((temp_i.variety == t1) || (temp_i.variety == t2) || (temp_i.variety == t3)) {
			val = (temp_i.charges > 0) ? temp_i.charges * temp_i.value : temp_i.value;
			if ((val >= min_val) && (val <= max_val) && (temp_i.treas_class != 0) &&
				(temp_i.treas_class <= loot_max))
				return temp_i;
			}
		}
	return item_record_type{};
}

item_record_type get_weapon(short loot,short level)
{
	if (loot == 0)
		return item_record_type{};
	return pull_item_of_type(loot,loot_min[loot],loot_max[loot],1,2,-1);
}

item_record_type get_armor(short loot,short level)
{
	if (loot == 0)
		return item_record_type{};
	short r1 = get_ran(1,(loot - 1) * 5 + 124,142);
	return pull_item_of_type(loot,loot_min[loot],loot_max[loot],13,-1,-1); 
}

item_record_type get_helm(short loot)
{
	return pull_item_of_type(loot,loot_min[loot],loot_max[loot],14,-1,-1); 
}

item_record_type get_gloves(short loot)
{
	return pull_item_of_type(loot,loot_min[loot],loot_max[loot],15,-1,-1); 
}

item_record_type get_boots(short loot)
{
	return pull_item_of_type(loot,loot_min[loot],loot_max[loot],17,-1,-1); 
}

item_record_type get_shield(short loot)
{
	return pull_item_of_type(loot,loot_min[loot],loot_max[loot],12,-1,-1); 
}

item_record_type get_potion(short loot)
{
	if (get_ran(1,0,80) < 20 * (4 - loot))
		return pull_item_of_type(loot,loot_min[loot],loot_max[loot] / 2,7,-1,-1);
	return pull_item_of_type(loot,loot_min[loot],loot_max[loot],7,-1,-1); 
}

item_record_type get_scroll(short loot)
{
	return pull_item_of_type(loot,loot_min[loot],loot_max[loot],8,-1,-1); 
}

item_record_type get_missile(short loot)
{
	if (get_ran(1,0,2) < 2)
		return pull_item_of_type(loot,loot_min[loot],loot_max[loot],5,6,4); 
	return pull_item_of_type(loot,loot_min[loot],loot_max[loot],23,24,25); 
}

item_record_type get_poison(short loot,short level)
{
	return pull_item_of_type(loot,loot_min[loot],loot_max[loot],20,-1,-1); 
}

item_record_type get_wand(short loot)
{
	return pull_item_of_type(loot,loot_min[loot],loot_max[loot],9,-1,-1); 
}

item_record_type get_ring(short loot)
{
	return pull_item_of_type(loot,loot_min[loot],loot_max[loot],18,-1,-1); 
}

item_record_type get_necklace(short loot)
{
	return pull_item_of_type(loot,loot_min[loot],loot_max[loot],19,-1,-1); 
} 
