#pragma once

#include "boe/game_file_structures.hpp"

const item_record_type& get_stored_item(short which);
item_record_type get_food(void);
const item_record_type& get_weapon(short loot,short level);
const item_record_type& get_armor(short loot,short level);
const item_record_type& get_helm(short loot);
const item_record_type& get_gloves(short loot);
const item_record_type& get_boots(short loot);
const item_record_type& get_shield(short loot);
const item_record_type& get_potion(short loot);
const item_record_type& get_scroll(short loot);
const item_record_type& get_missile(short loot);
const item_record_type& get_poison(short loot,short level);
const item_record_type& get_wand(short loot);
const item_record_type& get_ring(short loot);
const item_record_type& get_necklace(short loot);
const item_record_type& pull_item_of_type(short loot_max,short min_val,short max_val, item_variety t1, item_variety t2, item_variety t3);
