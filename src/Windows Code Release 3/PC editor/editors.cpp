#include <Windows.h>
#include <array>

#include "boe/pc.hpp"
#include "graphics.h"
#include "../global.h"
#include "editors.h"
#include "dlogtool.h"

/* Adventure globals */
extern party_record_type party;
extern Adventurers adven;
extern outdoor_record_type outdoors[2][2];
extern current_town_type c_town;
extern big_tr_type t_d;
extern town_item_list far	t_i;
extern unsigned char out[96][96];
extern setup_save_type setup_save;
extern stored_items_list_type stored_items[3];
extern stored_town_maps_type maps;
extern stored_outdoor_maps_type o_maps;
extern short dialog_answer;

extern Boolean dialog_not_toast;

extern short current_active_pc;

extern HWND mainPtr;
extern short current_cursor;

extern HCURSOR sword_curs;
extern Boolean diff_depth_ok,current_file_has_maps;

Boolean choice_active[6];



extern short store_trait_mode;
pc_record_type *store_pc;
HBITMAP button_num_gworld;

DLGPROC dlog_proc1;
HWND test_dlog3;
short answer_given;
HWND store_focus;
extern HINSTANCE store_hInstance;

void combine_things(short pc_num)
{

	short i,j,test;
	
	for (i = 0; i < 24; i++) {
		if ((adven[pc_num].items[i].variety > item_variety::None) &&
			(adven[pc_num].items[i].type_flag > 0) && (adven[pc_num].items[i].item_properties & 254 != 0)) {
			for (j = i + 1; j < 24; j++)
				if ((adven[pc_num].items[j].variety > item_variety::None) &&
				(adven[pc_num].items[j].type_flag == adven[pc_num].items[i].type_flag) 
				 && (adven[pc_num].items[j].item_properties & 254 != 0)) {
					add_string_to_buf("(items combined)");
					test = (short) (adven[pc_num].items[i].charges) + (short) (adven[pc_num].items[j].charges);
					if (test > 125) {
						adven[pc_num].items[i].charges = 125;
						ASB("Can have at most 125 of any item.");
						}
				 		else adven[pc_num].items[i].charges += adven[pc_num].items[j].charges;
				 	if (adven[pc_num].equip[j] == TRUE) {
				 		adven[pc_num].equip[i] = TRUE;
				 		adven[pc_num].equip[j] = FALSE;
				 		}
					take_item(pc_num,j);
				 	}
			}		
		if ((adven[pc_num].items[i].variety > item_variety::None) && (adven[pc_num].items[i].charges < 0))
			adven[pc_num].items[i].charges = 1;
		}
}

Boolean give_to_pc(short pc_num,item_record_type  item, short print_result)
{
	short free_space;

	if (item.variety == item_variety::None)
		return TRUE;
	if (((free_space = pc_has_space(adven[pc_num])) == 24 ) || (adven[pc_num].main_status != status_type::Normal))
		return FALSE;
		else {
			adven[pc_num].items[free_space] = item;
			combine_things(pc_num);
			return TRUE;
			}
	return FALSE;
}

Boolean give_to_party(item_record_type item,short print_result)
{
	short i = 0;
	
	while (i < 6) {
		if (give_to_pc(i,item,print_result) == TRUE)
			return TRUE;
		i++;
	}
	return FALSE;
}

void give_gold(short amount,Boolean print_result)
{
	party.gold = party.gold + amount;
}

Boolean take_gold(short amount,Boolean print_result)
{
	if (party.gold < amount)
		return FALSE;
	party.gold = party.gold - amount;
	return TRUE;
}


void take_item(short pc_num,short which_item)
//short pc_num,which_item;  // if which_item > 20, don't update stat win, item is which_item - 20
{
	short i;

	if ((adven[pc_num].weap_poisoned == which_item) && (adven[pc_num].gaffect(affect::PoisonedWeapon) > 0)) {
//			add_string_to_buf("  Poison lost.           ");
			adven[pc_num].gaffect(affect::PoisonedWeapon) = 0;
		}
	if ((adven[pc_num].weap_poisoned > which_item) && (adven[pc_num].gaffect(affect::PoisonedWeapon) > 0)) 
		adven[pc_num].weap_poisoned--;
		
	for (i = which_item; i < 23; i++) {
		adven[pc_num].items[i] = adven[pc_num].items[i + 1];
		adven[pc_num].equip[i] = adven[pc_num].equip[i + 1];
		}
	adven[pc_num].items[23].variety = item_variety::None;
	adven[pc_num].equip[23] = FALSE;

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
	
	store_dialog_answer = dialog_answer;
	make_cursor_sword();
	
	cd_create_dialog_parent_num(which_dlog,parent);
	

	while (dialog_not_toast)
		ModalDialog();	
	
	cd_kill_dialog(which_dlog,0);

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

	return dialog_answer;
}

short select_pc(short active_only,short free_inv_only)
//active_only;  // 0 - no  1 - yes   2 - disarm trap   
{
	if (active_only == 2)
		return char_select_pc(active_only,free_inv_only,"Trap! Who will disarm?");
		else return char_select_pc(active_only,free_inv_only,"Select a character:");
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
		//print_nums(0,0,num);
		return 1;
		}
	DialogBox(store_hInstance, MAKEINTRESOURCE(num), store_focus, dlog_proc1);
	DestroyWindow(test_dlog3);
	SetFocus(store_focus);
	return answer_given;
}

void display_traits_graphics()
{
	short i,store;

	for (i = 0; i < 3; i++) {
		cd_set_led(1013,4 + i,(store_pc->race == i) ? 1 : 0);
		}
	for (i = 0; i < 10; i++) {
		cd_set_led(1013,7 + i,(store_pc->traits[i] > 0) ? 1 : 0);
		}
	for (i = 0; i < 5; i++) {
		cd_set_led(1013,36 + i,(store_pc->traits[10 + i] > 0) ? 1 : 0);
		}

	store = pc_get_tnl(*store_pc);
	cdsin(1013,18,store);
}

void pick_race_abil_event_filter(short item_hit)
{
	char abil_str[256];
	pc_record_type *pc;

	pc = store_pc;
			switch (item_hit) {
				case 3:
					dialog_not_toast = FALSE;
					break;
				case 4: case 5: case 6:
					if (store_trait_mode == 0)
						pc->race = item_hit - 4;
					display_traits_graphics();
					get_str(abil_str,5,12 + item_hit);
					csit(1013,19,(char *) abil_str);
					break;
				case 36: case 37: case 38: case 39: case 40:
					if (store_trait_mode != 1)
						pc->traits[item_hit - 36 + 10] = (pc->traits[item_hit - 36 + 10] == TRUE) ? FALSE : TRUE;
					display_traits_graphics();
					get_str(abil_str,5,item_hit - 36 + 11);				
					csit(1013,19,(char *) abil_str);
					break;
				default:
					if (item_hit >= 100)
						return;
					if (store_trait_mode != 1)
						pc->traits[item_hit - 7] = (pc->traits[item_hit - 7] == TRUE) ? FALSE : TRUE;
					display_traits_graphics();
					get_str(abil_str,5,item_hit - 6);				
					csit(1013,19,(char *) abil_str);
					break;
				}	

}

static const char c_start_str1[]{"Click on advantage button for description."};
static const char c_start_str2[]{"Click on advantage button to add/lose."};


void pick_race_abil(pc_record_type *pc,short mode,short parent_num)
//mode; // 0 - edit  1 - just display  2 - can't change race
{
	mode = 0;
	store_trait_mode = mode;
	store_pc = pc;
	make_cursor_sword();

	cd_create_dialog_parent_num(1013,parent_num);

	display_traits_graphics();
	if (mode == 1)
		csit(1013,19,c_start_str1);
		else csit(1013,19,c_start_str2);

	while (dialog_not_toast)
		ModalDialog();	
	
	cd_kill_dialog(1013,0);
	dialog_not_toast = TRUE;
}
