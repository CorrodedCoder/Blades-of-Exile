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

short pc_has_space(const pc_record_type& pc);
short pc_prot_level(const pc_record_type& pc, short abil);
short pc_has_abil_equip(const pc_record_type& pc, short abil);
short pc_has_abil(const pc_record_type& pc, short abil);
short pc_amount_can_carry(const pc_record_type& pc);
void pc_sort_items(pc_record_type& pc);
bool pc_affect(pc_record_type& pc, affect type, short how_much);
short pc_carry_weight(const pc_record_type& pc);
