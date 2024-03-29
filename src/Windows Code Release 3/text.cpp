#define  LINES_IN_TEXT_WIN	11
#define	TEXT_BUF_LEN	70

#include <Windows.h>
#include <array>
#include <algorithm>
#include <format>
#include <cassert>

#include "global.h"
#include <cstdio>
#include "text.h"
#include <cstring>
#include "locutils.h"
#include "fields.h"
#include "exlsound.h"
#include "graphutl.h"
#include "graphutl_helpers.hpp"
#include "boe/hacks.hpp"
#include "boe/item.hpp"
#include "game_globals.hpp"

static const std::array m_mage_sp{"Spark","Minor Haste","Strength","Flame Cloud","Flame",
						"Minor Poison","Slow","Dumbfound","Stinking Cloud","Summon Beast",
						"Conflagration","Fireball","Weak Summoning","Web","Poison",
						"Ice Bolt","Slow Group","Major Haste","Firestorm","Summoning",
						"Shockstorm","Major Poison","Kill","Daemon","Major Blessing",
						"Major Summoning","Shockwave"};
static const std::array m_priest_sp{"Minor Bless","Light Heal","Wrack","Stumble","Bless",
						"Curse","Wound","Summon Spirit","Disease","Heal",
						"Holy Scourge","Smite","Curse All","Sticks to Snakes","Martyr's Shield",
						"Bless All","Major Heal","Flamestrike","Summon Host","Heal All",
						"Unholy Ravaging","Summon Guardian","Pestilence","Revive All","Avatar",
						"Divine Thud"};

static std::array < std::array<char, 50>, TEXT_BUF_LEN> text_buffer;
static short buf_pointer = 30;
short mark_where_printing_long;
Boolean printing_long = FALSE;

static const RECT item_buttons_from[7] = {{0,12,14,24},{14,12,28,24},{28,12,42,24},{42,12,56,24},
						{0,24,30,36},{30,24,60,36},{0,36,30,48}};

Boolean string_added = FALSE;
short store_text_x = 0, store_text_y = 0;
extern short stat_screen_mode;

void InsetRect (RECT *rect, short x, short y);

// graphics globals
extern short which_combat_type,stat_window;
extern HWND mainPtr;
extern HWND text_sbar,item_sbar;

extern HBITMAP mixed_gworld, pc_stats_gworld, item_stats_gworld, text_area_gworld,tiny_obj_gworld,party_template_gworld;
extern short terrain_there[9][9];
extern HBITMAP status_pattern_gworld,spec_scen_g;

// game globals
extern party_record_type party;
extern current_town_type c_town;
extern town_item_list 	t_i;
extern unsigned char out[96][96];
extern Adventurers adven;
extern big_tr_type  t_d;
extern short ulx,uly;
extern std::array< std::array<RECT, 6>, 8> item_buttons;
 // name, use, give, drip, info, sell/id
extern std::array< std::array<RECT, 5>, 6> pc_buttons;
 // name, hp, sp, info, trade
extern std::array< std::array<Boolean, 6>, 8> item_area_button_active;
extern std::array< std::array<Boolean, 5>, 6> pc_area_button_active;
extern const std::array<RECT, 9> item_screen_button_rects;
extern short spec_item_array[60];
extern const short abil_chart[200];
// combat globals
extern std::array<short, 9> item_bottom_button_active;

extern location pc_pos[6];
extern unsigned char combat_terrain[64][64];
extern short current_pc;
extern short shop_identify_cost;
extern std::array<short, 8> store_selling_values;
extern Boolean supressing_some_spaces;
extern location ok_space[4];

extern HFONT italic_font,bold_font,small_bold_font;
extern HPALETTE hpal;
extern HDC main_dc;
extern HINSTANCE store_hInstance;
extern piles_of_stuff_dumping_type5 data_store5;

short current_item_button[6] = {-1,-1,-1,-1,-1,-1};
short pc_button_state[6] = {-1,-1,-1,-1,-1,-1};

static void place_buy_button(short position,short pc_num,short item_num,HDC hdc);
static void place_item_bottom_buttons();
static void place_item_button(short which_button_to_put,short which_slot,short which_button_position,short extra_val);
static short print_terrain(location space);
static void draw_pc_effects_bmp(short pc, HBITMAP dest_bmp);


static void DrawString(const char* string, short x, short y, HDC hdc)
{
	RECT text_r = { 0,0,450,20 };
	OffsetRect(&text_r, x, y);
	DrawText(hdc, string, -1, &text_r, DT_LEFT | DT_SINGLELINE | DT_TOP | DT_NOCLIP);
}

short text_pc_has_abil_equip(short pc_num,short abil)
{
	short i = 0;
	while (((adven[pc_num].items[i].variety == item_variety::None) || (adven[pc_num].items[i].ability != abil)
		|| (adven[pc_num].equip[i] == FALSE)) && (i < 24))
	{
		i++;
	}
	return i;

}

const RECT c_final_from_draw_rect{ 0,0,271,116 };
const RECT c_final_to_draw_rect{ 0,0,271,116 };
const RECT c_small_erase_rects[3]{ {34,101,76,114},{106,101,147,114},{174,101,201,114} };
const RECT c_info_from{ 1,0,13,12 };
const COLORREF c_colors[6]{ RGB(0,0,0),RGB(255,0,0),RGB(128,0,0),RGB(0,160,0),RGB(0,0,255),RGB(255,255,255) };

// Draws the pc area in upper right
void put_pc_screen()
{
	Boolean right_buttons_same = TRUE;

	for (short i = 0; i < 6; i++)
	{
		if (((adven[i].main_status != status_type::Absent) && (pc_button_state[i] != 1)) ||
			((adven[i].main_status == status_type::Absent) && (pc_button_state[i] != 0)))
		{
			right_buttons_same = FALSE;
		}
	}

	// First refresh gworld with the pc info
	RECT erase_rect{ 2,17,270,99 };
	//rect_draw_some_item_bmp(orig_pc_info_screen_gworld, erase_rect, pc_info_screen_gworld, erase_rect, 0, 0);
	// First clean up gworld with pretty patterns
	// Will rewrite later
	//FillCRECT(&erase_rect,bg[6]);
	erase_rect.right -= 13;
	//if (right_buttons_same == TRUE)
	//	erase_rect.right -= 15;
	RECT from_rect{ erase_rect };
	OffsetRect(&from_rect,-1 * from_rect.left,-1 * from_rect.top);
	rect_draw_some_item_bmp(status_pattern_gworld,from_rect,pc_stats_gworld,erase_rect,0,0);
	//if (right_buttons_same == FALSE) {
	erase_rect.left = erase_rect.right;
	erase_rect.right += 13;
	from_rect = erase_rect;
	OffsetRect(&from_rect,-1 * from_rect.left,-1 * from_rect.top);
	rect_draw_some_item_bmp(status_pattern_gworld,from_rect,pc_stats_gworld,erase_rect,0,0);
	//	}
	for (short i = 0; i < 3; i++)
	{
		//	FillCRECT(&small_erase_rects[i],bg[7]);
		from_rect = c_small_erase_rects[i];
		OffsetRect(&from_rect,-1 * from_rect.left + 208,-1 * from_rect.top + 136);
		rect_draw_some_item_bmp(mixed_gworld,from_rect,pc_stats_gworld, c_small_erase_rects[i],0,0);
	}

	HDC hdc = CreateCompatibleDC(main_dc);
	//store_text_hdc = hdc;
	SelectPalette(hdc,hpal,0);
	SetBkMode(hdc,TRANSPARENT);
	SelectObject(hdc,small_bold_font);

	UINT c[6];
	for (short i = 0; i < 6; i++)
	{
		c[i] = GetNearestPaletteIndex(hpal, c_colors[i]);
	}

	HGDIOBJ store_bmp = SelectObject(hdc,pc_stats_gworld);

	// Put food, gold, day
	SetTextColor(hdc,PALETTEINDEX(c[5]));
	win_draw_string(hdc, c_small_erase_rects[1], std::format("{:d}", (short)party.gold),0,10);
	win_draw_string(hdc, c_small_erase_rects[0], std::format("{:d}", (short)party.food),0,10);
	win_draw_string(hdc, c_small_erase_rects[2], std::format("{:d}", calc_day()),0,10);
	SetTextColor(hdc,PALETTEINDEX(c[0]));

	for (short i = 0; i < 6; i++)
	{
		if (adven[i].main_status != status_type::Absent)
		{
			for (short j = 0; j < 5; j++)
				pc_area_button_active[i][j] = 1;
			if (i == current_pc)
			{
				//TextFace(italic | bold);
				//ForeColor(blueColor);
				SelectObject(hdc,italic_font);
				SetTextColor(hdc,PALETTEINDEX(c[4]));
			}
			win_draw_string(hdc,pc_buttons[i][0], std::format("{:d}. {:<20s}             ", i + 1, adven[i].name),0,10);
			//TextFace(bold);
			//ForeColor(blackColor);
			SelectObject(hdc,small_bold_font);
			SetTextColor(hdc,PALETTEINDEX(c[0]));

			RECT to_draw_rect = pc_buttons[i][1];      
			to_draw_rect.right += 20;
			switch (adven[i].main_status)
			{
			case status_type::Normal:
				if (adven[i].cur_health == adven[i].max_health)
				{
					SetTextColor(hdc, PALETTEINDEX(c[3]));
				}
				else
				{
					SetTextColor(hdc, PALETTEINDEX(c[1]));
				}
				win_draw_string(hdc,pc_buttons[i][1], std::format("{:-3d}              ", adven[i].cur_health),0,10);
				if (adven[i].cur_sp == adven[i].max_sp)
				{
					SetTextColor(hdc, PALETTEINDEX(c[4]));
				}
				else
				{
					SetTextColor(hdc, PALETTEINDEX(c[2]));
				}
				win_draw_string(hdc,pc_buttons[i][2], std::format("{:-3d}              ", adven[i].cur_sp),0,10);
				SetTextColor(hdc,PALETTEINDEX(c[0]));
				SelectObject(hdc,store_bmp);
				draw_pc_effects_bmp(i, pc_stats_gworld);
				SelectObject(hdc,pc_stats_gworld);
				break;
			case status_type::Dead:
				win_draw_string(hdc, to_draw_rect, "Dead", 0, 10);
				break;
			case status_type::Dust:
				win_draw_string(hdc, to_draw_rect, "Dust", 0, 10);
				break;
			case status_type::Stone:
				win_draw_string(hdc, to_draw_rect, "Stone", 0, 10);
				break;
			case status_type::Fled:
				win_draw_string(hdc, to_draw_rect, "Fled", 0, 10);
				break;
			case status_type::Surface:
				win_draw_string(hdc, to_draw_rect, "Surface", 0, 10);
				break;
			case status_type::Won:
				win_draw_string(hdc, to_draw_rect, "Won", 0, 10);
				break;
			default:
				win_draw_string(hdc, to_draw_rect, "Absent", 0, 10);
				break;
			}

			// Now put trade and info buttons
			//rect_draw_some_item_bmp(mixed_gworld,info_from,pc_stats_gworld,pc_buttons[i][3],1,0);
			//rect_draw_some_item_bmp(mixed_gworld,switch_from,pc_stats_gworld,pc_buttons[i][4],1,0);
			// do faster!
			to_draw_rect = pc_buttons[i][3];
			to_draw_rect.right = pc_buttons[i][4].right + 1;
			from_rect = c_info_from;
			from_rect.right = from_rect.left + to_draw_rect.right - to_draw_rect.left;
			pc_button_state[i] = 1;
			SelectObject(hdc,store_bmp);
			rect_draw_some_item_bmp(mixed_gworld,from_rect,pc_stats_gworld,to_draw_rect,1,0);
			SelectObject(hdc,pc_stats_gworld);
		}
		else
		{
			for (short j = 0; j < 5; j++)
			{
				pc_area_button_active[i][j] = 0;
			}
			pc_button_state[i] = 0;
		}
	}

	// Now put text on window.
	SelectObject(hdc,store_bmp);
	DeleteObject(hdc);

	// Now put text on window.
	rect_draw_some_item_bmp(pc_stats_gworld, c_final_from_draw_rect, pc_stats_gworld, offset_rect(c_final_to_draw_rect, PC_WIN_UL_X, PC_WIN_UL_Y), 0, 1);

	// Sometimes this gets called when character is slain. when that happens, if items for
	// that PC are up, switch item page.
	if ((current_pc < 6) && (adven[current_pc].main_status != status_type::Normal) && (stat_window == current_pc))
	{
		set_stat_window(current_pc);
	}
}

// Draws item area in middle right
// Screen_num is what page is visible in the item menu.
//    0 - 5 pc inventory  6 - special item  7 - missions
// Stat_screen_mode ... 0 - normal, adventuring, all buttons vis
//						1 - in shop, item info only
//						2 - in shop, identifying, shop_identify_cost is cost
//						3 - in shop, selling weapons
//						4 - in shop, selling armor
//						5 - in shop, selling all
//						6 - in shop, augmenting weapon,shop_identify_cost is type
void put_item_screen(short screen_num,short suppress_buttons)
// if suppress_buttons > 0, save time by not redrawing buttons
{
	short i_num,item_offset;
	short i = 0,j,pc;
	RECT erase_rect = {2,17,255,123},dest_rect,from_rect,to_rect;
	RECT upper_frame_rect = {3,3,268,16};
	RECT parts_of_area_to_draw[3] = {{0,0,271,17},{0,16,256,123},{0,123,271,144}};

	HDC hdc;
	COLORREF colors[6] = {RGB(0,0,0),RGB(255,0,0),RGB(128,0,0),RGB(0,160,0),RGB(0,0,255),RGB(255,255,255)};
	UINT c[6];
	HGDIOBJ store_bmp;

	// First refresh gworld with the pc info
	//rect_draw_some_item_bmp(orig_pc_info_screen_gworld, erase_rect, pc_info_screen_gworld, erase_rect, 0, 0);
	// First clean up gworld with pretty patterns
	// Will rewrite later
	//FillCRECT(&erase_rect,bg[6]);
	from_rect = erase_rect;
	OffsetRect(&from_rect,-1 * from_rect.left,-1 * from_rect.top);
	rect_draw_some_item_bmp(status_pattern_gworld,from_rect,item_stats_gworld,erase_rect,0,0);
	if (suppress_buttons == 0)
	{
		for (i = 0; i < 6; i++)
		{
			if ((adven[i].main_status != status_type::Normal) && (current_item_button[i] != -1))
			{
				current_item_button[i] = -1;
				//FillCRECT(&item_screen_button_rects[i],bg[7]);
				from_rect = item_screen_button_rects[i];
				OffsetRect(&from_rect, -1 * from_rect.left + 208, -1 * from_rect.top + 136);
				rect_draw_some_item_bmp(mixed_gworld, from_rect, item_stats_gworld, item_screen_button_rects[i], 0, 0);
			}
		}
	}
	//FillCRECT(&upper_frame_rect,bg[7]);
	to_rect = upper_frame_rect;
	to_rect.right = to_rect.left + 96;
	from_rect = to_rect;
	OffsetRect(&from_rect,-1 * from_rect.left + 208,-1 * from_rect.top + 136);
	rect_draw_some_item_bmp(mixed_gworld,from_rect,item_stats_gworld,to_rect,0,0);
	OffsetRect(&to_rect,96,0);
	from_rect = to_rect;
	OffsetRect(&from_rect,-1 * from_rect.left + 208,-1 * from_rect.top + 136);
	rect_draw_some_item_bmp(mixed_gworld,from_rect,item_stats_gworld,to_rect,0,0);

	hdc = CreateCompatibleDC(main_dc);
	//store_text_hdc = hdc;
	SelectPalette(hdc,hpal,0);
	SetBkMode(hdc,TRANSPARENT);
	SelectObject(hdc,small_bold_font);

	for (i = 0; i < 6; i++)
	{
		c[i] = GetNearestPaletteIndex(hpal, colors[i]);
	}

	store_bmp = SelectObject(hdc,item_stats_gworld);

	// Draw buttons at bottom
	if (suppress_buttons == 0)
	{
		//for (i = 0; i < 6; i++)
		//	FillCRECT(&item_screen_button_rects[i],bg[7]);
	}

	item_offset = GetScrollPos(item_sbar,SB_CTL);
	SetTextColor(hdc,PALETTEINDEX(c[5]));

	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 6; j++)
		{
			item_area_button_active[i][j] = FALSE;
		}
	}

	switch (screen_num)
	{
	case 6: // On special items page
		//TextFace(bold);
		SelectObject(hdc,bold_font);
		win_draw_string(hdc,upper_frame_rect, "Special items:",0,10);
		SetTextColor(hdc,PALETTEINDEX(c[0]));
		for (i = 0; i < 8; i++)
		{
			i_num = i + item_offset;
			if (spec_item_array[i_num] >= 0)
			{
				win_draw_string(hdc,item_buttons[i][0], data_store5.scen_strs[60 + spec_item_array[i_num] * 2],0,10);
				SelectObject(hdc,store_bmp);
				place_item_button(3,i,4,0);
				if ((scenario.special_item(spec_item_array[i_num]) % 10 == 1) && (is_not_combat()))
				{
					place_item_button(0, i, 3, 0);
				}
				SelectObject(hdc,item_stats_gworld);
			}
		}
		break;

	case 7: // On jobs page
		break;

	default: // on an items page
		pc = screen_num;
		win_draw_string(hdc,upper_frame_rect, std::format("{} inventory:", adven[pc].name),0,10);
		SetTextColor(hdc,PALETTEINDEX(c[0]));
		for (i = 0; i < 8; i++)
		{
			i_num = i + item_offset;
			win_draw_string(hdc,item_buttons[i][0], std::format("{:d}.", i_num + 1),0,10);
			dest_rect = item_buttons[i][0];
			dest_rect.left += 36;
			if (adven[pc].items[i_num].variety == item_variety::None)
			{

			}
			else
			{
				if (adven[pc].equip[i_num] == TRUE)
				{
					//TextFace(italic | bold);
					SelectObject(hdc,italic_font);
					if (adven[pc].items[i_num].variety < item_variety::Gold)
					{
						SetTextColor(hdc, PALETTEINDEX(c[1]));
					}
					else if ((adven[pc].items[i_num].variety >= item_variety::Shield) && (adven[pc].items[i_num].variety <= item_variety::Boots))
					{
						SetTextColor(hdc, PALETTEINDEX(c[3]));
					}
					else
					{
						SetTextColor(hdc, PALETTEINDEX(c[4]));
					}
				}
				else
				{
					SetTextColor(hdc, PALETTEINDEX(c[0]));
				}
				// Place object graphic here
				//if (adven[pc].items[i_num].variety != item_variety::None)
				//	draw_obj_graphic(i + ((which_item_page[pc] == 1) ? 1 : 0),adven[pc].items[i_num].graphic_num,text_panel_rect); // rect is space holder
				std::string to_draw;
				if (!is_ident(adven[pc].items[i_num]))
				{
					to_draw = std::format("{}  ", adven[pc].items[i_num].name);
				}
				else /// Don't place # of charges when Sell button up and space tight
				{
					if ((adven[pc].items[i_num].charges > 0) && (adven[pc].items[i_num].type != 2) && (stat_screen_mode <= 1))
					{
						to_draw = std::format("{} ({:d})", adven[pc].items[i_num].full_name, adven[pc].items[i_num].charges);
					}
					else
					{
						to_draw = adven[pc].items[i_num].full_name;
					}
				}
				dest_rect.left -= 2;
				win_draw_string(hdc,dest_rect,to_draw,0,10);
				//TextFace(0);
				//TextFace(bold);
				//ForeColor(blackColor);
				SelectObject(hdc,small_bold_font);
				SetTextColor(hdc,PALETTEINDEX(c[0]));

				// this is kludgy, awkwark, and has redundant code. Done this way to
				// make go faster, and I got lazy.
				SelectObject(hdc,store_bmp);
				if ((stat_screen_mode == 0) &&
					(is_town() || is_out() || (is_combat() && (pc == current_pc)))) // place give and drop and use
				{
					place_item_button(0,i,0,adven[pc].items[i_num].graphic_num); // item_graphic
					if (abil_chart[adven[pc].items[i_num].ability] != 4) // place use if can
					{
						place_item_button(10, i, 1, 0);
					}
					else
					{
						place_item_button(11, i, 1, 0);
					}
				}
				else
				{
					place_item_button(0,i,0,adven[pc].items[i_num].graphic_num); // item_graphic
					place_item_button(3,i,4,0); // info button
					if ((stat_screen_mode == 0) &&
						(is_town() || is_out() || (is_combat() && (pc == current_pc)))) // place give and drop and use
					{
						place_item_button(1,i,2,0);
						place_item_button(2,i,3,0);
						if (abil_chart[adven[pc].items[i_num].ability] != 4) // place use if can
						{
							place_item_button(0, i, 1, 0);
						}
					}
				}
				if (stat_screen_mode > 1)
				{
					place_buy_button(i,pc,i_num,hdc);
				}
				SelectObject(hdc,item_stats_gworld);
			} // end of if item is there
		} // end of for (i = 0; i < 8; i++)
		break;
	}

	// Now put text on window.
	SelectObject(hdc,store_bmp);
	DeleteObject(hdc);

	place_item_bottom_buttons();

	// Now put text on window.
	for (i = 0; i < 3; i++)
	{
		dest_rect = parts_of_area_to_draw[i];
		OffsetRect(&dest_rect,ITEM_WIN_UL_X,ITEM_WIN_UL_Y);
		rect_draw_some_item_bmp(item_stats_gworld, parts_of_area_to_draw[i],item_stats_gworld, dest_rect, 0, 1);
	}
}

static void place_buy_button(short position,short pc_num,short item_num,HDC hdc)
{
	RECT dest_rect,source_rect;
	RECT button_sources[3] = {{0,24,30,36},{30,24,60,36},{0,36,30,48}};
	short val_to_place;
	short aug_cost[10] = {4,7,10,8, 15,15,10, 0,0,0};
	HGDIOBJ store_bmp;

	if (adven[pc_num].items[item_num].variety == item_variety::None)
	{
		return;
	}

	dest_rect = item_buttons[position][5];

	val_to_place = (adven[pc_num].items[item_num].charges > 0) ?
		adven[pc_num].items[item_num].charges * adven[pc_num].items[item_num].value :
		adven[pc_num].items[item_num].value;
	val_to_place = val_to_place / 2;

	switch (stat_screen_mode)
	{
	case 2:
		if (!is_ident(adven[pc_num].items[item_num]))
		{
			item_area_button_active[position][5] = TRUE;
			source_rect = button_sources[0];
			val_to_place = shop_identify_cost;
		}
		break;
	case 3: // sell weapons 
		if (((adven[pc_num].items[item_num].variety < item_variety::PotionOrMagicItem) || (adven[pc_num].items[item_num].variety == item_variety::Crossbow) ||
			(adven[pc_num].equip[item_num] == FALSE) &&
			(adven[pc_num].items[item_num].variety == item_variety::Bolts)) &&
			is_ident(adven[pc_num].items[item_num]) && (val_to_place > 0) &&
				!is_cursed(adven[pc_num].items[item_num]))
		{
			item_area_button_active[position][5] = TRUE;
			source_rect = button_sources[1];
		}
		break;
	case 4: // sell armor
		if ((adven[pc_num].items[item_num].variety >= item_variety::Shield) && (adven[pc_num].items[item_num].variety <= item_variety::Boots) &&
			(adven[pc_num].equip[item_num] == FALSE) &&
			is_ident(adven[pc_num].items[item_num]) && (val_to_place > 0) &&
			!is_cursed(adven[pc_num].items[item_num]))
		{
			item_area_button_active[position][5] = TRUE;
			source_rect = button_sources[1];
		}
		break;
	case 5: // sell any
		if ((val_to_place > 0) && is_ident(adven[pc_num].items[item_num]) && 
			(adven[pc_num].equip[item_num] == FALSE) &&
				!is_cursed(adven[pc_num].items[item_num]))
		{
			item_area_button_active[position][5] = TRUE;
			source_rect = button_sources[1];
		}
		break;
	case 6: // augment weapons 
		if ((adven[pc_num].items[item_num].variety < item_variety::Gold) &&
			is_ident(adven[pc_num].items[item_num]) &&
			(adven[pc_num].items[item_num].ability == 0) &&
			!is_magic(adven[pc_num].items[item_num]))
		{
			item_area_button_active[position][5] = TRUE;
			source_rect = button_sources[2];
			val_to_place = max(aug_cost[shop_identify_cost] * 100,adven[pc_num].items[item_num].value * (5 + aug_cost[shop_identify_cost]));
		}
		break;
	}
	if (item_area_button_active[position][5] == TRUE)
	{
		store_selling_values[position] = val_to_place;
		dest_rect = item_buttons[position][5];
		dest_rect.right = dest_rect.left + 30;
		rect_draw_some_item_bmp(mixed_gworld, source_rect,item_stats_gworld, dest_rect, 1, 0);
		//if (val_to_place >= 10000)
		//	TextFace(0);
		store_bmp = SelectObject(hdc,item_stats_gworld);
		win_draw_string(hdc,item_buttons[position][5], std::format("        {:d}", val_to_place), 2, 10);
		SelectObject(hdc,store_bmp);
		//TextFace(bold);
	}
}

//extern Boolean item_area_button_active[8][6];
 // name, use, give, drop, info, sell/id
// shortcuts - if which_button_to_put is 10, all 4 buttons now
//				if which_button_to_put is 11, just right 2
static void place_item_button(short which_button_to_put,short which_slot,short which_button_position,short extra_val)
{
	RECT from_rect = {0,0,18,18},to_rect;

	if (which_button_position == 0) // this means put little item graphic, extra val is which_graphic
	{
		item_area_button_active[which_slot][which_button_position] = TRUE;
		OffsetRect(&from_rect,(extra_val % 10) * 18,(extra_val / 10) * 18);
		to_rect = item_buttons[which_slot][0];
		to_rect.right = to_rect.left + (to_rect.bottom - to_rect.top);
		InsetRect(&to_rect,-1,-1);
		OffsetRect(&to_rect,20,1);
		InsetRect(&from_rect,2,2);
		if (extra_val >= 150)
		{
			from_rect = get_custom_rect(extra_val - 150);
			rect_draw_some_item_bmp(spec_scen_g, from_rect,item_stats_gworld, to_rect, 0, 0);
		}
		else
		{
			rect_draw_some_item_bmp(tiny_obj_gworld, from_rect,item_stats_gworld, to_rect, 1, 0);
		}
		return;
	}

	if (which_button_to_put < 4) // this means put a regular item button
	{
		item_area_button_active[which_slot][which_button_position] = TRUE;
		rect_draw_some_item_bmp(mixed_gworld, item_buttons_from[which_button_to_put],
			item_stats_gworld, item_buttons[which_slot][which_button_position], 1, 0);
	}

	if (which_button_to_put == 10) // this means put all 4
	{
		item_area_button_active[which_slot][1] = TRUE;
		item_area_button_active[which_slot][2] = TRUE;
		item_area_button_active[which_slot][3] = TRUE;
		item_area_button_active[which_slot][4] = TRUE;
		from_rect = item_buttons_from[0]; from_rect.right = item_buttons_from[3].right;
		to_rect = item_buttons[which_slot][1];
		to_rect.right = to_rect.left + from_rect.right - from_rect.left;
		rect_draw_some_item_bmp(mixed_gworld, from_rect,item_stats_gworld, to_rect, 1, 0);
	}

	if (which_button_to_put == 11) // this means put right 3
	{
		item_area_button_active[which_slot][2] = TRUE;
		item_area_button_active[which_slot][3] = TRUE;
		item_area_button_active[which_slot][4] = TRUE;
		from_rect = item_buttons_from[1]; from_rect.right = item_buttons_from[3].right;
		to_rect = item_buttons[which_slot][2];
		to_rect.right = to_rect.left + from_rect.right - from_rect.left;
		rect_draw_some_item_bmp(mixed_gworld, from_rect,item_stats_gworld, to_rect, 1, 0);
	}
}

RECT get_custom_rect (short which_rect)
{
	RECT store_rect = {0,0,28,36};
	OffsetRect(&store_rect,28 * (which_rect % 10),36 * (which_rect / 10));
	return store_rect;
}

static void place_item_bottom_buttons()
{
	RECT pc_from_rect = {0,0,28,36},but_from_rect = {36,85,54,101},to_rect;
	short i;

	for (i = 0; i < 6; i++)
	{
		if (adven[i].main_status == status_type::Normal)
		{
			item_bottom_button_active[i] = TRUE;
			to_rect = item_screen_button_rects[i];
			//if (current_item_button[i] != adven[i].which_graphic) {
			current_item_button[i] = adven[i].which_graphic;
			rect_draw_some_item_bmp(mixed_gworld, but_from_rect, item_stats_gworld, to_rect, 0, 0);
			pc_from_rect = get_party_template_rect(i,0);
			InsetRect(&to_rect,2,2);
			rect_draw_some_item_bmp(party_template_gworld, pc_from_rect, item_stats_gworld, to_rect, 0, 0);
			//	}
		}
		else
		{
			item_bottom_button_active[i] = FALSE;
		}
	}
}

RECT get_party_template_rect(short pc_num,short mode)
{
	RECT source_rect;

	source_rect.top = (pc_num % 3) * BITMAP_HEIGHT;
	source_rect.bottom = 36 + (pc_num % 3) * BITMAP_HEIGHT;
	source_rect.left = (pc_num / 3) * BITMAP_WIDTH * 2 + ((mode == 1) ? 28 : 0);
	source_rect.right = source_rect.left + BITMAP_WIDTH;

	return source_rect;
}


void set_stat_window(short new_stat)
{
	short i,array_pos = 0;
	stat_window = new_stat;
	if ((stat_window < 6) && (adven[stat_window].main_status != status_type::Normal))
	{
		stat_window = first_active_pc();
	}

	switch (stat_window)
	{
	case 6:
		for (i = 0; i < 60; i++)
		{
			spec_item_array[i] = -1;
		}
		for (i = 0; i < 50; i++)
		{
			if (party.spec_items[i] > 0)
			{
				spec_item_array[array_pos] = i;
				array_pos++;
			}
		}
		array_pos = max(0,array_pos - 8);
		//SetControlMaximum(item_sbar,array_pos);
		SetScrollRange(item_sbar,SB_CTL,0,array_pos,FALSE);
		break;
	case 7:
		SetScrollRange(item_sbar,SB_CTL,0,2,FALSE);
		break;
	default:
		SetScrollRange(item_sbar,SB_CTL,0,16,FALSE);
		break;
	}
	//SetControlValue(item_sbar,0);
	SetScrollPos(item_sbar,SB_CTL,0,TRUE);
	put_item_screen(stat_window,0);
}

short first_active_pc()
{
	short i = 0;
	for (i = 0; i < 6; i++)
	{
		if (adven[i].main_status == status_type::Normal)
		{
			return i;
		}
	}
	return 6;
}


void refresh_stat_areas(short mode)
{
	short i,x;
	RECT dest_rect,parts_of_area_to_draw[3] = {{0,0,271,17},{0,16,256,123},{0,123,271,144}};
	RECT pc_stats_from =  {0,0,271,116},text_area_from = {0,0,256,138};
	x = mode * 10;
	dest_rect = pc_stats_from;
	OffsetRect(&dest_rect,PC_WIN_UL_X,PC_WIN_UL_Y);
	rect_draw_some_item_bmp(pc_stats_gworld, pc_stats_from, pc_stats_gworld, dest_rect, x, 1);
	for (i = 0; i < 3; i++)
	{
		dest_rect = parts_of_area_to_draw[i];
		OffsetRect(&dest_rect,ITEM_WIN_UL_X,ITEM_WIN_UL_Y);
		rect_draw_some_item_bmp(item_stats_gworld, parts_of_area_to_draw[i],item_stats_gworld, dest_rect, x, 1);
	}
	dest_rect = text_area_from;
	OffsetRect(&dest_rect,TEXT_WIN_UL_X,TEXT_WIN_UL_Y);
	rect_draw_some_item_bmp(text_area_gworld, text_area_from, text_area_gworld, dest_rect, x, 1);
}


const RECT c_source_rects[18] = {
	BOE_INIT_RECT(55,0,67,12), BOE_INIT_RECT(55,12,67,24), BOE_INIT_RECT(55,24,67,36),
	BOE_INIT_RECT(67,0,79,12), BOE_INIT_RECT(67,12,79,24), BOE_INIT_RECT(67,24,79,36),
	BOE_INIT_RECT(79,0,91,12), BOE_INIT_RECT(79,12,91,24), BOE_INIT_RECT(79,24,91,36),
	BOE_INIT_RECT(91,0,103,12), BOE_INIT_RECT(91,12,103,24), BOE_INIT_RECT(91,24,103,36),
	BOE_INIT_RECT(103,0,115,12), BOE_INIT_RECT(103,12,115,24), BOE_INIT_RECT(103,24,115,36),
	BOE_INIT_RECT(115,0,127,12), BOE_INIT_RECT(115,12,127,24), BOE_INIT_RECT(115,24,127,36)
};

// dest: 0 - in gworld  2 - on dialog
static void draw_pc_effects_ex(RectDrawDestination dest_bmp, const pc_record_type& adventurer, const RECT& dest_rect_start, short right_limit, short mode, short dest)
{
	RECT dest_rect{ dest_rect_start };

	if (hacks_adventurer_without_split_status(adventurer) != status_type::Normal)
	{
		return;
	}
			
	if ((adventurer.gaffect(affect::PoisonedWeapon) > 0) && (dest_rect.right < right_limit))
	{
		rect_draw_some_item(mixed_gworld,c_source_rects[4],dest_bmp,dest_rect,mode,dest);
		dest_rect.left += 13;
		dest_rect.right += 13;
	}
	if (adventurer.gaffect(affect::CursedBlessed) > 0)
	{
		rect_draw_some_item(mixed_gworld,c_source_rects[2],dest_bmp,dest_rect,mode,dest);
		dest_rect.left += 13;
		dest_rect.right += 13;
	}
	if (adventurer.gaffect(affect::CursedBlessed) < 0)
	{
		rect_draw_some_item(mixed_gworld,c_source_rects[3],dest_bmp,dest_rect,mode,dest);
		dest_rect.left += 13;
		dest_rect.right += 13;
	}
	if (adventurer.gaffect(affect::Poisoned) > 0)
	{
		rect_draw_some_item(mixed_gworld,c_source_rects[(adventurer.gaffect(affect::Poisoned) > 4) ? 1 : 0],dest_bmp,dest_rect,mode,dest);
		dest_rect.left += 13;
		dest_rect.right += 13;
	}
	if (adventurer.gaffect(affect::Invulnerable) > 0)
	{
		rect_draw_some_item(mixed_gworld,c_source_rects[5],dest_bmp,dest_rect,mode,dest);
		dest_rect.left += 13;
		dest_rect.right += 13;
	}
	if (adventurer.gaffect(affect::Speed) > 0)
	{
		rect_draw_some_item(mixed_gworld,c_source_rects[6],dest_bmp,dest_rect,mode,dest);
		dest_rect.left += 13;
		dest_rect.right += 13;
	}
	if (adventurer.gaffect(affect::Speed) < 0)
	{
		rect_draw_some_item(mixed_gworld,c_source_rects[8],dest_bmp,dest_rect,mode,dest);
		dest_rect.left += 13;
		dest_rect.right += 13;
	}
	if ((adventurer.gaffect(affect::MagicResistant) > 0) && (dest_rect.right < right_limit))
	{
		rect_draw_some_item(mixed_gworld,c_source_rects[9],dest_bmp,dest_rect,mode,dest);
		dest_rect.left += 13;
		dest_rect.right += 13;
	}
	if ((adventurer.gaffect(affect::Webbed) > 0) && (dest_rect.right < right_limit))
	{
		rect_draw_some_item(mixed_gworld,c_source_rects[10],dest_bmp,dest_rect,mode,dest);
		dest_rect.left += 13;
		dest_rect.right += 13;
	}
	if ((adventurer.gaffect(affect::Diseased) > 0) && (dest_rect.right < right_limit))
	{
		rect_draw_some_item(mixed_gworld,c_source_rects[11],dest_bmp,dest_rect,mode,dest);
		dest_rect.left += 13;
		dest_rect.right += 13;
	}
	if ((adventurer.gaffect(affect::Sanctuary) > 0) && (dest_rect.right < right_limit))
	{
		rect_draw_some_item(mixed_gworld,c_source_rects[12],dest_bmp,dest_rect,mode,dest);
		dest_rect.left += 13;
		dest_rect.right += 13;
	}
	if ((adventurer.gaffect(affect::Dumbfounded) > 0) && (dest_rect.right < right_limit))
	{
		rect_draw_some_item(mixed_gworld,c_source_rects[13],dest_bmp,dest_rect,mode,dest);
		dest_rect.left += 13;
		dest_rect.right += 13;
	}	
	if ((adventurer.gaffect(affect::MartyrsShield) > 0) && (dest_rect.right < right_limit))
	{
		rect_draw_some_item(mixed_gworld,c_source_rects[14],dest_bmp,dest_rect,mode,dest);
		dest_rect.left += 13;
		dest_rect.right += 13;
	}	
	if ((adventurer.gaffect(affect::Asleep) > 0) && (dest_rect.right < right_limit))
	{
		rect_draw_some_item(mixed_gworld,c_source_rects[15],dest_bmp,dest_rect,mode,dest);
		dest_rect.left += 13;
		dest_rect.right += 13;
	}
	if ((adventurer.gaffect(affect::Paralyzed) > 0) && (dest_rect.right < right_limit))
	{
		rect_draw_some_item(mixed_gworld,c_source_rects[16],dest_bmp,dest_rect,mode,dest);
		dest_rect.left += 13;
		dest_rect.right += 13;
	}
	if ((adventurer.gaffect(affect::Acid) > 0) && (dest_rect.right < right_limit))
	{
		rect_draw_some_item(mixed_gworld,c_source_rects[17],dest_bmp,dest_rect,mode,dest);
		dest_rect.left += 13;
		dest_rect.right += 13;
	}
}

void draw_pc_effects_dc(short pc, HDC dest_dc)
{
	RECT dest_rect = BOE_INIT_RECT(66, 354, 78, 366);
	dest_rect.top += pc * 24 + 18;
	dest_rect.bottom += pc * 24 + 18;
	draw_pc_effects_ex(dest_dc, adven[pc], dest_rect, 490, 0, 2);
}

static void draw_pc_effects_bmp(short pc, HBITMAP dest_bmp)
{
	const short name_width = string_length(adven[pc].name, main_dc);
	RECT dest_rect = BOE_INIT_RECT(18, 15, 30, 27);
	dest_rect.left = name_width + 33;
	dest_rect.right = dest_rect.left + 12;
	dest_rect.top += pc * 13;
	dest_rect.bottom += pc * 13;
	draw_pc_effects_ex(dest_bmp, adven[pc], dest_rect, pc_buttons[0][1].left - 5, 1, 0);
}

void print_party_stats() {
	add_string_to_buf("PARTY STATS:");
	add_string_to_buf("  Number of kills: {:d}                   ", party.total_m_killed);
	if (is_town() || (is_combat() && (which_combat_type == 1)))
	{
		add_string_to_buf("  Kills in this town: {:d}                   ", party.m_killed[c_town.town_num]);
	}
	add_string_to_buf("  Total experience: {:d}                   ", party.total_xp_gained);
	add_string_to_buf("  Total damage done: {:d}                   ", party.total_dam_done);
	add_string_to_buf("  Total damage taken: {:d}                   ", party.total_dam_taken);
	print_buf();
}


short do_look(location space)
{
	short i,j,num_items = 0;
	Boolean gold_here = FALSE, food_here = FALSE, is_lit = TRUE;
	location from_where;
		
	from_where = get_cur_loc();
	is_lit = pt_in_light(from_where,space);

	if ((is_out() && (same_point(space, party.p_loc))) ||
		(is_town() && (same_point(space, c_town.p_loc))))
	{
		add_string_to_buf("    Your party");
	}

	if (is_combat())
	{
		for (i = 0; i < 6; i++)
		{
			if ((same_point(space, pc_pos[i])) && (adven[i].main_status == status_type::Normal)
				&& (is_lit == TRUE) && (can_see(pc_pos[current_pc], space, 0) < 5))
			{
				add_string_to_buf("    {}", adven[i].name);
			}
		}
	}

	if (is_not_out())
	{
		for (i = 0; i < T_M; i++)
		{
			if ((c_town.monst.dudes[i].active != 0) && (is_lit == TRUE)
				&& (monst_on_space(space, i) == TRUE) &&
				(is_town() || (can_see(pc_pos[current_pc], space, 0) < 5))
				&& (c_town.monst.dudes[i].m_d.picture_num != 0))
			{
				const char* monster_name = scenario_ext.monster_name(c_town.monst.dudes[i].number);
				if (c_town.monst.dudes[i].m_d.health < c_town.monst.dudes[i].m_d.m_health)
				{
					if (c_town.monst.dudes[i].attitude % 2 == 1)
					{
						add_string_to_buf("    Wounded {} (H)", monster_name);
					}
					else
					{
						add_string_to_buf("    Wounded {} (F)", monster_name);
					}
				}
				else
				{
					if (c_town.monst.dudes[i].attitude % 2 == 1)
					{
						add_string_to_buf("    {} (H)", monster_name);
					}
					else
					{
						add_string_to_buf("    {} (F)", monster_name);
					}
				}
			}
		}
	}

	if (is_out())
	{
		for (i = 0; i < 10; i++)
		{
			if ((party.out_c[i].exists == TRUE)
				&& (same_point(space,party.out_c[i].m_loc)))
			{
				for (j = 0; j < 7; j++)
				{
					if (party.out_c[i].what_monst.monst[j] != 0)
					{
						add_string_to_buf("    {}", scenario_ext.monster_name(party.out_c[i].what_monst.monst[j]));
						j = 7;
					}
				}
			}
		}

		if (out_boat_there(space) < 30)
			add_string_to_buf("    Boat                ");
		if (out_horse_there(space) < 30)
			add_string_to_buf("    Horse                ");
	}

	if (is_not_out())
	{
		if (town_boat_there(space) < 30)
			add_string_to_buf("    Boat               ");
		if (town_horse_there(space) < 30)
			add_string_to_buf("    Horse               ");
		if (is_web(space.x,space.y))
			add_string_to_buf("    Web               ");
		if (is_crate(space.x,space.y))
			add_string_to_buf("    Crate               ");
		if (is_barrel(space.x,space.y))
			add_string_to_buf("    Barrel               ");
		if (is_fire_barrier(space.x,space.y))
			add_string_to_buf("    Magic Barrier               ");
		if (is_force_barrier(space.x,space.y))
			add_string_to_buf("    Magic Barrier               ");
		if (is_quickfire(space.x,space.y))
			add_string_to_buf("    Quickfire               ");
		if (is_fire_wall(space.x,space.y))
			add_string_to_buf("    Wall of Fire               ");
		if (is_force_wall(space.x,space.y))
			add_string_to_buf("    Wall of Force               ");
		if (is_antimagic(space.x,space.y))
			add_string_to_buf("    Antimagic Field               ");
		if (is_scloud(space.x,space.y))
			add_string_to_buf("    Stinking Cloud               ");
		if (is_ice_wall(space.x,space.y))
			add_string_to_buf("    Ice Wall               ");
		if (is_blade_wall(space.x,space.y))
			add_string_to_buf("    Blade Wall               ");
		if (is_small_blood(space.x,space.y))
			add_string_to_buf("    Blood stain               ");
		if (is_medium_blood(space.x,space.y))
			add_string_to_buf("    Blood stain               ");
		if (is_large_blood(space.x,space.y))
			add_string_to_buf("    Blood stain               ");
		if (is_small_slime(space.x,space.y))
			add_string_to_buf("    Smears of slime               ");
		if (is_big_slime(space.x,space.y))
			add_string_to_buf("    Smears of slime               ");
		if (is_ash(space.x,space.y))
			add_string_to_buf("    Ashes               ");
		if (is_bones(space.x,space.y))
			add_string_to_buf("    Bones               ");
		if (is_rubble(space.x,space.y))
			add_string_to_buf("    Rubble               ");

		for (i = 0; i < NUM_TOWN_ITEMS; i++)
		{
			if ((t_i.items[i].variety != item_variety::None) && (same_point(space,t_i.items[i].item_loc))
				&& (is_lit == TRUE))
			{
				if (t_i.items[i].variety == item_variety::Gold)
				{
					gold_here = TRUE;
				}
				else if (t_i.items[i].variety == item_variety::Food)
				{
					food_here = TRUE;
				}
				else
				{
					num_items++;
				}
			}
		}

		if (gold_here == TRUE)
			add_string_to_buf("    Gold");
		if (food_here == TRUE)
			add_string_to_buf("    Food");
		if (num_items > 8)
		{
			add_string_to_buf("    Many items");
		}
		else
		{
			for (i = 0; i < NUM_TOWN_ITEMS; i++)
			{
				if ((t_i.items[i].variety != item_variety::None) && (t_i.items[i].variety != item_variety::Gold) && (t_i.items[i].variety != item_variety::Food) &&
					(same_point(space, t_i.items[i].item_loc)) && !is_contained(t_i.items[i]))
				{
					if (is_ident(t_i.items[i]))
					{
						add_string_to_buf("    {}", t_i.items[i].full_name);
					}
					else
					{
						add_string_to_buf("    {}", t_i.items[i].name);
					}
				}
			}
		}
	}

	if (is_lit == FALSE)
	{
		add_string_to_buf("    Dark                ");
		return 0;
	}

	return print_terrain(space);
}

short town_boat_there(location where)
{
	short i;

	// Num boats stores highest # of boat in town
	for (i = 0; i < 30; i++)
	{
		if ((party.boats[i].exists == TRUE) && (party.boats[i].which_town == c_town.town_num)
			&& (same_point(where, party.boats[i].boat_loc)))
		{
			return i;
		}
	}
	return 30;
}

short out_boat_there(location where)
{
	short i;
	
	for (i = 0; i < 30; i++)
	{
		if ((party.boats[i].exists == TRUE) && (same_point(where, party.boats[i].boat_loc))
			&& (party.boats[i].which_town == 200))
		{
			return i;
		}
	}
	return 30;
}

short town_horse_there(location where)
{
	short i;
	
	// Num boats stores highest # of boat in town
	for (i = 0; i < 30; i++)
	{
		if ((party.horses[i].exists == TRUE) && (party.horses[i].which_town == c_town.town_num)
			&& (same_point(where, party.horses[i].horse_loc)))
		{
			return i;
		}
	}
	return 30;
}

short out_horse_there(location where)
{
	short i;
	for (i = 0; i < 30; i++)
	{
		if ((party.horses[i].exists == TRUE) && (same_point(where, party.horses[i].horse_loc))
			&& (party.horses[i].which_town == 200))
		{
			return i;
		}
	}
	return 30;
}

void notify_out_combat_began(out_wandering_type encounter,short *nums) 
{
	short i;

	add_string_to_buf("COMBAT!                 ");

	for (i = 0; i < 6; i++)
	{
		if (encounter.monst[i] != 0)
		{
			switch (encounter.monst[i])
			{
			default:
				add_string_to_buf("  {:d} x {}        ", nums[i], scenario_ext.monster_name(encounter.monst[i]));
				break;
			}
		}
	}
	if (encounter.monst[6] != 0)
	{
		add_string_to_buf("  {}        ", scenario_ext.monster_name(encounter.monst[6]));
	}
}

static const char * get_ter_name(unsigned char num)
{
	if ((num == 90) && (is_out() || is_town() || (is_combat() && (which_combat_type == 1))))
	{
		return "Pit";
	}
	else
	{
		return scenario_ext.terrain_name(num);
	}
}

void print_monst_name(unsigned char m_type)
{
	add_string_to_buf("{}:", scenario_ext.monster_name(m_type));
}

void print_monst_attacks(unsigned char m_type,short target)
//short target; // < 100 - pc  >= 100  monst
{
	if (target < 100)
	{
		add_string_to_buf("{} attacks {}", scenario_ext.monster_name(m_type), adven[target].name);
	}
	else
	{
		add_string_to_buf("{} attacks {}", scenario_ext.monster_name(m_type), scenario_ext.monster_name(c_town.monst.dudes[target - 100].number));
	}
}

void damaged_message(short damage,short type)
{
	char str[256];
	get_str(str,20,130 + type);
	add_string_to_buf("  {} for {:d}", str,damage);
}

// This prepares the monster's string for the text bar
std::string format_monster_going(unsigned char m_num,short ap)
{
	return std::format("{} (ap: {:d})", scenario_ext.monster_name(m_num),ap);
}

void monst_spell_note(unsigned char number,short which_mess)
{
	const char * const monster_name = scenario_ext.monster_name(number);

	switch (which_mess)
	{
	case 1:		add_string_to_buf("  {} scared. ", monster_name);break;
	case 2:		add_string_to_buf("  {} slowed. ", monster_name);break;
	case 3:		add_string_to_buf("  {} weakened.", monster_name);break;
	case 4:		add_string_to_buf("  {} poisoned.", monster_name);break;
	case 5:		add_string_to_buf("  {} cursed.", monster_name);break;
	case 6:		add_string_to_buf("  {} ravaged.", monster_name);break;
	case 7:		add_string_to_buf("  {} undamaged.", monster_name);break;
	case 8:		add_string_to_buf("  {} is stoned.", monster_name);break;
	case 9:		add_string_to_buf("  Gazes at {}.", monster_name);break;
	case 10:	add_string_to_buf("  {} resists.", monster_name);break;
	case 11:	add_string_to_buf("  Drains {}.", monster_name);break;
	case 12:	add_string_to_buf("  Shoots at {}.", monster_name);break;
	case 13:	add_string_to_buf("  Throws spear at {}.", monster_name); break;
	case 14:	add_string_to_buf("  Throws rock at {}.", monster_name); break;
	case 15:	add_string_to_buf("  Throws razordisk at {}.", monster_name); break;
	case 16:	add_string_to_buf("  Hits {}.", monster_name); break;
	case 17:	add_string_to_buf("{} disappears.", monster_name); break;
	case 18:	add_string_to_buf("  Misses {}.", monster_name); break;
	case 19:	add_string_to_buf("  {} is webbed.", monster_name);break;
	case 20:	add_string_to_buf("  {} chokes.", monster_name);break;
	case 21:	add_string_to_buf("  {} summoned.", monster_name);break;
	case 22:	add_string_to_buf("  {} is dumbfounded.", monster_name);break;
	case 23:	add_string_to_buf("  {} is charmed.", monster_name);break;
	case 24:	add_string_to_buf("  {} is recorded.", monster_name);break;
	case 25:	add_string_to_buf("  {} is diseased.", monster_name);break;
	case 26:	add_string_to_buf("  {} is an avatar!", monster_name);break;
	case 27:	add_string_to_buf("  {} splits!", monster_name);break;
	case 28:	add_string_to_buf("  {} falls asleep.", monster_name);break;
	case 29:	add_string_to_buf("  {} wakes up.", monster_name);break;
	case 30:	add_string_to_buf("  {} paralyzed.", monster_name);break;
	case 31:	add_string_to_buf("  {} covered with acid.", monster_name);break;
	case 32:	add_string_to_buf("  Fires spines at {}.", monster_name);break;
	}
}

void monst_cast_spell_note(unsigned char number,short spell,short type)
//short type; // 0 - mage 1- priest
{
	add_string_to_buf("{} casts:", scenario_ext.monster_name(number));
	add_string_to_buf("  {}", (type == 1) ? m_priest_sp[spell - 1] : m_mage_sp[spell - 1]);
}

void monst_breathe_note(unsigned char number)
{
	add_string_to_buf("{} breathes.", scenario_ext.monster_name(number));
}

void monst_damaged_mes(short which_m, short how_much, short how_much_spec)
{
	if (how_much_spec > 0)
	{
		add_string_to_buf("  {} takes {:d}+{:d}", scenario_ext.monster_name(c_town.monst.dudes[which_m].number), how_much, how_much_spec);
	}
	else
	{
		add_string_to_buf("  {} takes {:d}", scenario_ext.monster_name(c_town.monst.dudes[which_m].number), how_much);
	}
}

void monst_killed_mes(short which_m)
{
	add_string_to_buf("  {} dies.", scenario_ext.monster_name(c_town.monst.dudes[which_m].number));
}

void print_nums(short a,short b,short c)
{
	add_string_to_buf("debug: {:d} {:d} {:d}", a,b,c);
}

static short print_terrain(location space)
{
	unsigned char which_terrain;
	if (is_out())
	{
		which_terrain = out[space.x][space.y];
	}
	if (is_town())
	{
		which_terrain = t_d.terrain[space.x][space.y];
	}
	if (is_combat())
	{
		which_terrain = combat_terrain[space.x][space.y];
	}
	add_string_to_buf("    {}", get_ter_name(which_terrain));
	return (short) which_terrain;
}


void add_string_to_buf(std::string_view str)
{
	//SetControlValue(text_sbar,58);
	SetScrollPos(text_sbar,SB_CTL,58,TRUE);
	string_added = TRUE;
	if (buf_pointer == mark_where_printing_long)
	{
		printing_long = TRUE;
		print_buf();
		through_sending();
	}
	format_to_buf(text_buffer[buf_pointer].data(), "{:<49.49s}", str.data());
	text_buffer[buf_pointer][49] = 0;
//	c2pstr(text_buffer[buf_pointer].data());
	if (buf_pointer == (TEXT_BUF_LEN - 1))
	{
		buf_pointer = 0;
	}
	else
	{
		buf_pointer++;
	}
}

void init_buf()
{
	for (auto& buffer : text_buffer)
	{
		buffer[0] = ' ';
		buffer[1] = '\0';
	}
}

void print_buf () 
{
	short num_lines_printed = 0,ctrl_val;
	short line_to_print;
	short start_print_point;
	Boolean end_loop = FALSE;
	RECT store_text_rect = {0,0,256,138},dest_rect,erase_rect = {1,1,255,137};
	RECT from_rect,to_rect;
	HDC hdc;
	HGDIOBJ store_bmp;

	if (string_added == TRUE)
	{
		// First clean up gworld with pretty patterns
		//FillCRECT(&erase_rect,bg[6]);
		InsetRect(&erase_rect,1,1);
		erase_rect.right++;
		to_rect = erase_rect;
		to_rect.bottom = to_rect.top + 128;
		from_rect = to_rect;
		OffsetRect(&from_rect,-1 * from_rect.left,-1 * from_rect.top);
		rect_draw_some_item_bmp(status_pattern_gworld,from_rect,text_area_gworld,to_rect,0,0);
		to_rect = erase_rect;
		to_rect.top = to_rect.bottom - 8;
		from_rect = to_rect;
		OffsetRect(&from_rect,-1 * from_rect.left,-1 * from_rect.top);
		rect_draw_some_item_bmp(status_pattern_gworld,from_rect,text_area_gworld,to_rect,0,0);


		hdc = CreateCompatibleDC(main_dc);
		//store_text_hdc = hdc;
		SelectPalette(hdc,hpal,0);
		SetBkMode(hdc,TRANSPARENT);
		SelectObject(hdc,small_bold_font);

		store_bmp = SelectObject(hdc,text_area_gworld);

		//ctrl_val = 58 - GetControlValue(text_sbar);
		ctrl_val = 58 - GetScrollPos(text_sbar,SB_CTL);
		start_print_point = buf_pointer - LINES_IN_TEXT_WIN - ctrl_val;
		if (start_print_point < 0)
		{
			start_print_point = TEXT_BUF_LEN + start_print_point;
		}
		line_to_print= start_print_point;
	
		while ((line_to_print!= buf_pointer) && (num_lines_printed < LINES_IN_TEXT_WIN))
		{
			//MoveTo(4, 13 + 12 * num_lines_printed);
			//drawstring(text_buffer[line_to_print].data());
			DrawString(text_buffer[line_to_print].data(), 4, 2 + 12 * num_lines_printed,hdc);
			num_lines_printed++;
			line_to_print++;
			if (line_to_print== TEXT_BUF_LEN)
			{
				line_to_print= 0;
			}
			
			if ((num_lines_printed == LINES_IN_TEXT_WIN - 1) && (printing_long == TRUE))
			{
				end_loop = FALSE;
				line_to_print= buf_pointer;
			}
	
		}
			
		// Now put text on window.
		SelectObject(hdc,store_bmp);
		DeleteObject(hdc);
	}

	dest_rect = store_text_rect;

	OffsetRect(&dest_rect,TEXT_WIN_UL_X,TEXT_WIN_UL_Y);
 	// Now put text on window.
	rect_draw_some_item_bmp(text_area_gworld, store_text_rect, text_area_gworld, dest_rect, 0, 1);
	string_added = FALSE;	
}

void through_sending()
{
	mark_where_printing_long = buf_pointer + LINES_IN_TEXT_WIN - 1;
	if (mark_where_printing_long > TEXT_BUF_LEN - 1)
	{
		mark_where_printing_long -= TEXT_BUF_LEN;
	}
	printing_long = FALSE;
}

/* Draw a bitmap in the world window. hor in 0 .. 8, vert in 0 .. 8,
	object is ptr. to bitmap to be drawn, and masking is for Copybits. */
void Draw_Some_Item (HBITMAP src_gworld, RECT src_rect, HBITMAP targ_gworld, location target, char masked, short main_win)
{
	RECT	destrec = {0,0,28,36};
	
	if ((target.x < 0) || (target.y < 0) || (target.x > 8) || (target.y > 8))
	{
		return;
	}
	if (src_gworld == NULL)
	{
		return;
	}
	if ((supressing_some_spaces == TRUE) &&
		(not_same_point(target, ok_space[0])) &&
		(not_same_point(target, ok_space[1])) &&
		(not_same_point(target, ok_space[2])) &&
		(not_same_point(target, ok_space[3])))
	{
		return;
	}
	terrain_there[target.x][target.y] = -1;
	
	destrec = coord_to_rect(target.x,target.y);
	rect_draw_some_item_bmp(src_gworld, src_rect, targ_gworld, destrec, masked, main_win);
}

RECT coord_to_rect(short i,short j)
{
	RECT to_return;
	to_return.left = 13 + BITMAP_WIDTH * i;
	to_return.right = to_return.left + BITMAP_WIDTH;
	to_return.top = 13 + BITMAP_HEIGHT * j;
	to_return.bottom = to_return.top + BITMAP_HEIGHT;		
	return to_return;
}


void get_str(char *str,short i, short j)
{
	GetIndString(str, i, j);
}

short string_length(const char *str,HDC hdc)
{
	short text_len[257];
	short total_width = 0,i,len;
	char p_str[256];
	
	for (i = 0; i < 257; i++)
	{
		text_len[i] = 0;
	}
	
	strcpy(p_str,str);
	MeasureText(256,p_str,text_len,hdc);
	len = strlen(str);

	//print_nums(text_len[1],text_len[2],text_len[3]);
	//print_nums(text_len[10],text_len[20],text_len[30]);
	for (i = 0; i < 257; i++)
	{
		if ((text_len[i] > total_width) && (i <= len))
		{
			total_width = text_len[i];
		}
	}
	return total_width;
}


// mode: 0 - align up and left, 1 - center on one line
// str is a c string, 256 characters
// uses current font
void win_draw_string(HDC dest_hdc,RECT dest_rect, std::string_view str,short mode,short line_height)
{
	std::string adjusted;
	adjusted.reserve(str.size());

	// this will need formatting for '|' line breaks
	for (const auto c: str)
	{
		if (c == '|')
		{
			adjusted.push_back(13);
        }
		else if (c == '_')
		{
			adjusted.push_back(34);
		}
		else
		{
			adjusted.push_back(c);
		}
	}

	// if dest is main window, add ulx, uly
	if (dest_hdc == main_dc)
	{
		OffsetRect(&dest_rect, ulx, uly);
	}

	switch (mode)
	{
	case 0:
        dest_rect.bottom += 6;
		DrawText(dest_hdc, adjusted.c_str(), adjusted.size(), &dest_rect, DT_LEFT | DT_NOPREFIX | DT_WORDBREAK);
		break;
	case 1:
		dest_rect.bottom += 6; dest_rect.top -= 6;
		DrawText(dest_hdc, adjusted.c_str(), adjusted.size(), &dest_rect, DT_CENTER | DT_NOPREFIX | DT_VCENTER | DT_NOCLIP | DT_SINGLELINE);
		break;
	case 2:
	case 3:
		dest_rect.bottom += 6; dest_rect.top -= 6;
		DrawText(dest_hdc, adjusted.c_str(), adjusted.size(), &dest_rect, DT_LEFT | DT_NOPREFIX | DT_VCENTER | DT_NOCLIP | DT_SINGLELINE);
		break;
	}
	// not yet done adjusts for 1, 2, 3
}

short calc_day()
{
	return (short) ((party.age) / 3700) + 1;
}

Boolean day_reached(unsigned char which_day, unsigned char which_event)
// which_day is day event should happen
// which_event is the party.key_times value to cross reference with. 
// if the key_time is reached before which_day, event won't happen
// if it's 8, event always happens
// which_day gets an extra 20 days to give party bonus time
{
	short what_day;
	
	what_day = (short) (which_day) + 20;
	
	if ((which_event != 8) && (party.key_times[which_event] < what_day))
	{
		return FALSE;
	}

	if (calc_day() >= what_day)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

// BEGIN EXTRA WINDOWS STUFF

void WinDrawString(std::string_view str,short x,short y)
{
	HDC hdc;
	COLORREF colors[2] = {RGB(0,0,0),RGB(255,255,255)};
	UINT c[2];

	c[0] = GetNearestPaletteIndex(hpal,colors[0]);
	c[1] = GetNearestPaletteIndex(hpal,colors[1]);

	hdc = GetDC(mainPtr);
	SelectPalette(hdc,hpal,0);
 	SetViewportOrgEx(hdc,ulx,uly,nullptr);
	SelectObject(hdc,small_bold_font);
	SetBkMode(hdc,TRANSPARENT);
	SetTextColor(hdc,PALETTEINDEX(c[1]));
	DrawString(str.data(), x, y, hdc);
	fry_dc(mainPtr,hdc);
}

void WinBlackDrawString(std::string_view str,short x,short y)
{
	HDC hdc = GetDC(mainPtr);
	SelectPalette(hdc,hpal,0);
 	SetViewportOrgEx(hdc,ulx,uly,nullptr);
	SelectObject(hdc,small_bold_font);
	SetBkMode(hdc,TRANSPARENT);
	DrawString(str.data(), x, y, hdc);
	fry_dc(mainPtr,hdc);
}


void FlushEvents(short mode)
// mode... 0 - keystrokes   1 - mouse presses  2 - both
{
	MSG msg;

	if ((mode == 0) || (mode == 2))
	{
		while ((PeekMessage(&msg, mainPtr, WM_KEYFIRST, WM_KEYLAST, PM_REMOVE)) != 0)
		{
		}
	}
	if ((mode == 1) || (mode == 2))
	{
		while ((PeekMessage(&msg, mainPtr, WM_MOUSEFIRST, WM_MOUSELAST, PM_REMOVE)) != 0)
		{
		}
	}
}

void ExitToShell()
{
	PostQuitMessage(0);
}

void ClipRect(RECT *rect)
{
	HRGN rgn;
	RECT rect2;
	rect2 = *rect;
	OffsetRect(&rect2,ulx,uly);
	rgn = CreateRectRgn(rect2.left,rect2.top,rect2.right,rect2.bottom);
	SelectClipRgn(main_dc,rgn);
	DeleteObject(rgn);
}

void beep()
{
	MessageBeep(MB_OK);
	Delay(30);
}

void SysBeep(short a)
{
	MessageBeep(MB_OK);
}

void GetIndString(char *str,short i, short j) {
	UINT resnum = 0,len;
	short k;

	resnum = i * 300 + j;

	len = LoadString(store_hInstance,resnum,str,256);
	if (len == 0)
	{
		str[0] = '\0';
		return;
	}
	for (k = 0; k < 256; k++)
	{
		if (str[k] == '|')
		{
			str[k] = 13;
		}
		if (str[k] == '_')
		{
			str[k] = 34;
		}
	}
}

void InsetRect(RECT *rect,short x, short y)
{
	InflateRect(rect,-1 * x, -1 * y);
}

void force_reprint()
{
	string_added = TRUE;
}

// Adapted from Wine source: https://github.com/reactos/wine/blob/master/dlls/gdi.exe16/gdi.c
static DWORD GetTextExtent16(HDC hdc, LPCSTR str, INT16 count)
{
	SIZE size;
	if (!GetTextExtentPoint32A(hdc, str, count, &size))
	{
		assert(false);
		return 0;
	}
	return MAKELONG(size.cx, size.cy);
}

// Note ... this expects a str len of at most 256 and
// len_array pointing to a 256 long array of shorts
void MeasureText(short str_len,char *str, short *len_array,HDC hdc)
{
	short text_len[257];
	short i;
	char p_str[257];
	DWORD val_returned;
	char *store_array;
	short *store2;

	store_array = (char *) len_array;
	//text_len = len_array;
	for (i = 0; i < 256; i++)
	{
		text_len[i] = 0;
	}
	for (i = 1; i < str_len; i++)
	{
		strncpy(p_str,str,i);
		p_str[i] = 0;
		val_returned = GetTextExtent16(hdc,p_str,i);
		text_len[i] = LOWORD(val_returned);
	}
	for (i = 0; i < 256; i++)
	{
		store2 = (short *) store_array;
		*store2 = text_len[i];
		store_array += 2;
	}
}

// kludgy annoyance
void MoveTo(short x, short y)
{
	store_text_x = x;
	store_text_y = y - 16;
}

void MoveToDrawString(const char *string,HDC hdc)
{
	DrawString(string,store_text_x,store_text_y, hdc);
}
