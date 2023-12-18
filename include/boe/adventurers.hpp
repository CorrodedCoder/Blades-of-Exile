#pragma once

#include "boe/game_file_structures.hpp"

bool cave_lore_present(const Adventurers & adven);
bool woodsman_present(const Adventurers& adven);
short mage_lore_total(const Adventurers& adven);
void adventurers_heal(Adventurers& adventurers, short amt);
bool adventurers_cure(Adventurers& adventurers, short amt);
void restore_sp_party(Adventurers& adventurers, short amt);
