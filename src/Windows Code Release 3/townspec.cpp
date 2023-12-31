#include <Windows.h>
#include <array>

#include "global.h"
#include "party.h"
#include "items.h"
#include "monster.h"
#include "infodlgs.h"
#include "locutils.h"
#include "text.h"
#include "townspec.h"
#include "exlsound.h"
#include "boe/hacks.hpp"
#include "game_globals.hpp"

extern party_record_type party;
extern current_town_type c_town;
extern short current_pc, stat_window;
extern location center;
extern Adventurers adven;
extern big_tr_type t_d;

static const short trap_odds[30]{
	5,30,35,42,48, 55,63,69,75,77, 78,80,82,84,86, 88,90,92,94,96,98,99,99,99,99,99,99,99,99,99
};

void activate_monster_enc(short enc_num,short str1,short str2,short strsnd,short *flip_bit)
{
	if (*flip_bit == 0)
	{
		display_enc_string(str1,str2,strsnd);
		activate_monsters(enc_num,1);
		*flip_bit = 20;
	}
}

// OTS = One Time Strings
void OTS(short str1b,short str2b,short str12a,short sound,unsigned char *flip_bit)
{
	if (*flip_bit > 0)
	{
		return;
	}

	*flip_bit = 20;

	if (str2b == 0)
	{
		D2ES(str12a, str1b, 0, 0, sound);
	}
	else
	{
		D2ES(str12a, str1b, str12a, str2b, sound);
	}
}

// GFI go for it ... do simple dialog check
Boolean GFI(short dialog_num)
{
	return (FCD(dialog_num, 0) > 1) ? TRUE : FALSE;
}

#if 0
// Not referenced

//DSG = Do Special Give
void DSG(short item_num,unsigned char *flip_bit,short dialog_num,short what_spec,short amt_gold,short amt_food)
//short special;  // 0 - nah  3** - Set spec item 1000 + * give food  2000 + * give cash
{
	short choice;
	Boolean did_give;

	item_record_type item = item_source.stored_item(item_num);
	if (item_num == 0)
		item.variety = 0;
	if (*flip_bit == 0)
	{
		choice = fancy_choice_dialog(dialog_num,0);
		if (choice == 1)
		{
			return;
		}
		if (item.variety == 0)
		{
			did_give = TRUE;
		}
		else
		{
			did_give = give_to_party(item, 0);
		}
		if (did_give == TRUE)
		{
			party.food += amt_food;
			party.gold += amt_gold;
			if (what_spec >= 0)
			{
				if (party.spec_items[what_spec] > 0)
				{
					ASB("You already have this special item.");
					return;
				}
				party.spec_items[what_spec] += 1;
				put_item_screen(stat_window,0);
			}
			*flip_bit = 20;
			put_pc_screen();
		}
		else
		{
			choice = fancy_choice_dialog(1049, 0);
		}
	}
}
#endif

Boolean run_trap(short pc_num,short trap_type,short trap_level,short diff)
//short pc_num; // 6 - BOOM!  7 - pick here
//short trap_type; // 0 - random  1 - blade  2 - dart  3 - gas  4 - boom  5,6  - no   
				 // 7 - level drain  8 - alert  9 - big flames 10 - dumbfound 11 - town hostile
				 //	20 + *  - trap *, but nasty 
{
	short r1,skill,i,num_hits = 1,i_level;
	
	if (pc_num > 7) // Debug
	{
		SysBeep(50);
		ASB("TRAP ERROR! REPORT!");
		return TRUE;
	}
	
	if (pc_num == 7)
	{
		pc_num = select_pc(1,0);
		if (pc_num == 6)
		{
			return FALSE;
		}
	}

	num_hits += trap_level;
		
	if (trap_type == 0)
		trap_type = rand_short(1,4);

	if ((trap_type ==  5) || (trap_type == 6))
		return TRUE;
		
	if (pc_num < 6)
	{
		i = pc_stat_adj(adven[pc_num], skill::Dexterity);
		if ((i_level = pc_prot_level(adven[pc_num], 42)) > 0)
		{
			i = i + i_level / 2;
		}
		skill = boe_clamp(adven[pc_num].skills[skill::DisarmTraps] + 
				+ adven[pc_num].skills[skill::Luck] / 2 + 1 - c_town.difficulty + 2 * i,0,20);

		r1 = rand_short(0,100) + diff;
		// Nimble?
		if (adven[pc_num].traits[trait::NimbleFingers] == FALSE)
		{
			r1 -= 6;
		}

		if (r1 < trap_odds[skill])
		{
			add_string_to_buf("  Trap disarmed.            ");
			return TRUE;
		}

		add_string_to_buf("  Disarm failed.          ");
	}
	
	switch (trap_type)
	{
	case 1:
		for (i = 0; i < num_hits; i++)
		{
			add_string_to_buf("  A knife flies out!              ");
			r1 = get_ran(2 + c_town.difficulty / 14,1,10);
			damage_pc(pc_num,r1, damage_type::Weapon,-1);
		}
		break;
			
	case 2:
		add_string_to_buf("  A dart flies out.              ");
		r1 = 3 + c_town.difficulty / 14;
		r1 = r1 + trap_level * 2;
		poison_pc(pc_num,r1);
		break;
			
	case 3:
		add_string_to_buf("  Poison gas pours out.          ");
		r1 = 2 + c_town.difficulty / 14;
		r1 = r1 + trap_level * 2;
		for (i = 0; i < 6; i++)
		{
			poison_pc(i, r1);
		}
		break;
	
	case 4: 
		for (i = 0; i < num_hits; i++)
		{
			add_string_to_buf("  There is an explosion.        ");
			r1 = get_ran(3 + c_town.difficulty / 13,1,8);
			hit_party(r1, damage_type::Fire);
		}
		break;
			
	case 5:
		add_string_to_buf("  A purple ray flies out.          ");
		r1 = 200 + c_town.difficulty * 100;
		r1 = r1 + trap_level * 400;
		sleep_pc(pc_num,r1, affect::Paralyzed,50);
		break;

	case 7:
		add_string_to_buf("  You feel weak.            ");
		r1 = 40;
		r1 = r1 + trap_level * 30;
		adven[pc_num].experience = max (0,adven[pc_num].experience - r1);
		break;

	case 8:
		add_string_to_buf("  An alarm goes off!!!            ");
		make_town_hostile();
		break;

	case 9:
		add_string_to_buf("  Flames shoot from the walls.        ");
		r1 = get_ran(10 + trap_level * 5,1,8);
		hit_party(r1, damage_type::Fire);
		break;

	case 10:
		add_string_to_buf("  You feel disoriented.        ");
		for (i = 0; i < 6; i++)
		{
			dumbfound_pc(i, 2 + trap_level * 2);
		}
		break;

	case 11:
		add_string_to_buf("  You prick your finger. ");
		r1 = 3 + c_town.difficulty / 14;
		r1 = r1 + trap_level * 2;
		disease_pc(pc_num,r1);
		break;

	case 12:
		add_string_to_buf("  A foul substance sprays out.");
		r1 = 2 + c_town.difficulty / 14;
		r1 = r1 + trap_level * 2;
		for (i = 0; i < 6; i++)
		{
			disease_pc(i, r1);
		}
		break;
	}
	put_pc_screen();
	put_item_screen(stat_window,0);
	return TRUE;
}

location get_spec_loc(short which)
{
	for (short i = 0; i < 50; i++)
	{
		if (c_town.town.spec_id[i] == which)
		{
			return c_town.town.special_locs[i];
		}
	}
	return {};
}

void start_split(short a,short b,short noise) 
{
	party.stuff_done[304][0] = 1;
	party.stuff_done[304][1] = c_town.p_loc.x;
	party.stuff_done[304][2] = c_town.p_loc.y;
	c_town.p_loc.x = a;
	c_town.p_loc.y = b;
	for (short i = 0; i < 6; i++)
	{
		if (i != party.stuff_done[304][3])
		{
			hacks_adventurer_set_split_status(adven[i]);
		}
	}
	current_pc = party.stuff_done[304][3];
	update_explored(c_town.p_loc);
	center = c_town.p_loc;
	if (noise > 0)
	{
		play_sound(10);
	}
}

void end_split(short noise) 
{
	if (party.stuff_done[304][0] == 0)
	{
		ASB("Party already together!");
		return;
	}
	c_town.p_loc.x = party.stuff_done[304][1];
	c_town.p_loc.y = party.stuff_done[304][2];
	party.stuff_done[304][0] = 0;
	for (short i = 0; i < 6; i++)
	{
		hacks_adventurer_reset_split_status(adven[i]);
	}
	update_explored(c_town.p_loc);
	center = c_town.p_loc;
	if (noise > 0)
	{
		play_sound(10);
	}
	add_string_to_buf("You are reunited.");
}

// 0 if no pull.
// 1 if in right position now.
// 2 if in left position
// levers should always start to left.
short handle_lever(location w)
{
	if (FCD(1020, 0) == 1)
	{
		return 0;
	}
	play_sound(94);
	switch_lever(w);
	return 1;
}

void switch_lever(location w)
{
	alter_space(w.x,w.y, scenario.ter_type(t_d.terrain[w.x][w.y]).trans_to_what);
}
