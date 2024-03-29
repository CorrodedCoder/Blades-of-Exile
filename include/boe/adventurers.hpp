#pragma once

#include "boe/game_file_structures.hpp"

bool someone_poisoned(const Adventurers& adventurers);
bool someone_awake(const Adventurers& adventurers);
bool cave_lore_present(const Adventurers & adven);
bool woodsman_present(const Adventurers& adven);
short mage_lore_total(const Adventurers& adven);
bool adventurers_heal(Adventurers& adventurers, short amt);
bool adventurers_cure(Adventurers& adventurers, short amt);
bool adventurers_restore_sp(Adventurers& adventurers, short amt);
bool adventurers_has_ability(const Adventurers& adventurers, short abil);
short adventurers_luck_total(const Adventurers& adventurers);
short adventurers_level_total(const Adventurers& adventurers);
bool adventurers_dead(const Adventurers& adventurers);
short adventurers_count_normal(const Adventurers& adventurers);
bool adventurers_anyone_normal(const Adventurers& adventurers);
