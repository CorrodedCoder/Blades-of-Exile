#pragma once

#include "global_structs.hpp"

#define T_M 	60

#define	SLEEP_TICKS		0L
#define	MOUSE_REGION	0L
#define DRAG_EDGE		15

#define	DISPLAY_LEFT	23
#define	DISPLAY_TOP		23
#define	BITMAP_WIDTH	28
#define	BITMAP_HEIGHT	36

#define	STORED_GRAPHICS	240

#define	PC_WIN_UL_X	291
#define	PC_WIN_UL_Y	5
#define	ITEM_WIN_UL_X	291
#define	ITEM_WIN_UL_Y	130
#define	TEXT_WIN_UL_X	291
#define	TEXT_WIN_UL_Y	283
#define	NUM_BUTTONS		15
#define ASB	add_string_to_buf
#define	PSD	party.stuff_done
#define	DES	display_enc_string
#define	FCD	fancy_choice_dialog
#define	D2ES display_2_enc_string
#define	NUM_MONST_G	173

#define DOOR_LIGHT can_enter = run_trap(7,&PSD[c_town.town_num][which],4220,0); break;
#define DOOR_HEAVY can_enter = run_trap(7,&PSD[c_town.town_num][which],4220,20); break;
#define DOOR_ALARM can_enter = run_trap(7,&PSD[c_town.town_num][which],4220,11); break;
#define DRESSER_LIGHT can_enter = run_trap(7,&PSD[c_town.town_num][which],4221,0); break;
#define DRESSER_HEAVY can_enter = run_trap(7,&PSD[c_town.town_num][which],4221,20); break;
#define DRESSER_ALARM can_enter = run_trap(7,&PSD[c_town.town_num][which],4221,11); break;
#define FLOOR_LIGHT can_enter = run_trap(7,&PSD[c_town.town_num][which],4222,0); break;
#define FLOOR_HEAVY can_enter = run_trap(7,&PSD[c_town.town_num][which],4222,20); break;
#define FLOOR_ALARM can_enter = run_trap(7,&PSD[c_town.town_num][which],4222,11); break;
#define CHEST_LIGHT can_enter = run_trap(7,&PSD[c_town.town_num][which],3450,0); break;
#define CHEST_HEAVY can_enter = run_trap(7,&PSD[c_town.town_num][which],3450,20); break;
#define CHEST_ALARM can_enter = run_trap(7,&PSD[c_town.town_num][which],3450,11); break;

#define	CDGT	cd_retrieve_text_edit_str
#define	CDGN	cd_retrieve_text_edit_num
#define	CDST	cd_set_text_edit_str
#define	CDSN	cd_set_text_edit_num

short s_pow(short x,short y);
short a_v(short x);
short ex_abs(short x);
void pause(short length);
Boolean sd_legit(short a, short b);

// Windows RECT structure layout is: left, top, right, bottom
// Mac Quickdraw Rect structure layout is: top, left, bottom, right
// Where common BOE rectangle initialization data is used, it expects
// that data to be in the Mac Quickdraw format, so provide a convenience
// for now.
#ifdef _WIN32
#define BOE_INIT_RECT(top, left, bottom, right) {left, top, right, bottom}
#else
#define BOE_INIT_RECT(top, left, bottom, right) {top, left, bottom, right}
#endif

static inline RECT offset_rect(const RECT& input, int x, int y)
{
	auto output{ input };
	::OffsetRect(&output, x, y);
	return output;
}
