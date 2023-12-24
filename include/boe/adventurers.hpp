#pragma once

#include "boe/game_file_structures.hpp"

bool cave_lore_present(const Adventurers & adven);
bool woodsman_present(const Adventurers& adven);
short mage_lore_total(const Adventurers& adven);
void adventurers_heal(Adventurers& adventurers, short amt);
bool adventurers_cure(Adventurers& adventurers, short amt);
void adventurers_restore_sp(Adventurers& adventurers, short amt);
bool adventurers_has_ability(const Adventurers& adventurers, short abil);
short adventurers_luck_total(const Adventurers& adventurers);
short adventurers_level_total(const Adventurers& adventurers);
