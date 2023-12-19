#include "boe/pc.hpp"
#include <algorithm>

void pc_heal(pc_record_type& pc, short amt)
{
	if ((pc.main_status == 1) && (pc.cur_health < pc.max_health))
	{
		pc.cur_health = std::min(static_cast<short>(pc.cur_health + amt), pc.max_health);
	}
}

bool pc_cure(pc_record_type& pc, short amt)
{
	if (pc.main_status != 1)
	{
		return false;
	}
	pc.status[2] = std::max(static_cast<short>(pc.status[2] - amt), static_cast<short>(0));
	return true;
}

void pc_restore_sp(pc_record_type& pc, short amt)
{
	if (pc.cur_sp < pc.max_sp)
	{
		pc.cur_sp = std::min(static_cast<short>(pc.cur_sp + amt), pc.max_sp);
	}
}

void pc_setup_blank(pc_record_type& pc)
{
	pc_record_type	dummy_pc;
	short i;

	dummy_pc.main_status = 0;
	dummy_pc.name[0] = '\n';
	dummy_pc.name[1] = '\0';

	for (i = 0; i < 30; i++)
		dummy_pc.skills[i] = (i < 3) ? 1 : 0;
	dummy_pc.cur_health = 6;
	dummy_pc.max_health = 6;
	dummy_pc.cur_sp = 0;
	dummy_pc.max_sp = 0;
	dummy_pc.experience = 0;
	dummy_pc.skill_pts = 60;
	dummy_pc.level = 1;
	for (i = 0; i < 15; i++)
		dummy_pc.status[i] = 0;
	for (i = 0; i < 24; i++)
		dummy_pc.items[i] = item_record_type{};
	for (i = 0; i < 24; i++)
		dummy_pc.equip[i] = BOE_FALSE;

	for (i = 0; i < 62; i++) {
		dummy_pc.priest_spells[i] = (i < 30) ? BOE_TRUE : BOE_FALSE;
		dummy_pc.mage_spells[i] = (i < 30) ? BOE_TRUE : BOE_FALSE;
	}
	dummy_pc.which_graphic = 0;
	dummy_pc.weap_poisoned = 24;

	for (i = 0; i < 15; i++) {
		dummy_pc.advan[i] = BOE_FALSE;
		dummy_pc.traits[i] = BOE_FALSE;
	}
	dummy_pc.race = 0;
	dummy_pc.exp_adj = 100;
	dummy_pc.direction = 0;

	pc = dummy_pc;
}

void pc_setup_debug(pc_record_type& pc, short num)
{
	pc_record_type	dummy_pc;
	short i;

	dummy_pc.main_status = 1;
	switch (num) {
	case 0:	strncpy(dummy_pc.name, "Gunther", 20);
		break;
	case 1:	strncpy(dummy_pc.name, "Yanni", 20);
		break;
	case 2:	strncpy(dummy_pc.name, "Mandolin", 20);
		break;
	case 3:	strncpy(dummy_pc.name, "Pete", 20);
		break;
	case 4:	strncpy(dummy_pc.name, "Vraiment", 20);
		break;
	case 5:	strncpy(dummy_pc.name, "Goo", 20);
		break;

	}

	for (i = 0; i < 30; i++)
		dummy_pc.skills[i] = (i < 3) ? 20 : 8;
	dummy_pc.cur_health = 60;
	dummy_pc.max_health = 60;
	dummy_pc.cur_sp = 90;
	dummy_pc.max_sp = 90;
	dummy_pc.experience = 0;
	dummy_pc.skill_pts = 60;
	dummy_pc.level = 1;
	for (i = 0; i < 15; i++)
		dummy_pc.status[i] = 0;
	for (i = 0; i < 24; i++)
		dummy_pc.items[i] = item_record_type{};
	for (i = 0; i < 24; i++)
		dummy_pc.equip[i] = BOE_FALSE;

	for (i = 0; i < 62; i++) {
		dummy_pc.priest_spells[i] = BOE_TRUE;
		dummy_pc.mage_spells[i] = BOE_TRUE;
	}
	//	dummy_pc.which_graphic = num * 3 + 1;
	dummy_pc.which_graphic = num + 4;
	dummy_pc.weap_poisoned = 16;

	for (i = 0; i < 15; i++) {
		dummy_pc.advan[i] = BOE_FALSE;
		dummy_pc.traits[i] = BOE_FALSE;
	}

	dummy_pc.race = 0;
	dummy_pc.exp_adj = 100;
	dummy_pc.direction = 0;

	pc = dummy_pc;
}

void pc_setup_prefab(pc_record_type& pc, short num)
{
	pc_record_type	dummy_pc;
	short i;
	short pc_stats[6][19] = { {8,6,2, 6,0,0,0,0,0, 0,0,0,0,1, 0,0,2,0,0},
						{8,7,2, 0,0,6,3,0,3, 0,0,0,0,0, 0,0,0,2,0},
						{8,6,2, 3,3,0,0,2,0, 0,0,0,0,0, 4,4,0,2,1},
						{3,2,6, 2,0,0,2,0,0, 3,0,3,0,1, 0,0,0,0,0},
						{2,2,6, 3,0,0,2,0,0,  2,1,4,0,0, 0,0,0,0,1},
						{2,2,6, 0,2,0,2,0,1, 0,3,3,2,0, 0,0,0,0,0} };
	short pc_health[6] = { 22,24,24,16,16,18 };
	short pc_sp[6] = { 0,0,0,20,20,21 };
	short pc_graphics[6] = { 3,32,29,16,23,14 };
	short pc_race[6] = { 0,2,1,0,0,0 };
	short pc_t[6][15] = { {0,0,1,0,0,0,1,0,0,0, 0,1,0,0,0},
						{1,0,0,0,0,1,0,0,0,0, 1,0,0,0,0},
						{0,0,0,1,0,0,0,0,0,0, 0,0,1,0,0},
						{0,1,0,0,0,0,0,0,0,0, 0,0,0,0,0},
						{0,0,0,0,1,0,1,1,0,0, 0,0,0,0,1},
						{0,1,0,0,0,0,0,0,0,0, 0,0,0,0,0} };


	dummy_pc.main_status = 1;
	switch (num) {
	case 0:	strncpy(dummy_pc.name, "Jenneke", 20);
		break;
	case 1:	strncpy(dummy_pc.name, "Thissa", 20);
		break;
	case 2:	strncpy(dummy_pc.name, "Frrrrrr", 20);
		break;
	case 3:	strncpy(dummy_pc.name, "Adrianna", 20);
		break;
	case 4:	strncpy(dummy_pc.name, "Feodoric", 20);
		break;
	case 5:	strncpy(dummy_pc.name, "Michael", 20);
		break;

	}

	for (i = 0; i < 19; i++)
		dummy_pc.skills[i] = pc_stats[num][i];
	dummy_pc.cur_health = pc_health[num];
	dummy_pc.max_health = pc_health[num];
	dummy_pc.experience = 0;
	dummy_pc.skill_pts = 0;
	dummy_pc.level = 1;

	for (i = 0; i < 15; i++)
		dummy_pc.status[i] = 0;
	for (i = 0; i < 24; i++)
		dummy_pc.items[i] = item_record_type{};
	for (i = 0; i < 24; i++)
		dummy_pc.equip[i] = BOE_FALSE;
	dummy_pc.cur_sp = pc_sp[num];
	dummy_pc.max_sp = pc_sp[num];
	for (i = 0; i < 62; i++) {
		dummy_pc.priest_spells[i] = (i < 30) ? BOE_TRUE : BOE_FALSE;
		dummy_pc.mage_spells[i] = (i < 30) ? BOE_TRUE : BOE_FALSE;
	}
	for (i = 0; i < 15; i++) {
		dummy_pc.traits[i] = static_cast<Boolean>(pc_t[num][i]);
		dummy_pc.advan[i] = BOE_FALSE;
	}

	dummy_pc.race = pc_race[num];
	dummy_pc.exp_adj = 100;
	dummy_pc.direction = 0;

	dummy_pc.which_graphic = pc_graphics[num];

	pc = dummy_pc;
}
