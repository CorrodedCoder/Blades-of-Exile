#include <Windows.h>
#include <array>
#include <span>

#include "global.h"
#include "graphics.h"
#include "text.h"
#include "dlogtool.h"
#include "items.h"
#include "party.h"
#include "fields.h"
#include "locutils.h"
#include "newgraph.h"
#include "infodlgs.h"
#include "exlsound.h"
#include <cstdio>
#include <cstring>
#include "graphutl.h"
#include "monster.h"
#include "boe/utility.hpp"
#include "boe/item.hpp"
#include "game_globals.hpp"

static const short c_treas_chart[5][6]{
	{0,-1,-1,-1,-1,-1},
	{1,-1,-1,-1,-1,-1},
	{2,1,1,-1,-1,-1},
	{3,2,1,1,-1,-1},
	{4,3,2,2,1,1}
};
static const short c_treas_odds[5][6]{
	{10,0,0,0,0,0},
	{50,0,0,0,0,0},
	{60,50,40,0,0,0},
	{100,90,80,70,0,0},
	{100,80,80,75,75,75}
};
static const short c_id_odds[21]{
	0,10,15,20,25,30,35,39,43,47, 
	51,55,59,63,67,71,73,75,77,79,81
};
static const short c_max_mult[5][10]{
	{0,0,0,0,0,0,0,0,0,1},
	{0,0,1,1,1,1,2,3,5,20},
	{0,0,1,1,2,2,4,6,10,25},
	{5,10,10,10,15,20,40,80,100,100},
	{25,25,50,50,50,100,100,100,100,100}
};
static const short c_min_chart[5][10]{
	{0,0,0,0,0,0,0,0,0,1},
	{0,0,0,0,0,0,0,0,5,20},
	{0,0,0,0,1,1,5,10,15,40},
	{10,10,15,20,20,30,40,50,75,100},
	{50,100,100,100,100,200,200,200,200,200}
};

extern short stat_window,overall_mode,current_cursor,which_combat_type,current_pc;
extern party_record_type party;
extern talking_record_type talking;

extern current_town_type c_town;
extern town_item_list 	t_i;
extern HWND mainPtr;
extern Boolean in_startup_mode,boom_anim_active;
extern HINSTANCE store_hInstance;

extern Adventurers adven;
extern big_tr_type  t_d;
extern location pc_pos[6];

extern HCURSOR sword_curs;
extern Boolean modeless_exists[18],diff_depth_ok;
extern short modeless_key[18];
extern HWND modeless_dialogs[18];
extern short town_size[3];
extern short town_type;
extern DLGPROC modeless_dlogprocs[18];
extern short dialog_answer;
extern Boolean cd_event_filter();
extern Boolean dialog_not_toast;
extern HBITMAP pcs_gworld;
extern HDC main_dc,main_dc2,main_dc3;

static short item_max = 0;

short first_item_shown,store_get_mode,current_getting_pc,store_pcnum,total_items_gettable; // these 5 used for get items dialog
short item_array[130]; // NUM_TOWN_ITEMS + a bit

short answer_given = 1;

static char *store_str;
short store_dnum;

HWND test_dlog3;
HWND store_focus;
DLGPROC dlog_proc1;

short being_created;
short procinst_exists[18] = {0,0,0,0,0, 0,0,0,0,0 ,0,0,0,0,0, 0,0,0};

static short display_item(location from_loc, short pc_num, short mode, bool check_container);

Boolean give_to_pc(short pc_num,const item_record_type& item,short  print_result)
{
	if (item.variety == item_variety::None)
		return TRUE;
	if (item.variety == item_variety::Gold) {
		party.gold += item.item_level;
		ASB("You get some gold.");
		return TRUE;
		}
	if (item.variety == item_variety::Food) {
		party.food += item.item_level;
		ASB("You get some food.");
		return TRUE;
		}
	if (item_weight(item) > 
	  pc_amount_can_carry(adven[pc_num]) - pc_carry_weight(adven[pc_num])) {
	  	if (print_result == TRUE) {
		  	SysBeep(20);
			ASB("Item too heavy to carry.");
			}
		return FALSE;
	  	}
	const auto free_space{ pc_has_space(adven[pc_num]) };
	if ((free_space == 24) || (adven[pc_num].main_status != status_type::Normal))
		return FALSE;
	else
	{
			adven[pc_num].items[free_space] = item;
			adven[pc_num].items[free_space].item_properties &= 253; // not property
			adven[pc_num].items[free_space].item_properties &= 247; // not contained
			if (print_result == 1)
			{
				if (stat_window == pc_num)
					put_item_screen(stat_window,0);
			}
			if ( (in_startup_mode == FALSE) && (print_result == TRUE) )
			{
				if (is_ident(adven[pc_num].items[free_space]) == 0)
					add_string_to_buf("  {} gets {}.",adven[pc_num].name,item.name);
				else
					add_string_to_buf("  {} gets {}.",adven[pc_num].name,item.full_name);
			}
			combine_things(pc_num);
			pc_sort_items(adven[pc_num]);
			return TRUE;
	}
	return FALSE;
}

Boolean give_to_party(const item_record_type& item,short print_result)
{
	short i = 0;
	
	while (i < 6) {
		if (give_to_pc(i,item,print_result) == TRUE)
			return TRUE;
		i++;
	}
	return FALSE;
}

Boolean forced_give(short item_num,short abil)
// if abil > 0, force abil, else ignore
{
	short i,j;
	item_record_type item;

	if ((item_num < 0) || (item_num > 399))
		return TRUE;
	item = item_source.stored_item(item_num);
	if (abil > 0)
		item.ability = abil;
	for (i = 0; i < 6; i++)
		for (j = 0; j < 24; j++)
			if ((adven[i].main_status == status_type::Normal) && (adven[i].items[j].variety == item_variety::None)) {
				adven[i].items[j] = item;
				if (is_ident(item) == 0)
					add_string_to_buf("  {} gets {}.",adven[i].name,item.name);
				else
					add_string_to_buf("  {} gets {}.",adven[i].name,item.full_name);
				combine_things(i);
				pc_sort_items(adven[i]);
				return TRUE;
				}
	return FALSE;
}

Boolean GTP(short item_num)
{
	item_record_type item;

	item = item_source.stored_item(item_num);
	return give_to_party(item,TRUE);
}
Boolean silent_GTP(short item_num)
{
	item_record_type item;
	
	item = item_source.stored_item(item_num);
	return give_to_party(item,FALSE);
}
void give_gold(short amount,Boolean print_result)
{
	if (amount < 0) return;
	party.gold = party.gold + amount;
	if (print_result == TRUE)
		put_pc_screen();
}

Boolean take_gold(short amount,Boolean print_result)
{
	if (party.gold < amount)
		return FALSE;
	party.gold = party.gold - amount;
	if (print_result == TRUE)
		put_pc_screen();
	return TRUE;
}

Boolean party_take_abil(short abil)
{
	short i,item;
	
	for (i = 0; i < 6; i++)
		if (adven[i].main_status == status_type::Normal)
			if ((item = pc_has_abil(adven[i],abil)) < 24) {
				if (adven[i].items[item].charges > 1)
					adven[i].items[item].charges--;
					else take_item(i,item);
				return TRUE;
				}
	return FALSE;
}

void give_food(short amount,Boolean print_result)
{
	if (amount < 0) return;
	party.food = party.food + amount;
	if (print_result == TRUE)
		put_pc_screen();	
}

short take_food(short amount,Boolean print_result)
{
	short diff;

	diff = amount - party.food;
	if (diff > 0) {
		party.food = 0;
		if (print_result == TRUE)
			put_pc_screen();
		return diff;
		}

	party.food = party.food - amount;
	if (print_result == TRUE)
		put_pc_screen();	
	return 0;	
}

// returns 1 if OK, 2 if no room, 3 if not enough cash, 4 if too heavy, 5 if too many of item
short pc_ok_to_buy(const pc_record_type& pc, short cost, const item_record_type& item)
{
	const short could_accept = pc_could_accept(pc, item);
	if (could_accept == 1)
	{
		if (take_gold(cost, FALSE) == FALSE)
			return 3;
	}
	return could_accept;
}

void take_item(short pc_num,short which_item)
//short pc_num,which_item;  // if which_item > 30, don't update stat win, item is which_item - 30
{
	Boolean do_print = TRUE;
	
	if (which_item >= 30)
	{
		do_print = FALSE;
		which_item -= 30;
	}

	if (pc_remove_item(adven[pc_num], which_item))
	{
		add_string_to_buf("  Poison lost.           ");
	}
	
	if ((stat_window == pc_num) && (do_print == TRUE))
		put_item_screen(stat_window,1);
}

void remove_charge(short pc_num,short which_item)
{
	if (adven[pc_num].items[which_item].charges > 0) {
			adven[pc_num].items[which_item].charges--;
			if (adven[pc_num].items[which_item].charges == 0) {
				take_item(pc_num,which_item);
				}
		}

	if (stat_window == pc_num)
		put_item_screen(stat_window,1);

}

void equip_item(short pc_num, short item_num)
{
	short num_equipped_of_this_type = 0;
	short num_hands_occupied = 0;
	short i;
	short equip_item_type = 0;

	if ((overall_mode == 10) && (adven[pc_num].items[item_num].variety == item_variety::Food))
	{
		add_string_to_buf("Equip: Not in combat");
	}
	else
	{
		// unequip
		if (adven[pc_num].equip[item_num] == TRUE)
		{
			if ((adven[pc_num].equip[item_num] == TRUE) && is_cursed(adven[pc_num].items[item_num]))
			{
				add_string_to_buf("Equip: Item is cursed.           ");
			}
			else
			{
				adven[pc_num].equip[item_num] = FALSE;
				add_string_to_buf("Equip: Unequipped");
				if ((adven[pc_num].weap_poisoned == item_num) && (adven[pc_num].gaffect(affect::PoisonedWeapon) > 0))
				{
					add_string_to_buf("  Poison lost.           ");
					adven[pc_num].gaffect(affect::PoisonedWeapon) = 0;
				}
			}
		}
		else // equip
		{
			if (!equippable(adven[pc_num].items[item_num].variety))
			{
				add_string_to_buf("Equip: Can't equip this item.");
			}
			else
			{
				for (i = 0; i < 24; i++)
				{
					if (adven[pc_num].equip[i] == TRUE)
					{
						if (adven[pc_num].items[i].variety == adven[pc_num].items[item_num].variety)
						{
							num_equipped_of_this_type++;
						}
						num_hands_occupied = num_hands_occupied + num_hands_to_use(adven[pc_num].items[i].variety);
					}
				}

				equip_item_type = excluding_types(adven[pc_num].items[item_num].variety);
				// Now if missile is already equipped, no more missiles
				if (equip_item_type > 0)
				{
					for (i = 0; i < 24; i++)
					{
						if ((adven[pc_num].equip[i] == TRUE) && (excluding_types(adven[pc_num].items[i].variety) == equip_item_type))
						{
							add_string_to_buf("Equip: You have something of");
							add_string_to_buf("  this type equipped.");
							return;
						}
					}
				}

				if (is_combat() && (adven[pc_num].items[item_num].variety == item_variety::Armor))
				{
					add_string_to_buf("Equip: Not armor in combat");
				}
				else if ((2 - num_hands_occupied) < num_hands_to_use(adven[pc_num].items[item_num].variety))
				{
					add_string_to_buf("Equip: Not enough free hands");
				}
				else if (num_that_can_equip(adven[pc_num].items[item_num].variety) <= num_equipped_of_this_type)
				{
					add_string_to_buf("Equip: Can't equip another");
				}
				else
				{
					adven[pc_num].equip[item_num] = TRUE;
					add_string_to_buf("Equip: OK");
				}
			}
		}
	}
	if (stat_window == pc_num)
	{
		put_item_screen(stat_window, 1);
	}
}


void drop_item(short pc_num,short item_num,location where_drop)
{

	short choice,how_many = 0;
	item_record_type item_store;
	Boolean take_given_item = TRUE;
	location loc;
	
	item_store = adven[pc_num].items[item_num];

	if ((adven[pc_num].equip[item_num] == TRUE) &&
		is_cursed(adven[pc_num].items[item_num])) 
			add_string_to_buf("Drop: Item is cursed.           ");	
	else switch (overall_mode) {
		case 0:
			choice = fancy_choice_dialog(1093,0);
			if (choice == 1)
				return;
			add_string_to_buf("Drop: OK");
			if ((item_store.type_flag > 0) && (item_store.charges > 1)) {
				how_many = get_num_of_items(item_store.charges);
				if (how_many == item_store.charges)
					take_item(pc_num,item_num);
					else adven[pc_num].items[item_num].charges -= how_many;
				}
				else take_item(pc_num,item_num);
			break;
		
		case 5: case 15:
			loc = where_drop;
			if ((item_store.type_flag > 0) && (item_store.charges > 1)) {
				how_many = get_num_of_items(item_store.charges);
				if (how_many <= 0)
					return;
				if (how_many < item_store.charges)
					take_given_item = FALSE;
				item_store.charges = how_many;
				}
			if (is_container(loc))
				item_store.item_properties = item_store.item_properties | 8;
			if (place_item(item_store,loc,FALSE) == FALSE) {
				add_string_to_buf("Drop: Too many items on ground");
				item_store.item_properties = item_store.item_properties & 247; // not contained
				}
				else {
					if (is_contained(item_store))
						add_string_to_buf("Drop: Item put away");
						else add_string_to_buf("Drop: OK");
					adven[pc_num].items[item_num].charges -= how_many;
					if (take_given_item)
						take_item(pc_num,item_num);
					}	
			break;
		}
}

Boolean place_item(const item_record_type& item,location where,Boolean forced)
{
	short i;
	
	for (i = 0; i < NUM_TOWN_ITEMS; i++)
		if (t_i.items[i].variety == item_variety::None) {
			t_i.items[i] = item;
			t_i.items[i].item_loc = where;
			reset_item_max();
			return TRUE;
			}
	if (forced == FALSE)
		return FALSE;
	destroy_an_item();
	for (i = 0; i < NUM_TOWN_ITEMS; i++)
		if (t_i.items[i].variety == item_variety::None) {
			t_i.items[i] = item;
			t_i.items[i].item_loc = where;
			reset_item_max();
			return TRUE;
			}
	
	return TRUE;
}

void destroy_an_item()
{
////
	short i;
	ASB("Too many items. Some item destroyed.");
	for (i = 0; i < NUM_TOWN_ITEMS; i++)
		if (t_i.items[i].type_flag == 15) {
			t_i.items[i].variety = item_variety::None;
			return;
			}
	for (i = 0; i < NUM_TOWN_ITEMS; i++)
		if (t_i.items[i].value < 3) {
			t_i.items[i].variety = item_variety::None;
			return;
			}
	for (i = 0; i < NUM_TOWN_ITEMS; i++)
		if (t_i.items[i].value < 30) {
			t_i.items[i].variety = item_variety::None;
			return;
			}
	for (i = 0; i < NUM_TOWN_ITEMS; i++)
		if (!is_magic(t_i.items[i])) {
			t_i.items[i].variety = item_variety::None;
			return;
			}
	i = rand_short(0,NUM_TOWN_ITEMS);
	t_i.items[i].variety = item_variety::None;
	
}

void give_thing(short pc_num, short item_num)
{
	short who_to,how_many = 0;
	item_record_type item_store;
	Boolean take_given_item = TRUE;
	
	if ((adven[pc_num].equip[item_num] == TRUE) &&
			is_cursed(adven[pc_num].items[item_num]))
			add_string_to_buf("Give: Item is cursed.           ");
  			else {
				item_store = adven[pc_num].items[item_num];
				who_to = char_select_pc(1,1,"Give item to who?");
				if ((overall_mode == 10) && (adjacent(pc_pos[pc_num],pc_pos[who_to]) == FALSE)) {
					add_string_to_buf("Give: Must be adjacent.");
					who_to = 6;
					}

				if ((who_to < 6) && (who_to != pc_num) 
					&& ((overall_mode != 10) || (adjacent(pc_pos[pc_num],pc_pos[who_to]) == TRUE))) {
					if ((item_store.type_flag > 0) && (item_store.charges > 1)) {
						how_many = get_num_of_items(item_store.charges);
						if (how_many == 0)
							return;
						if (how_many < item_store.charges)
							take_given_item = FALSE;
						adven[pc_num].items[item_num].charges -= how_many;
						item_store.charges = how_many;
						}					
					if (give_to_pc(who_to,item_store,0) == TRUE) {
						if (take_given_item)
							take_item(pc_num,item_num);	
						}
						else {
							if (pc_has_space(adven[who_to]) == 24)
								ASB("Can't give: PC has max. # of items.");
								else ASB("Can't give: PC carrying too much.");
							if (how_many > 0)
								adven[pc_num].items[item_num].charges += how_many;
							}				
				}
		}
}

void combine_things(short pc_num)
{
	short i,j,test;
	
	for (i = 0; i < 24; i++) {
		if ((adven[pc_num].items[i].variety > item_variety::None) &&
			(adven[pc_num].items[i].type_flag > 0) && (is_ident(adven[pc_num].items[i]))) {
			for (j = i + 1; j < 24; j++)
				if ((adven[pc_num].items[j].variety > item_variety::None) &&
				(adven[pc_num].items[j].type_flag == adven[pc_num].items[i].type_flag) 
				 && (is_ident(adven[pc_num].items[j]))) {
					add_string_to_buf("(items combined)");
					test = (short) (adven[pc_num].items[i].charges) + (short) (adven[pc_num].items[j].charges);
					if (test > 125) {
						adven[pc_num].items[i].charges = 125;
						ASB("(Can have at most 125 of any item.");
						}
				 		else adven[pc_num].items[i].charges += adven[pc_num].items[j].charges;
				 	if (adven[pc_num].equip[j] == TRUE) {
				 		adven[pc_num].equip[i] = TRUE;
				 		adven[pc_num].equip[j] = FALSE;
				 		}
					take_item(pc_num,30 + j);
				 	}
			}		
		if ((adven[pc_num].items[i].variety > item_variety::None) && (adven[pc_num].items[i].charges < 0))
			adven[pc_num].items[i].charges = 1;
		}
}

// Procedure only ready for town and outdoor
short dist_from_party(location where)
{
	short store = 1000, i;
	
	if ((overall_mode >= 10) && (overall_mode < 20)) {
		for (i = 0; i < 6; i++)
			if (adven[i].main_status == status_type::Normal)
				store = min(store,dist(pc_pos[i],where));
		}
		else store = dist(c_town.p_loc,where);
		
	return store;
}

void set_item_flag(item_record_type *item)
{
	if ((item->is_special > 0) && (item->is_special < 65)) {
		item->is_special--;
		party.item_taken[c_town.town_num][item->is_special / 8] =
			party.item_taken[c_town.town_num][item->is_special / 8] | s_pow(2,item->is_special % 8);
		item->is_special = 0;
		}
}

short get_item(location place,short pc_num,bool check_container)
//short pc_num; // if 6, any   
{
	short i,taken = 0;
	Boolean item_near = FALSE;
	short mass_get = 1;
	
	for (i = 0; i < T_M; i++)
		if ((c_town.monst.dudes[i].active > 0) && (c_town.monst.dudes[i].attitude == 1)
			&& (can_see(place,c_town.monst.dudes[i].m_loc,0) < 5))
				mass_get = 0;
		
	for (i = 0; i < NUM_TOWN_ITEMS; i++)
		if (t_i.items[i].variety != item_variety::None)
			if (((adjacent(place,t_i.items[i].item_loc) == TRUE) || 
			 ((mass_get == 1) && !check_container &&
			 ((dist(place,t_i.items[i].item_loc) <= 4) || (is_combat() && (which_combat_type == 0)))
			  && (can_see(place,t_i.items[i].item_loc,0) < 5))) 
			  && (!is_contained(t_i.items[i]) || check_container)) {
				taken = 1;

					if (t_i.items[i].value < 2)
						t_i.items[i].item_properties = t_i.items[i].item_properties | 1;
					item_near = TRUE;
				}
	if (item_near == TRUE)
		if (display_item(place,pc_num,mass_get,check_container) > 0) { // if true, there was a theft
			for (i = 0; i < T_M; i++)
				if ((c_town.monst.dudes[i].active > 0) && (c_town.monst.dudes[i].attitude % 2 != 1)
					&& (can_see(place,c_town.monst.dudes[i].m_loc,0) < 5)) {
						make_town_hostile();
						i = T_M;
						add_string_to_buf("Your crime was seen!");
						}
			}
				
	if (pc_num != 10) {
		if (taken == 0)
			add_string_to_buf("Get: nothing here");
			else add_string_to_buf("Get: OK");
		}

	reset_item_max();

	return taken;
}

void make_town_hostile()
{
	short i,num;
	Boolean fry_party = FALSE;

	if (which_combat_type == 0)
		return;
	give_help(53,0,0);
	c_town.monst.friendly = 1;
	////
	for (i = 0; i < T_M; i++)
		if ((c_town.monst.dudes[i].active > 0) && (c_town.monst.dudes[i].summoned == 0)){
			c_town.monst.dudes[i].attitude = 1;
			num = c_town.monst.dudes[i].number;
			c_town.monst.dudes[i].mobile = TRUE;
			if (scenario.monster(num).spec_skill == 37) {
				c_town.monst.dudes[i].active = 2;
				
				// If a town, give pwoer boost
				c_town.monst.dudes[i].m_d.health *= 3;
				c_town.monst.dudes[i].gaffect(affect::Speed) = 8;
				c_town.monst.dudes[i].gaffect(affect::CursedBlessed) = 8;
				}
			}
			
// In some towns, doin' this'll getcha' killed.
//// wedge in special

	if (fry_party == TRUE) {
		for (i = 0; i < 6; i++)
			if (adven[i].main_status > status_type::Absent)
				adven[i].main_status = status_type::Absent;
		stat_window = 6;
		boom_anim_active = FALSE;	
		}
}


void put_item_graphics()
{
	short i,storage;
	item_record_type item;

	// First make sure all arrays for who can get stuff are in order.
	if ((current_getting_pc < 6) && ((adven[current_getting_pc].main_status != status_type::Normal) 
	 || (pc_has_space(adven[current_getting_pc]) == 24))) {
	 	current_getting_pc = 6;
	 	
	 	}
	for (i = 0; i < 6; i++)
		if ((adven[i].main_status == status_type::Normal) && (pc_has_space(adven[i]) < 24)
		 && ((is_not_combat()) || (current_pc == i))) {
			if (current_getting_pc == 6)
				current_getting_pc = i;
			cd_activate_item(987,3 + i,1);
			}
			else {
				cd_activate_item(987,3 + i,0);
				cd_activate_item(987,11 + i,0);
				}
	for (i = 0; i < 6; i++)
		if (current_getting_pc == i)
			cd_add_label(987,3 + i,"*   ",1007);
			else cd_add_label(987,3 + i,"    ",1007); 
			
	// darken arrows, as appropriate
	if (first_item_shown == 0)
		cd_activate_item(987,9,0);
		else cd_activate_item(987,9,1);
	if ((first_item_shown > total_items_gettable - 7) || 
		(total_items_gettable <= 8) )
		cd_activate_item(987,10,0);
		else cd_activate_item(987,10,1); 

	for (i = 0; i < 8; i++) {
		// first, clear whatever item graphic is there
		csp(987,20 + i * 4,950);

		if (item_array[i + first_item_shown] != 200) { // display an item in window
			item = t_i.items[item_array[i + first_item_shown]]; 
			csit(987,21 + i * 4, std::format("{}", (is_ident(item) == TRUE) ? item.full_name : item.name));
			if (item.graphic_num >= 150)
				csp(987,20 + i * 4,3000 + 2000 + item.graphic_num - 150);
				else csp(987,20 + i * 4,4800 + item.graphic_num);////
			csit(987,22 + i * 4, get_item_interesting_string(item));
			storage = item_weight(item);
			csit(987,53 + i, std::format("Weight: {:d}", storage));
	  	}
		else
		{
			// erase the spot
			csit(987,21 + i * 4, "");
			csit(987,22 + i * 4, "");
			csit(987,53 + i, "");
		}
	}
	
	if (current_getting_pc < 6)
	{
		i = pc_amount_can_carry(adven[current_getting_pc]);
		storage = pc_carry_weight(adven[current_getting_pc]);
		csit(987,52, std::format("{} is carrying {:d} out of {:d}.", adven[current_getting_pc].name, storage, i));
	}
		
	for (i = 0; i < 6; i++) 
		if (adven[i].main_status == status_type::Normal) {
			csp(987,11 + i,800 + adven[i].which_graphic);
			}
}


void display_item_event_filter (short item_hit)
{
	item_record_type item;
	short i;
	
		switch (item_hit) {
			case 1:
				dialog_not_toast = FALSE;
				break;
			case 9:
				if (first_item_shown > 0)
					first_item_shown -= 8;
				put_item_graphics();					
				break;
			case 10:
				if (first_item_shown < 116)
					first_item_shown += 8;
				put_item_graphics();					
				break;
			case 3: case 4: case 5: case 6:case 7: case 8: 
				current_getting_pc = item_hit - 3;
				put_item_graphics();					
				break;
			default:
				if (current_getting_pc == 6) {
					break;
					}
				item_hit = (item_hit - 19) / 4;
				item_hit += first_item_shown;
				if (item_array[item_hit] >= NUM_TOWN_ITEMS)
					break;
				item = t_i.items[item_array[item_hit]];
				if (is_property(item)) {
					i = (dialog_answer == 0) ? fancy_choice_dialog(1011,987) : 2;
					if (i == 1)
						break;
						else {
							dialog_answer = 1;
							item.item_properties = item.item_properties & 253;
  							}
					}


				if (t_i.items[item_array[item_hit]].variety == item_variety::Gold) {
					if (t_i.items[item_array[item_hit]].item_level > 3000)
						t_i.items[item_array[item_hit]].item_level = 3000;
				set_item_flag(&item);
					give_gold(t_i.items[item_array[item_hit]].item_level,FALSE);
					force_play_sound(39);
					}
				else if (t_i.items[item_array[item_hit]].variety == item_variety::Food) {
					give_food(t_i.items[item_array[item_hit]].item_level,FALSE);
 				set_item_flag(&item);
					set_item_flag(&t_i.items[item_array[item_hit]]);
					force_play_sound(62);
					}
  				else {
					if (item_weight(item) >
					  pc_amount_can_carry(adven[current_getting_pc]) - pc_carry_weight(adven[current_getting_pc])) {
						SysBeep(20);
						csit(987,52,"It's too heavy to carry.");
						give_help(38,0,987);
						break;
						}

 				set_item_flag(&item);
					force_play_sound(0);
					give_to_pc(current_getting_pc, item, 0);
					}
				t_i.items[item_array[item_hit]] = item_record_type{};
				for (i = item_hit; i < 125; i++)
					item_array[i] = item_array[i + 1];
				total_items_gettable--;
				put_item_graphics();
				break;
				}

}


// Returns TRUE is a theft committed
static short display_item(location from_loc,short pc_num,short mode, bool check_container)
//pc_num;  // < 6 - this pc only  6 - any pc
//short mode; // 0 - adjacent  1 - all in sight
{
	short i,array_position = 0;
	
	make_cursor_sword();
	
	first_item_shown = 0;
	store_get_mode = mode;
	current_getting_pc = current_pc;
	store_pcnum = pc_num;
	dialog_answer = 0;
	
	for (i = 0; i < 130; i++)
		item_array[i] = 200;
	
	total_items_gettable = 0;
	for (i = 0; i < NUM_TOWN_ITEMS; i++)
		if (t_i.items[i].variety != item_variety::None)
		{
			if (((adjacent(from_loc,t_i.items[i].item_loc) == TRUE) || 
				 ((mode == 1) && !check_container &&
				 ((dist(from_loc,t_i.items[i].item_loc) <= 4) || (is_combat() && (which_combat_type == 0)))
				  && (can_see(from_loc,t_i.items[i].item_loc,0) < 5))) &&
				  (is_contained(t_i.items[i]) == check_container) &&
				  (!check_container || (same_point(t_i.items[i].item_loc,from_loc)))) 
			{
			 	item_array[array_position] = i;
				array_position++;
				total_items_gettable++;
			}
		}
	if (pcs_gworld == NULL)
		pcs_gworld = load_pict(902,main_dc);
	cd_create_dialog(987,mainPtr);

	if (check_container)
		csit(987,17,"Looking in container:");
		else if (mode == 0)
		csit(987,17,"Getting all adjacent items:");
			else csit(987,17,"Getting all nearby items:");
	cd_set_flag(987,18,1);
	cd_set_flag(987,51,0);
	cd_set_flag(987,52,0);
	for (i = 0; i < 8; i++)
		cd_attach_key(987,19 + 4 * i,(char) (97 + i));
	put_item_graphics();

	if (party.help_received[36] == 0)
	{
		cd_initial_draw(987);
		give_help(36,37,987);
	}

	while (dialog_not_toast)
		ModalDialog();	
		
	cd_kill_dialog(987,0);
	DisposeGWorld(pcs_gworld);
	pcs_gworld = NULL;
	put_item_screen(stat_window,0);
	put_pc_screen();

	return dialog_answer;
}

short custom_choice_dialog(const std::span<const std::string_view, 6>& strs,short pic_num,short buttons[3])
{
	short store_dialog_answer = dialog_answer;
	make_cursor_sword();

	cd_create_custom_dialog(mainPtr, strs, pic_num, buttons);

	while (dialog_not_toast)
		ModalDialog();

	cd_kill_dialog(900,0);

	if (in_startup_mode == FALSE)
		refresh_screen(0);
	else
		draw_startup(0);

	const short i = dialog_answer;
	dialog_answer = store_dialog_answer;
	return i;
}

short custom_choice_dialog(const std::array<std::array<char, 256>, 6>& strs, short pic_num, short buttons[3])
{
	const std::array< std::string_view, 6> strs_arr{ strs[0].data(), strs[1].data(), strs[2].data(), strs[3].data(), strs[4].data(), strs[5].data() };
	return custom_choice_dialog(strs_arr, pic_num, buttons);
}

void fancy_choice_dialog_event_filter (short item_hit)
{
	dialog_not_toast = FALSE;
	dialog_answer = item_hit;
}

short fancy_choice_dialog(short which_dlog,short parent)
// ignore parent in Mac version
{
	short i,store_dialog_answer;
	char temp_str[256];
	
	store_dialog_answer = dialog_answer;
	make_cursor_sword();
	
	cd_create_dialog_parent_num(which_dlog,parent);
	
	if (which_dlog == 1062) {
		i = rand_short(0,12);
		get_str(temp_str,11,10 + i);
		csit(1062,10, temp_str);
		}
	while (dialog_not_toast)
		ModalDialog();	
	
	cd_kill_dialog(which_dlog,0);

	if (parent < 2) {
		if (in_startup_mode == FALSE)
			refresh_screen(0);
			else draw_startup(0);
  		}
	i = dialog_answer;
	dialog_answer = store_dialog_answer;
	
	return i;
}

void select_pc_event_filter (short item_hit)
{
	dialog_not_toast = FALSE;
	if (item_hit == 16)
		dialog_answer = 6;
		else dialog_answer = item_hit - 3;
}

short char_select_pc(short active_only,short free_inv_only, const char * title)
//active_only;  // 0 - no  1 - yes   2 - disarm trap   
{
	short i;

	make_cursor_sword();
	
	cd_create_dialog(1018,mainPtr);
	
	if (active_only == 2)
		csit(1018,15,"Select PC to disarm trap:");
		else csit(	1018,15,title);
	
	for (i = 0; i < 6; i++) {
		if ((adven[i].main_status == status_type::Absent) ||
			((active_only == TRUE) && (adven[i].main_status > status_type::Normal)) ||
			((free_inv_only == 1) && (pc_has_space(adven[i]) == 24)) || (adven[i].main_status == status_type::Fled)) {
				cd_activate_item(1018, 3 + i, 0);
				}
		if (adven[i].main_status != status_type::Absent) {
				csit(1018,9 + i,adven[i].name);		
			}		
			else cd_activate_item(1018, 9 + i, 0);
		}
	
	while (dialog_not_toast)
		ModalDialog();	
	cd_kill_dialog(1018,0);

	if (in_startup_mode == FALSE)
		refresh_screen(0);
		else draw_startup(0);

	return dialog_answer;
}

short select_pc(short active_only,short free_inv_only)
//active_only;  // 0 - no  1 - yes   2 - disarm trap   
{
	if (active_only == 2)
		return char_select_pc(active_only,free_inv_only,"Trap! Who will disarm?");
		else return char_select_pc(active_only,free_inv_only,"Select a character:");
}

void get_num_of_items_event_filter (short item_hit)
{
	dialog_answer = std::stoi(cd_get_text_edit_str(1012));
	dialog_not_toast = FALSE;
}

short get_num_of_items(short max_num)
//town_num; // Will be 0 - 200 for town, 200 - 290 for outdoors
//short sign_type; // terrain type
{
	make_cursor_sword();
	cd_create_dialog(1012,mainPtr);
	csit(1012,4, std::format("How many? (0-{:d}) ", max_num));
	cd_set_text_edit_str(1012, std::format("{:d}", max_num));
	cd_set_edit_focus();
		
	while (dialog_not_toast)
		ModalDialog();	
	cd_kill_dialog(1012,0);
	
	dialog_answer = boe_clamp(dialog_answer,0,max_num);
	
	return dialog_answer;
}

static INT_PTR CALLBACK choice_dialog_proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	test_dlog3 = hDlg;

	switch (message) {
		case WM_INITDIALOG:
			center_window(hDlg);

			return TRUE;
		case WM_PAINT:

			return FALSE;

		case WM_COMMAND:
			answer_given = (short) wParam;
			if (answer_given == 100)
				answer_given = 2;
			EndDialog(hDlg, 0);
			return TRUE;
		}
	return FALSE;
	}

short choice_dialog(short pic,short num)
{
	store_focus = GetFocus();
	dlog_proc1 = choice_dialog_proc;
	if (dlog_proc1 == NULL) {
		add_string_to_buf("Dialog error, number...");
		print_nums(0,0,num);
		return 1;
		}
	DialogBox(store_hInstance, MAKEINTRESOURCE(num), store_focus, dlog_proc1);
	DestroyWindow(test_dlog3);
	SetFocus(store_focus);
	return answer_given;
}


static INT_PTR CALLBACK modeless_dialog_proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	short i,which_d,store_which;
	Boolean id_dlg = TRUE;

	for (i = 0; i < 18; i++)
		if ((modeless_exists[i] == TRUE) &&
		(hDlg == modeless_dialogs[i])) {
			which_d = i;
			i = 50;
			}
	if (i == 18) {
		id_dlg = FALSE;
		}
		else store_which = modeless_key[which_d];

	switch (message) {
		case WM_INITDIALOG:
			ShowWindow(hDlg,SW_SHOW);
			break;
		case WM_PAINT:
			if (being_created > 0) {
				store_which = being_created;
				id_dlg = TRUE;
				being_created = -1;
				}
			if (id_dlg == FALSE)
				return 0;

			return FALSE;

		case WM_COMMAND:
			DestroyWindow(hDlg);
			return TRUE;
		case WM_DESTROY:
			if (id_dlg == TRUE)
				modeless_exists[which_d] = FALSE;
			return 0;
		}
	return FALSE;
}

void create_modeless_dialog(short which_dlog)
{
	short i,which_d;

	for (i = 0; i < 18; i++)
		if (which_dlog == modeless_key[i]) {
			which_d = i;
			i = 50;
			}
	if (i == 18)
		return;
	if (modeless_exists[which_d] == TRUE) {
		SetFocus(modeless_dialogs[which_d]);
		return;
		}

	being_created = which_dlog;

	modeless_exists[which_d] = TRUE;
	if (procinst_exists[which_d] == 0) {
		procinst_exists[which_d] = 1;
		modeless_dlogprocs[which_d] = modeless_dialog_proc;
		}
	modeless_dialogs[which_d] = CreateDialog(store_hInstance,
	 MAKEINTRESOURCE(which_dlog),mainPtr,modeless_dlogprocs[which_d]);
}

void make_cursor_watch()
{
	SetCursor(LoadCursor(NULL,IDC_WAIT));
	ShowCursor(TRUE);
}


void place_glands(location where,unsigned char m_type)
{
	item_record_type store_i;
	monster_record_type monst;
	
	monst = return_monster_template(m_type);
	
	if ((monst.corpse_item >= 0) && (monst.corpse_item < 400) && (rand_short(0,100) < monst.corpse_item_chance)) {
		store_i = item_source.stored_item(monst.corpse_item);
		place_item(store_i,where,FALSE);
		}
}

void reset_item_max()
{
	short i;

	for (i = 0; i < NUM_TOWN_ITEMS; i++)
		if (t_i.items[i].variety != item_variety::None)
			item_max = i + 1;
}

void place_treasure(location where,short level,short loot,short mode)
//short mode;  // 0 - normal, 1 - force
{

	item_record_type new_item;
	short amt,r1,i,j;
	short max,min;
	
	if (loot == 1)
		amt = get_ran(2,1,7) + 1;
		else amt = loot * (rand_short(0,10 + (loot * 6) + (level * 2)) + 5);

	if (adventurers_level_total(adven) <= 12)
		amt += 1;	
	if ((adventurers_level_total(adven) <= 60)	&& (amt > 2))
		amt += 2;
		
	if (amt > 3) {	
			new_item = item_source.stored_item(0);
			new_item.item_level = amt;
			r1 = rand_short(1,9);
			if (((loot > 1) && (r1 < 7)) || ((loot == 1) && (r1 < 5)) || (mode == 1)
				|| ((r1 < 6) && (adventurers_level_total(adven) < 30)) || (loot > 2) )
				place_item(new_item,where,FALSE);
		}
	for (j = 0; j < 5; j++) {
		r1 = rand_short(0,100);
		if ((c_treas_chart[loot][j] >= 0) && (r1 <= c_treas_odds[loot][j] + adventurers_luck_total(adven))) {
			r1 = rand_short(0,9);
			min = c_min_chart[c_treas_chart[loot][j]][r1];
			r1 = rand_short(0,9);
			max = (min + level + (2 * (loot - 1)) + (adventurers_luck_total(adven) / 3)) * c_max_mult[c_treas_chart[loot][j]][r1];
			if (rand_short(0,1000) == 500) {
				max = 10000;
				min = 100;
				}
				
			// reality check
			if ((loot == 1) && (max > 100) && (rand_short(0,8) < 7))
				max = 100;
			if ((loot == 2) && (max > 200) && (rand_short(0,8) < 6))
				max = 200;
				
				
				new_item = item_source.treasure(c_treas_chart[loot][j],level,mode);
				if ((item_val(new_item) < min) || (item_val(new_item) > max)) {
					new_item = item_source.treasure(c_treas_chart[loot][j],level,mode);
					if ((item_val(new_item) < min) || (item_val(new_item) > max)) {
						new_item = item_source.treasure(c_treas_chart[loot][j],level,mode);
						if (item_val(new_item) > max)
							new_item.variety = item_variety::None;
						}
					}

			// not many magic items
			if (mode == 0) {
				if (is_magic(new_item) && (level < 2) && (rand_short(0,5) < 3))
					new_item.variety = item_variety::None;
				if (is_magic(new_item) && (level == 2) && (rand_short(0,5) < 2))
					new_item.variety = item_variety::None;
				if (is_cursed(new_item) && (rand_short(0,5) < 3))
					new_item.variety = item_variety::None;
				}
				
			// if forced, keep dipping until a treasure comes uo
			if ((mode == 1)	&& (max >= 20)) {
				do
					new_item = item_source.treasure(c_treas_chart[loot][j],level,mode);
					while ((new_item.variety == item_variety::None) || (item_val(new_item) > max));
				}

			// Not many cursed items
			if (is_cursed(new_item) && (rand_short(0,2) == 1))
				new_item.variety = item_variety::None;
							
			if (new_item.variety != item_variety::None) {
				for (i = 0; i < 6; i++)
					if ((adven[i].main_status == status_type::Normal) 
						&& (rand_short(0,100) < c_id_odds[adven[i].skills[skill::ItemLore]]))
							new_item.item_properties = new_item.item_properties | 1;
				place_item(new_item,where,FALSE);
				}			
			}
		}

}

void refresh_store_items()
{
	short i,j;
	short loot_index[10] = {1,1,1,1,2,2,2,3,3,4};
	
	for (i = 0; i < 5; i++)
		for (j = 0; j < 10; j++) {
			party.magic_store_items[i][j] = item_source.treasure(loot_index[j],7,1);
			if ((party.magic_store_items[i][j].variety == item_variety::Gold) ||
				(party.magic_store_items[i][j].variety == item_variety::Food))
				party.magic_store_items[i][j] = item_record_type{};
			party.magic_store_items[i][j].item_properties =
				party.magic_store_items[i][j].item_properties | 1;
			}
}


void get_text_response_event_filter (short item_hit)
{
	strcpy(store_str, cd_get_text_edit_str(store_dnum).c_str());
	dialog_not_toast = FALSE;
}

void get_text_response(short dlg,char *str,short parent_num)
{

	short i;

	make_cursor_sword();
	
	store_str = str;
	store_dnum = dlg;
	
	cd_create_dialog_parent_num(dlg,parent_num);
	cd_set_edit_focus();
			
	while (dialog_not_toast)
		ModalDialog();	
	for (i = 0; i < 15; i++)
		if ((str[i] > 64) && (str[i] < 91))
			str[i] = str[i] + 32;
	//ASB(str);
	final_process_dialog(dlg);
	//cd_kill_dialog(dlg,0);
		
}

// returns true is party has item of given item class
// mode - 0 - take one of them, 1 - don't take
Boolean party_check_class(short item_class,short mode) ////
{
	short i,j;
	
	if (item_class == 0)
		return FALSE;
	for (i = 0; i < 6; i++)
		if (adven[i].main_status == status_type::Normal)
			for (j = 23; j >= 0; j--)
				if ((adven[i].items[j].variety > item_variety::None) && (adven[i].items[j].special_class == item_class)) {
					if (mode == 0) {
						if (adven[i].items[j].charges > 1)
							adven[i].items[j].charges--;
							else take_item(i,j);
						}
					return TRUE;
					}
	return FALSE;
}
