#pragma once

#include "boe/game_file_structures.hpp"

void pc_setup_blank(pc_record_type& pc);
void pc_setup_debug(pc_record_type& pc, short num);
void pc_setup_prefab(pc_record_type& pc, short num);
void pc_heal(pc_record_type& pc, short amt);
bool pc_cure(pc_record_type& pc, short amt);
void pc_restore_sp(pc_record_type& pc, short amt);
short pc_encumberance(const pc_record_type& pc);
short pc_get_tnl(const pc_record_type& pc);
short pc_could_accept(const pc_record_type& pc, const item_record_type& item);

bool pc_has_cave_lore(const pc_record_type& pc);
bool pc_has_woodsman(const pc_record_type& pc);
short pc_has_space(const pc_record_type& pc);
short pc_prot_level(const pc_record_type& pc, short abil);
short pc_has_abil_equip(const pc_record_type& pc, short abil);
short pc_has_abil(const pc_record_type& pc, short abil);
short pc_amount_can_carry(const pc_record_type& pc);
void pc_sort_items(pc_record_type& pc);
bool pc_affect(pc_record_type& pc, affect type, short how_much);
short pc_carry_weight(const pc_record_type& pc);
short pc_luck(const pc_record_type& pc);
short pc_level(const pc_record_type& pc);
short pc_combat_encumberance(const pc_record_type& pc);
short pc_calculate_moves(const pc_record_type& pc, int party_age);

bool pc_acid(pc_record_type& pc, short how_much);
bool pc_web(pc_record_type& pc, short how_much);
bool pc_slow(pc_record_type& pc, short how_much);
bool pc_paralyze(pc_record_type& pc, short how_much, short adjust);
bool pc_sleep(pc_record_type& pc, short how_much, short adjust);
bool pc_disease(pc_record_type& pc, short how_much);
bool pc_dumbfound(pc_record_type& pc, short how_much, short modifier);
bool pc_curse(pc_record_type& pc, short how_much);
bool pc_poison(pc_record_type& pc, short how_much);

short pc_damage_adjust(const pc_record_type& pc, short how_much, damage_type type, short type_of_attacker, short parry_modifier, short party_adjust);
short pc_stat_adj(const pc_record_type& pc, skill which);
bool pc_can_cast_spell_ex(const pc_record_type& pc, short type, short spell_num);

short skill_hit_chance(short type);
short skill_bonus(short type);
