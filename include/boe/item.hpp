#pragma once

#include "boe/game_file_structures.hpp"

short item_weight(const item_record_type& item);
bool item_is_weapon(const item_record_type& item);
bool is_ident(item_record_type item);
bool is_magic(item_record_type item);
bool is_contained(item_record_type item);
bool is_cursed(item_record_type item);
bool is_property(item_record_type item);
