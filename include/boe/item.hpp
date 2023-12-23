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
