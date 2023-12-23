#pragma once

#include "boe/game_file_structures.hpp"

short item_weight(const item_record_type& item);
short item_val(const item_record_type& item);
bool item_is_weapon(const item_record_type& item);
bool is_ident(const item_record_type& item);
bool is_magic(const item_record_type& item);
bool is_contained(const item_record_type& item);
bool is_cursed(const item_record_type& item);
bool is_property(const item_record_type& item);

Boolean equippable(item_variety variety);
short num_hands_to_use(item_variety variety);
short num_that_can_equip(item_variety variety);
short excluding_types(item_variety variety);
