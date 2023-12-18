#pragma once

#include "boe/game_file_structures.hpp"

bool cave_lore_present(const Adventurers & adven);
bool woodsman_present(const Adventurers& adven);
short mage_lore_total(const Adventurers& adven);
void heal_party(Adventurers& adventurers, short amt);
