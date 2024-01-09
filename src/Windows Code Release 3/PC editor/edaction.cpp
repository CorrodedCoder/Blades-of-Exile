#include <Windows.h>
#include <array>
#include <cstdio>

#include "boe/pc.hpp"
#include "graphics.h"
#include "../global.h"
#include "editors.h"
#include "edfileio.h" 
#include "edaction.h" 
#include "edsound.h" 
#include "dlogtool.h" 
#include "../graphutl.h"
#include "boe/utility.hpp"
#include "boe/spell.hpp"

/* Adventure globals */
extern party_record_type party;
extern Adventurers adven;
extern Boolean dialog_not_toast,ed_reg;
extern long ed_flag;
extern HWND mainPtr;
extern Boolean file_in_mem;
extern short dialog_answer;
extern Boolean save_blocked;
extern RECT edit_rect[5][2];
extern short current_active_pc;
extern RECT pc_area_buttons[6][4]; // 0 - whole 1 - pic 2 - name 3 - stat strs 4,5 - later
extern RECT item_string_rects[24][4]; // 0 - name 1 - drop  2 - id  3 - 
extern short ulx, uly;


static const short skill_cost[20] = { 3,3,3,2,2,2, 1,2,2,6,
						5, 1,2,4,2,1, 4,2,5,0 };
static const short skill_max[20] = { 20,20,20,20,20,20,20,20,20,7,
						7,20,20,10,20,20,20,20,20 };
static const short skill_g_cost[20] = { 50,50,50,40,40,40,30,50,40,250,
						250,25,100,200,30,20,100,80,0,0 };

static short which_pc_displayed,store_which_to_edit;
static short store_train_pc;
static short store_skills[20], store_h, store_sp, which_skill, store_skp = 10000, store_g = 10000;

short store_trait_mode;
Boolean talk_done = FALSE;
pc_record_type *store_xp_pc;

//extern Rect pc_area_buttons[6][6] ; // 0 - whole 1 - pic 2 - name 3 - stat strs 4,5 - later
//extern Rect item_string_rects[24][4]; // 0 - name 1 - drop  2 - id  3 - 
Boolean handle_action(POINT the_point, UINT wparam, LONG lparam )
//short mode; // ignore,
{
	short i;
	
	short choice = 4;
	
	Boolean to_return = FALSE;
	Boolean ctrl_key = FALSE;
	Boolean right_button = FALSE;

	if (lparam != -1) {
		the_point.x -= ulx;
		the_point.y -= uly;
		}
	if (lparam == -2)
		right_button = TRUE;
	if (MK_CONTROL & wparam)
		ctrl_key = TRUE;

	if (file_in_mem == FALSE) 
		return FALSE;


	for (i = 0; i < 6; i++)
		if ((PtInRect(&pc_area_buttons[i][0],the_point) == TRUE) &&
			(adven[i].main_status > status_type::Absent)) {
			do_button_action(0,i);
			current_active_pc = i;
			display_party(6,1);
			draw_items(1);
			}
	for (i = 0; i < 5; i++)
		if ((PtInRect(&edit_rect[i][0],the_point) == TRUE) &&
			(adven[current_active_pc].main_status > status_type::Absent)) {
			do_button_action(0,i + 10);
			if ((ed_reg == FALSE) && (save_blocked == FALSE))
				if ((choice = FCD(904,0)) == 1)
					return to_return;
					else save_blocked = TRUE;
			switch(i) {
				case 0:
					display_pc(current_active_pc,0,0);
					break;
				case 1:
			 		display_pc(current_active_pc,1,0);
					break;
				case 2: 
					pick_race_abil(&adven[current_active_pc],0,0);
					break;
				case 3: 
					spend_xp(current_active_pc,1,0);
					break;
				case 4: 
					edit_xp(&adven[current_active_pc]);
					
					break;
			}
		}
	for (i = 0; i < 24; i++)
		if ((PtInRect(&item_string_rects[i][1],the_point) == TRUE) && // drop item
			(adven[current_active_pc].items[i].variety > item_variety::None)) { // variety = 0 no item in slot/ non 0 item exists
				flash_rect(item_string_rects[i][1]);
				take_item(current_active_pc,i);
				draw_items(1);
				}
	for (i = 0; i < 24; i++)
		if ((PtInRect(&item_string_rects[i][2],the_point) == TRUE) && // identify item
			(adven[current_active_pc].items[i].variety > item_variety::None)) {
				flash_rect(item_string_rects[i][2]);
				adven[current_active_pc].items[i].item_properties = adven[current_active_pc].items[i].item_properties | 1;
				draw_items(1);
				}
	
	return to_return;
}

void flash_rect(RECT to_flash)
{
	long dummy;
	HDC hdc;

	hdc = GetDC(mainPtr);
	SetViewportOrgEx(hdc,ulx,uly,nullptr);
	InvertRect (hdc,&to_flash);
	play_sound(37);
	Delay(5,&dummy);
	InvertRect (hdc,&to_flash);
	fry_dc(mainPtr,hdc);
}

void edit_gold_or_food_event_filter (short item_hit)
{
	char get_text[256];
	
	cd_get_text_edit_str((store_which_to_edit == 0) ? 1012 : 947,(char *) get_text);
	dialog_answer = 0;
	sscanf(get_text,"%hd",&dialog_answer);
	dialog_not_toast = FALSE;
}

void edit_gold_or_food(short which_to_edit)
//0 - gold 1 - food
{
	store_which_to_edit = which_to_edit;

	make_cursor_sword();
	
	cd_create_dialog((which_to_edit == 0) ? 1012 : 947,mainPtr);
		
	const auto sign_text{ std::format("{:d}",(short)((which_to_edit == 0) ? party.gold : party.food)) };
	cd_set_text_edit_str((which_to_edit == 0) ? 1012 : 947, sign_text);
	
	cd_set_edit_focus();
	while (dialog_not_toast)
		ModalDialog();	
	
	
	cd_kill_dialog((which_to_edit == 0) ? 1012 : 947,0);

	if (dialog_answer < 0)
		dialog_answer = -1;
	else
		dialog_answer = boe_clamp(dialog_answer,0,25000);
	
	if (dialog_answer >= 0)
	{
		if (which_to_edit == 0) 
			party.gold = dialog_answer;
		else
			party.food = dialog_answer;
	}
}

void edit_day_event_filter (short item_hit)
{
	char get_text[256];
	cd_get_text_edit_str(917,(char *) get_text);
	dialog_answer = 0;
	sscanf(get_text,"%hd",&dialog_answer);
	dialog_not_toast = FALSE;
}

void edit_day()
{
	make_cursor_sword();
	cd_create_dialog(917,mainPtr);
	const auto sign_text{ std::format("{:d}",(short)(((party.age) / 3700) + 1)) };
	cd_set_text_edit_str(917, sign_text);
	cd_set_edit_focus();
	while (dialog_not_toast)
		ModalDialog();	
	cd_kill_dialog(917,0);
	dialog_answer = boe_clamp(dialog_answer,0,500);
	party.age = (long) (3700) * (long) (dialog_answer);
}


void put_pc_graphics()
{
	short i;

	for (i = 3; i < 65; i++) {
		if (((store_trait_mode == 0) && (adven[which_pc_displayed].mage_spells[i - 3] == TRUE)) ||
		 ((store_trait_mode == 1) && (adven[which_pc_displayed].priest_spells[i - 3] == TRUE)))
			cd_set_led(991,i,1);
			else cd_set_led(991,i,0);
		}

	cd_set_item_text(991,69,adven[which_pc_displayed].name);
}
Boolean display_pc_event_filter (short item_hit)
{
	short pc_num;

	pc_num = which_pc_displayed;
			switch (item_hit) {
				case 1: case 65:
					dialog_not_toast = FALSE;
					break;

				case 66:
					do {
						pc_num = (pc_num == 0) ? 5 : pc_num - 1;
						} while (adven[pc_num].main_status == status_type::Absent);
					which_pc_displayed = pc_num;
					put_pc_graphics();
					break;
				case 67:
					do {
						pc_num = (pc_num == 5) ? 0 : pc_num + 1;
						} while (adven[pc_num].main_status == status_type::Absent);
					which_pc_displayed = pc_num;
					put_pc_graphics();	
					break;
					
				default:
					if (store_trait_mode == 0)
						adven[which_pc_displayed].mage_spells[item_hit - 3] = 
							1 - adven[which_pc_displayed].mage_spells[item_hit - 3];
						else
						adven[which_pc_displayed].priest_spells[item_hit - 3] = 
							1 - adven[which_pc_displayed].priest_spells[item_hit - 3];
					put_pc_graphics();							
					break;
				}	
	return FALSE;
}

void display_pc(short pc_num,short mode,short parent)
{
	short i;
	char label_str[256];
	
	if (adven[pc_num].main_status == status_type::Absent) {
		for (pc_num = 0; pc_num < 6; pc_num++)
			if (adven[pc_num].main_status == status_type::Normal)
				break;
		}
	which_pc_displayed = pc_num;
	store_trait_mode = mode;

	make_cursor_sword();

	cd_create_dialog_parent_num(991,0);

	for (i = 3; i < 65; i++) {
		get_str(label_str,(mode == 0) ? 7 : 8,(i - 3) * 2 + 1);
		cd_add_label(991,i,(char *)label_str,46);
		}
	put_pc_graphics();

	cd_set_pict(991,2,714 + mode);
	while (dialog_not_toast)
		ModalDialog();

	cd_kill_dialog(991,0);
}


Boolean display_alchemy_event_filter (short item_hit)
{
	short i;

			switch (item_hit) {
				case 1: case 3:
					dialog_not_toast = FALSE;
					break;
				default:
					party.alchemy[item_hit - 4] = 1 - party.alchemy[item_hit - 4];
					break;

				}
	for (i = 0; i < 20; i++) {
		if (party.alchemy[i] > 0)
			cd_set_led(996,i + 4,1);
			else cd_set_led(996,i + 4,0);
		}
					return FALSE;
}

void display_alchemy()
{
	short i;

	make_cursor_sword();

	cd_create_dialog_parent_num(996,0);


	for (i = 0; i < 20; i++) {
		cd_add_label(996,i + 4,alchemy_name(i),1083);
		if (party.alchemy[i] > 0)
			cd_set_led(996,i + 4,1);
			else cd_set_led(996,i + 4,0);
		}

	while (dialog_not_toast)
		ModalDialog();

	cd_kill_dialog(996,0);
	dialog_not_toast = TRUE;

}

void do_xp_keep(short pc_num,short mode)
{
					for (short i = 0; i < 20; i++)
						adven[pc_num].skills[i] = store_skills[i];
					adven[pc_num].cur_health += store_h - adven[pc_num].max_health;
					adven[pc_num].max_health = store_h;
					adven[pc_num].cur_sp += store_sp - adven[pc_num].max_sp;
					adven[pc_num].max_sp = store_sp;

}

void draw_xp_skills()
{
	short i;
	for (i = 0; i < 19; i++) {
		if ((store_skp >= skill_cost[i]) && (store_g >= skill_g_cost[i]))
			cd_text_frame(1010,54 + i,11);
			else cd_text_frame(1010,54 + i,1);
		cd_set_item_num(1010,54 + i,store_skills[i]);
		}

		if ((store_skp >= 1) && (store_g >= 10))
			cd_text_frame(1010,52,11);
			else cd_text_frame(1010,52,1);
	cd_set_item_num(1010,52,store_h);
		if ((store_skp >= 1) && (store_g >= 15))
			cd_text_frame(1010,53,11);
			else cd_text_frame(1010,53,1);
	cd_set_item_num(1010,53,store_sp);
}


void do_xp_draw()
{
	const short pc_num = store_train_pc;

	cd_set_item_text (1010, 51, adven[pc_num].name);

	for (short i = 0; i < 20; i++)
		store_skills[i] = adven[pc_num].skills[i];
	store_h = adven[pc_num].max_health;
	store_sp = adven[pc_num].max_sp;
	store_g = 12000;
	store_skp = 10000;

	draw_xp_skills();

	update_gold_skills();
}

Boolean spend_xp_event_filter (short item_hit)
{
	short pc_num;
	Boolean talk_done = FALSE;

	pc_num = store_train_pc;

		switch (item_hit) {
			case 73:
				dialog_answer = 0;
				talk_done = TRUE;
				break;
	


			case 3: case 4:
					if ((store_h >= 250) && (item_hit == 4)) 
							play_sound(0);
						else {
							if (item_hit == 3) {
								store_g += 10;
								store_h -= 2;
								store_skp += 1;
								}
								else {
									if ((store_g < 10) || (store_skp < 1)) {

										play_sound(0);
										}
										else {
											store_g -= 10;
											store_h += 2;
											store_skp -= 1;
											}
								}

							update_gold_skills();
							cd_set_item_num(1010,52,store_h);
                     	draw_xp_skills();

						}
				break;

			case 5: case 6:
					if ((store_sp >= 150) && (item_hit == 6))
							play_sound(0);
						else {
							if (item_hit == 5) {
								store_g += 15;
								store_sp -= 1;
								store_skp += 1;
								}
								else {
									if ((store_g < 15) || (store_skp < 1)) {

										play_sound(0);
										}
										else {
											store_sp += 1;
											store_g -= 15;
											store_skp -= 1;
											}
								}

							update_gold_skills();
							cd_set_item_num(1010,53,store_sp);
							draw_xp_skills();
						}
				break;

			case 48:
					do_xp_keep(pc_num,0);
					dialog_answer = 1;
					talk_done = TRUE;
				break;

			case 49:

						do_xp_keep(pc_num,0);
						do {
							pc_num = (pc_num == 0) ? 5 : pc_num - 1;
						} while (adven[pc_num].main_status != status_type::Normal);
						store_train_pc = pc_num;
						do_xp_draw();
				break;

			case 50:

						do_xp_keep(pc_num,0);
						do {
							pc_num = (pc_num == 5) ? 0 : pc_num + 1;
						} while (adven[pc_num].main_status != status_type::Normal);
						store_train_pc = pc_num;
						do_xp_draw();
				break;

			case 100:
				break;

			default:
				if (item_hit >= 100) {
					}
				else {
				which_skill = (item_hit - 7) / 2;
				
				if (((store_skills[which_skill] >= skill_max[which_skill]) && ((item_hit - 7) % 2 == 1)) ||
					((store_skills[which_skill] == 0) && ((item_hit - 7) % 2 == 0) && (which_skill > 2)) ||
					((store_skills[which_skill] == 1) && ((item_hit - 7) % 2 == 0) && (which_skill <= 2)))
						play_sound(0);
					else {
						if ((item_hit - 7) % 2 == 0) {
							store_g += skill_g_cost[which_skill];
							store_skills[which_skill] -= 1;
							store_skp += skill_cost[which_skill];
							}
							else {
								if ((store_g < skill_g_cost[which_skill]) || (store_skp < skill_cost[which_skill])) {

									play_sound(0);
									}
									else {
										store_skills[which_skill] += 1;
										store_g -= skill_g_cost[which_skill];
										store_skp -= skill_cost[which_skill];
										}
							}

							update_gold_skills();
							cd_set_item_num(1010,54 + which_skill,store_skills[which_skill]);
							draw_xp_skills();
						}
				}	
				break;
			}
			
	store_train_pc = pc_num;
	if (talk_done == TRUE) {
		dialog_not_toast = FALSE;
		}
	return FALSE;
}
void update_gold_skills()
{
	csit(1010,47,"Lots!");
	csit(1010,46,"Lots!");
}
Boolean spend_xp(short pc_num, short mode, short parent)
//short mode; // 0 - create  1 - train
// returns 1 if cancelled
{
	store_train_pc = pc_num;
	make_cursor_sword();
	cd_create_dialog_parent_num(1010,parent);
	cd_add_label(1010,52, std::format("Health ({:d}/{:d})", 1, 10), 1075);
	//cd_add_label(1010,5,get_text,1040);
	cd_add_label(1010,53, std::format("Spell Pts. ({:d}/{:d})", 1, 15), 1075);
	for (short i = 54; i < 73; i++)
	{
		char text2[256];
		get_str(text2,9,1 + 2 * (i - 54));
		cd_add_label(1010,i, std::format("{} ({:d}/{:d})", text2, skill_cost[i - 54], skill_g_cost[i - 54]), (i < 63) ? 1075 : 1069);
	}
	do_xp_draw();
	dialog_answer = 0;

	while (dialog_not_toast)
		ModalDialog();	
	
	cd_kill_dialog(1010,0);
	return dialog_answer;
}

void give_reg_info_event_filter (short item_hit)
{
			switch (item_hit) {
				case 1: 
					dialog_not_toast = FALSE;
					break;
				}
}

void give_reg_info()
{
	make_cursor_sword();

	cd_create_dialog_parent_num(1073,0);

	while (dialog_not_toast)
		ModalDialog();
	
	cd_kill_dialog(1073,0);

}




void do_registration_event_filter (short item_hit)
{
	char get_text[256];
	
	cd_get_text_edit_str(1075,(char *) get_text);
	dialog_answer = 0;
	sscanf(get_text,"%hd",&dialog_answer);
		
	dialog_not_toast = FALSE;
}

void do_registration()
{
	make_cursor_sword();
	
	cd_create_dialog(1075,mainPtr);
		
	cdsin(1075,7,(short) ed_flag);	
	cd_set_edit_focus();
	
	while (dialog_not_toast)
		ModalDialog();	
	
	
	cd_kill_dialog(1075,0);
	
	if (dialog_answer == 1)
		play_sound(0);
	
	if (dialog_answer == (short) init_data(ed_flag)) {

		FCD(1078,0);
		ed_reg = TRUE;
		build_data_file(2);
		}
		else {
			play_sound(0);
			FCD(1077,0);
			}

	
}

void edit_xp_event_filter (short item_hit)
{
	char get_text[256];
	
	cd_get_text_edit_str(1024,(char *) get_text);
	dialog_answer = 0;
	sscanf(get_text,"%hd",&dialog_answer);

	dialog_not_toast = FALSE;
}

void edit_xp(pc_record_type *pc)
{
	store_xp_pc = pc;
	make_cursor_sword();
	cd_create_dialog(1024,mainPtr);
	cd_set_text_edit_str(1024, std::format("{:d}", (short)pc->experience));
	cdsin(1024,8, pc_get_tnl(*store_xp_pc));
	
	while (dialog_not_toast)
		ModalDialog();
	
	cd_kill_dialog(1024,0);

	if (dialog_answer < 0)
		dialog_answer = dialog_answer * -1;

	dialog_answer = boe_clamp(dialog_answer,0,10000);
	pc->experience = dialog_answer;
}
