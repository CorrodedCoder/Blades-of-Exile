item_record_type	return_dummy_item();
item_record_type get_stored_item(short which);
item_record_type get_food();
item_record_type	get_weapon(short loot,short level);
item_record_type	get_armor(short loot,short level);
item_record_type get_helm(short loot);
item_record_type get_gloves(short loot);
item_record_type get_boots(short loot);
item_record_type	get_shield(short loot);
item_record_type	get_potion(short loot);
item_record_type	get_scroll(short loot);
item_record_type	get_missile(short loot);
item_record_type	get_poison(short loot,short level);
item_record_type get_wand(short loot);
item_record_type get_ring(short loot);
item_record_type get_necklace(short loot);
item_record_type pull_item_of_type(short loot_max,short min_val,short max_val,short t1, short t2, short t3);
