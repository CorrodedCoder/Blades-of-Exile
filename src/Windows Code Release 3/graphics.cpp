#include <Windows.h>
#include <array>
#include <cstring>
#include <cstdio>
#include "global.h"
#include "graphics.h"
#include "newgraph.h"
#include "gutils.h"           
#include "monster.h"                        
#include "dlogtool.h"
#include "locutils.h"
#include "fields.h"
#include "text.h"
#include "exlsound.h"
#include "graphutl.h"
#include "graphutl_helpers.hpp"

extern HWND	mainPtr;
extern RECT	windRect;
extern short stat_window,give_delays,overall_mode;
extern short current_spell_range,town_type,store_anim_type;
extern Boolean in_startup_mode,anim_onscreen,play_sounds,frills_on,startup_loaded,party_in_memory;
extern short town_size[3];
extern short anim_step;
extern party_record_type far party;
extern piles_of_stuff_dumping_type data_store;
extern talking_record_type far talking;
extern scenario_data_type far scenario;

extern pc_record_type far adven[6];
extern big_tr_type far  t_d;
extern outdoor_record_type far outdoors[2][2];
extern current_town_type far c_town;
extern town_item_list far  t_i;
extern unsigned char far out[96][96];
extern unsigned char far out_e[96][96];
extern unsigned char far combat_terrain[64][64];
extern effect_pat_type current_pat;
extern Boolean web,crate,barrel,fire_barrier,force_barrier,quickfire,force_wall,fire_wall,antimagic,scloud,ice_wall,blade_wall;
extern short ulx,uly;
extern location pc_pos[6],pc_dir[6],center;
extern short which_combat_type,current_pc;
extern Boolean monsters_going,boom_anim_active,cartoon_happening;
extern short current_ground;
extern short terrain_pic[256];
extern short pc_moves[6];
extern short num_targets_left;
extern location spell_targets[8];
extern short display_mode;
extern HWND text_sbar,item_sbar,shop_sbar;
extern RECT sbar_rect,item_sbar_rect,shop_sbar_rect;
extern RECT talk_area_rect, word_place_rect,startup_top;
extern HBRUSH map_brush[25];
extern HBITMAP map_bitmap[25];
extern POINT store_anim_ul;
extern long register_flag,last_redraw_time;
extern long ed_flag,ed_key;
extern Boolean registered,ed_reg;
extern Boolean fast_bang;

extern HPALETTE hpal;
extern far PALETTEENTRY ape[256];
extern HDC main_dc,main_dc2,main_dc3;

extern HFONT fantasy_font,font,small_bold_font,italic_font,underline_font,bold_font;
extern HCURSOR arrow_curs[3][3], sword_curs, key_curs, target_curs,talk_curs,look_curs;

extern HINSTANCE store_hInstance;
extern Boolean modeless_exists[18],diff_depth_ok;
extern short modeless_key[18];
extern HWND modeless_dialogs[18];
extern piles_of_stuff_dumping_type4 data_store4;

extern unsigned char m_pic_index[200];

extern HBRUSH bg[14];
HBITMAP bg_bitmap[14];
HBRUSH checker_brush = NULL;
HBITMAP checker_bitmap = NULL,bw_bitmap;
HBITMAP startup_button_orig,startup_button_g,anim_mess ;

Boolean done_fancy_startup_once = FALSE;

extern Boolean fry_startup;

//HRGN clip_region;

//PaletteHandle new_palette,old_palette,wank_palette;

short terrain_there[9][9]; // this is an optimization variabel. Keeps track of what terrain
	// is in the terrain spot, so things don't get redrawn. 
	// 0 - 299 just terrain graphic in place
	// 300 - blackness
	// -1 - nothign worth saving 

extern long anim_ticks;

// 0 - terrain   1 - buttons   2 - pc stats 
// 3 - item stats   4 - text bar   5 - text area (not right)
RECT win_from_rects[6] = {{0,0,279,351},{0,0,258,37},{0,0,288,115},{0,0,288,143},{0,0,279,21},{0,0,288,0}};
RECT win_to_rects[6] = {{5,5,284,356},{5,383,263,420},{0,0,271,116},{0,0,271,144},{5,358,284,379},{0,0,256,138}};

// 0 - title  1 - button  2 - credits  3 - base button
RECT startup_from[4] = {{0,0,602,274},{0,274,301,322},{301,0,579,67},{301,274,341,314}}; ////
extern RECT startup_button[6];
	
	RECT trim_rects[8] = {{0,0,28,5},{0,31,28,36},{0,0,5,36},{24,0,28,36},
						{0,0,5,5},{24,0,28,5},{24,31,28,36},{0,31,5,36}};  /**/

RECT	top_left_rec = {0,0,28,36};
short which_graphic_index[6] = {50,50,50,50,50,50};

char combat_graphics[5] = {28,29,36,79,2};
short debug_nums[6] = {0,0,0,0,0,0};					     
short remember_tiny_text = 300; // Remembers what's in the tiny text-bar, to prevent redrawing.
	// 50 indicates area name, other number indicates which-rect.
	// Add 200 if last mess. was in town

char light_area[13][13];
char unexplored_area[13][13];

// Declare the graphics
HBITMAP mixed_gworld, pc_stats_gworld, item_stats_gworld, text_area_gworld;
HBITMAP storage_gworld,terrain_screen_gworld,text_bar_gworld,orig_text_bar_gworld,buttons_gworld;
HBITMAP party_template_gworld,items_gworld,tiny_obj_gworld,fields_gworld;
extern HBITMAP map_gworld;
HBITMAP dlg_buttons_gworld,missiles_gworld,dlogpics_gworld,small_temp_gworld;

HBITMAP dialog_pattern_gworld,pattern_gworld,status_pattern_gworld;

// Startup graphics, will die when play starts
HBITMAP startup_gworld;
HPALETTE opening_palette;

HRGN clip_region;
HBRUSH gbrush;
HPEN gpen;

// Graphics storage vals
short far which_g_stored[STORED_GRAPHICS];
// 0 - 299 terrain graphic   300 + i  monster graphic i, face right  600 + i face left
// 1000 + 100 * j + i   anim i pos j    2300 + i  combat monster graphic i, face right  2600 + i face left
short far wish_list[STORED_GRAPHICS];
short far storage_status[STORED_GRAPHICS]; // 0 - empty 1 - in use 2 - there, not in use

Boolean has_run_anim = FALSE,currently_loading_graphics = FALSE;
//short anim_step = 0;
//short overall_anim_step = 0;

RECT main_win_rect = {0,0,250,410};
RECT main_win2_source_rect = {0,0,265,410};
RECT main_win2_rect = {250,0,515,410};

RECT tiny_world_1_source_rect = {0,0,145,190},
	tiny_world_1_rect = {242,195,475,385};

RECT share_mess_source_rect = {0,0,120,59},
	share_mess_rect = {384,120,504,179};
RECT start_buttons_source_rect = {0,0,190,186},
	start_buttons_rect = {30,214,220,400}; /**/

// Variables to store trim. Makes game faster, but wastes 15K. We'll see how it works...
char far out_trim[96][96];
char far town_trim[64][64];

// Array to store which spots have been seen. Time-saver for drawing fields
char spot_seen[9][9];

char anim_str[256];
location anim_str_loc;
short startup_anim_pos = 43;

extern short combat_posing_monster , current_working_monster ; // 0-5 PC 100 + x - monster x
Boolean supressing_some_spaces = FALSE;
location ok_space[4] = {{0,0},{0,0},{0,0},{0,0}};
 	char combat_string[100];


static void undo_clip()
{
	//	RECT overall_rect = {0,0,530,435};
	HRGN rgn = CreateRectRgn(0, 0, 5000, 5000);
	SelectClipRgn(main_dc, rgn);
	DeleteObject(rgn);
}

void make_cursor_sword()
{
	SetCursor(sword_curs);
}

void adjust_window_mode()
{
	create_clip_region();
	undo_clip();
	if (overall_mode != 45) {
		if (in_startup_mode == TRUE)
			draw_startup(0);
		if (in_startup_mode == FALSE)
			refresh_screen(0);
		}
	MoveWindow(text_sbar,ulx + 547,uly + 283,15,138,TRUE);
	MoveWindow(item_sbar,ulx + 547,uly + 147,15,107,TRUE);
	MoveWindow(shop_sbar,ulx + 258,uly + 67,16,290,TRUE);
}


void plop_fancy_startup()
{
	HBITMAP pict_to_draw = NULL;
	short i,j;

	RECT from_rect = {0,0,350,350},to_rect,whole_window;
	POINT graphic_ul;
	HGDIOBJ old_brush;
	long cur_time;
	RECT big_pic_from = {2,48,641,434};

	GetClientRect(mainPtr,&whole_window);

	//from_rect = screen_rect;
	//if (whole_window.right > 613)
	//	OffsetRect(&screen_rect,(whole_window.right - 613) / 2 - 4,0);
	//if (whole_window.bottom > 448)
	//	OffsetRect(&screen_rect,0,(whole_window.bottom - 448) / 2 - 15);

	if (done_fancy_startup_once == FALSE) {
		// initialize buffers and rects
		for (i = 0; i < STORED_GRAPHICS; i++) {
			which_g_stored[i] = (i < 50) ? i : 0;
			wish_list[i] = 0;
			storage_status[i] = (i < 50) ? 1 : 0;
			}
		for (i = 0;i < 8; i++)
			OffsetRect(&trim_rects[i],61,37);
		for (i = 0; i < 9; i++)
			for (j = 0; j < 9; j++)
				terrain_there[i][j] = -1;

		OffsetRect(&win_to_rects[5],TEXT_WIN_UL_X,TEXT_WIN_UL_Y);
		OffsetRect(&win_to_rects[2],PC_WIN_UL_X,PC_WIN_UL_Y);
		OffsetRect(&win_to_rects[3],ITEM_WIN_UL_X,ITEM_WIN_UL_Y);

		main_dc = GetDC(mainPtr);

		pict_to_draw = ReadDib("blscened/SPIDLOGO.BMP",main_dc);

		SelectPalette(main_dc,hpal,0);
		SelectObject(main_dc,font);
		SetBkMode(main_dc,TRANSPARENT);
		main_dc2 = CreateCompatibleDC(main_dc);
		SetMapMode(main_dc2,GetMapMode(main_dc));
		SelectPalette(main_dc2,hpal,0);
		main_dc3 = CreateCompatibleDC(main_dc);
		SetMapMode(main_dc3,GetMapMode(main_dc));
		SelectPalette(main_dc3,hpal,0);
		SetStretchBltMode(main_dc,STRETCH_DELETESCANS);
		SetStretchBltMode(main_dc2,STRETCH_DELETESCANS);
		SetStretchBltMode(main_dc3,STRETCH_DELETESCANS);
		}
		else pict_to_draw = ReadDib("blscened/SPIDLOGO.BMP",main_dc);

	cur_time = GetCurrentTime();
	//gray_out_window(0);
	old_brush = SelectObject(main_dc,GetStockObject(BLACK_BRUSH));
	Rectangle(main_dc, whole_window.left,whole_window.top,
		whole_window.right,whole_window.bottom);
	SelectObject(main_dc,old_brush);
	//sx = ulx;
	//sy = uly;

	graphic_ul.x = (whole_window.right - 350) / 2 - ulx;
	graphic_ul.y = (whole_window.bottom - 350) / 2 - uly - 10;
	to_rect = from_rect;
	OffsetRect(&to_rect,graphic_ul.x,graphic_ul.y);
	rect_draw_some_item_bmp(pict_to_draw,from_rect,pict_to_draw,to_rect,0,1);
	play_sound(95);
	//ulx = sx;
	//uly = sy;
	DeleteObject(pict_to_draw);
	if (done_fancy_startup_once == FALSE) {
		done_fancy_startup_once = TRUE;

		Set_up_win();
		init_startup();
		}

	while (GetCurrentTime() - cur_time < 3000) ;

	old_brush = SelectObject(main_dc,GetStockObject(BLACK_BRUSH));
	Rectangle(main_dc, whole_window.left,whole_window.top,
		whole_window.right,whole_window.bottom);
	SelectObject(main_dc,old_brush);
	pict_to_draw = ReadDib("blscened/START.BMP",main_dc);

	cur_time = GetCurrentTime();

	graphic_ul.x = (whole_window.right - 639) / 2 - ulx;
	graphic_ul.y = (whole_window.bottom - 486) / 2 - uly + 46;
	from_rect = big_pic_from;
	to_rect = from_rect;

	OffsetRect(&to_rect,-1 * to_rect.left + graphic_ul.x,-1 * to_rect.top + graphic_ul.y);
	rect_draw_some_item_bmp(pict_to_draw,from_rect,pict_to_draw,to_rect,0,1);
	DeleteObject(pict_to_draw);

	play_sound(-22);
	while (GetCurrentTime() - cur_time < 5000) ;
	old_brush = SelectObject(main_dc,GetStockObject(BLACK_BRUSH));
	Rectangle(main_dc, whole_window.left,whole_window.top,
		whole_window.right,whole_window.bottom);
	SelectObject(main_dc,old_brush);
	inflict_palette();
}

void fancy_startup_delay()
{
	//play_sound(-9);
	 // insert gray out window here

	FlushEvents(2);
	}

void init_startup()
{
	COLORREF x = RGB(204,204,204);
	UINT c;

	startup_loaded = TRUE;

	c = GetNearestPaletteIndex(hpal,x);
	gbrush = CreateSolidBrush(PALETTEINDEX(c));
	gpen = CreatePen(PS_SOLID,1,PALETTEINDEX(c));

	startup_gworld = load_pict(830,main_dc);
	startup_button_orig = load_pict(832,main_dc);
	startup_button_g = load_pict(832,main_dc);
	anim_mess = load_pict(831,main_dc);
}

void draw_startup(short but_type)
{
	RECT to_rect;
	RECT r1 = {-1000,-1000,1000,5},r2 = {-1000,-1000,5,1000},r3 = {-1000,418,1000,2000},
		r4 = {579,-1000,2500,1000};
	short i;

	if (startup_loaded == FALSE)
		return;

	r1.bottom = uly + 5;
	paint_pattern_main(r1,0);
	r2.right = ulx + 5;
	paint_pattern_main(r2,0);
	r3.top += uly + 5;
	paint_pattern_main(r3,0);
	r4.left += ulx + 5;
	paint_pattern_main(r4,0);
	to_rect = startup_from[0];
	OffsetRect(&to_rect,5,5);
	rect_draw_some_item_bmp(startup_gworld,startup_from[0],startup_gworld,to_rect,0,1);

//	to_rect = startup_from[2];
//	OffsetRect(&to_rect,306,(-1 * to_rect.top) + 356);
//	rect_draw_some_item_bmp(startup_gworld,startup_from[2],startup_gworld,to_rect,0,1);

	for (i = 0; i < 5; i++) {
		rect_draw_some_item_bmp(startup_gworld,startup_from[1],startup_gworld,startup_button[i],0,1);
		draw_start_button(i,but_type);
		}
	draw_startup_anim();
	
	draw_startup_stats();
	}

void draw_startup_stats()
{
	RECT from_rect,to_rect,party_from = {0,0,28,36},pc_rect,frame_rect;
	short i;
	char str[256];
	COLORREF colors[5] = {RGB(0,0,0),RGB(255,255,255),RGB(0,204,255),RGB(0,160,0),RGB(0,0,255)};
	UINT c[5];
	HBRUSH hbrush;

	for (i = 0; i < 5; i++)
		c[i] = GetNearestPaletteIndex(hpal,colors[i]);
	to_rect = startup_top;
	OffsetRect(&to_rect, 20, 35);
	hbrush = CreateSolidBrush(PALETTEINDEX(c[2]));

	SetTextColor(main_dc,PALETTEINDEX(c[1]));

	if (party_in_memory == FALSE) {
		//TextSize(20);
		SelectObject(main_dc,fantasy_font);
		OffsetRect(&to_rect,175,40);
		char_win_draw_string(main_dc,to_rect,
			"No Party in Memory",0,18);
		}
	if (party_in_memory == TRUE) {
		frame_rect = startup_top;
		InflateRect(&frame_rect,-50,-50);
		frame_rect.top += 30;
		OffsetRect(&frame_rect,ulx ,uly + 10);
		FrameRect(main_dc,&frame_rect,hbrush);

		SelectObject(main_dc,fantasy_font);
		OffsetRect(&to_rect,213,25);
		char_win_draw_string(main_dc,to_rect,
			"Your party:",0,18);
		SelectObject(main_dc,small_bold_font);
		for (i = 0; i < 6; i++) {
			pc_rect = startup_top;
			pc_rect.right = pc_rect.left + 300;
			pc_rect.bottom = pc_rect.top + 79;
			OffsetRect(&pc_rect,60 + 232 * (i / 3) + 25,95 + 45 * (i % 3));

			if (adven[i].main_status > 0) {
				from_rect = party_from;
				OffsetRect(&from_rect,56 * (i / 3),36 * (i % 3));
				to_rect = party_from;
				OffsetRect(&to_rect,pc_rect.left,pc_rect.top + 2);
				rect_draw_some_item_bmp(party_template_gworld,from_rect,
					party_template_gworld,to_rect,0,1);
				InflateRect(&to_rect,1,1);
            OffsetRect(&to_rect,ulx,uly);
				FrameRect(main_dc,&to_rect,hbrush);

				SelectObject(main_dc,bold_font);
				OffsetRect(&pc_rect,35,0);
				char_win_draw_string(main_dc,pc_rect,
					adven[i].name,0,18);
				OffsetRect(&to_rect,pc_rect.left + 8,pc_rect.top + 8);

				}
			//TextSize(12);
			OffsetRect(&pc_rect,12,16);
				SelectObject(main_dc,small_bold_font);
			switch (adven[i].main_status) {
				case 1:
					switch (adven[i].race) {
						case 0: sprintf(str,"Level %d Human",adven[i].level); break;
						case 1: sprintf(str,"Level %d Nephilim",adven[i].level); break;
						case 2: sprintf(str,"Level %d Slithzerikai",adven[i].level); break;
						}
					char_win_draw_string(main_dc,pc_rect,(char *) str,0,18);
					OffsetRect(&pc_rect,0,13);
					sprintf(str,"Health %d, Spell pts. %d",
						adven[i].max_health,adven[i].max_sp);
					char_win_draw_string(main_dc,pc_rect,(char *) str,0,18);
					break;
				case 2:
					char_win_draw_string(main_dc,pc_rect,"Dead",0,18);
					break;
				case 3:
					char_win_draw_string(main_dc,pc_rect,"Dust",0,18);
					break;
				case 4:
					char_win_draw_string(main_dc,pc_rect,"Stone",0,18);
					break;
				}
			}
		}

	SelectObject(main_dc,small_bold_font);
	pc_rect = startup_from[0];
	OffsetRect(&pc_rect,5,5);
	pc_rect.top = pc_rect.bottom - 28;
	pc_rect.left = pc_rect.right - 275;
	char_win_draw_string(main_dc,pc_rect,"Copyright 1997-8, All Rights Reserved, v1.0.1",0,18);
	if (registered == FALSE) {
		pc_rect.left = startup_from[0].left + 22;
		char_win_draw_string(main_dc,pc_rect,"Unregistered copy. To order, select How To Order.",0,18);
		}

	SetTextColor(main_dc,PALETTEINDEX(c[0]));
	DeleteObject(hbrush);
}

void draw_startup_anim()
{
	RECT anim_to = {1,4,276,44},anim_from;
	RECT anim_size = {0,0,301,48};

	anim_from = anim_to;
	OffsetRect(&anim_from,-1,-4 + startup_anim_pos);
	startup_anim_pos = (startup_anim_pos + 1) % 542;
	rect_draw_some_item_bmp(startup_button_orig,anim_size,startup_button_g,anim_size,0,0);
	rect_draw_some_item_bmp(anim_mess,anim_from,startup_button_g,anim_to,1,0);
	rect_draw_some_item_bmp(startup_button_g,anim_size,startup_button_g,startup_button[5],0,1);
}

static const std::array button_labels{ "Load Game","Make New Party","How To Order",
	"Start Scenario","Custom Scenario","Quit" };

void draw_start_button(short which_position,short which_button)
{
	RECT from_rect,to_rect;
	COLORREF colors[5] = {RGB(0,0,0),RGB(255,0,0),RGB(255,255,255),RGB(0,255,255),RGB(0,0,255)};
	UINT c[5];

	short i;

	for (i = 0; i < 5; i++)
		c[i] = GetNearestPaletteIndex(hpal,colors[i]);

	from_rect = startup_from[3];
	OffsetRect(&from_rect, (which_button > 0) ? 40 : 0,0);
	to_rect = startup_button[which_position];
	to_rect.left += 4; to_rect.top += 4;
	to_rect.right = to_rect.left + 40;
	to_rect.bottom = to_rect.top + 40;
   from_rect.top += 4; to_rect.top += 4;
   rect_draw_some_item_bmp(startup_gworld,from_rect,startup_gworld,to_rect,0,1);

	//TextFont(dungeon_font_num);
	//TextFace(0);
	//TextSize(24);
	to_rect = startup_button[which_position];
	//to_rect.left += 80;
	OffsetRect(&to_rect, 10, 0);
	if (which_button == 5)
		which_button = 4;
	if (which_button == 0)
		SetTextColor(main_dc,PALETTEINDEX(c[2]));
		else SetTextColor(main_dc,PALETTEINDEX(c[2]));
	SelectObject(main_dc,fantasy_font);
	if (which_position == 3)
		OffsetRect(&to_rect,-7,0);
	SetTextColor(main_dc,PALETTEINDEX(c[0]));
	OffsetRect(&to_rect,0,1);
	char_win_draw_string(main_dc,to_rect,
		button_labels[which_position],1,18);
	OffsetRect(&to_rect,0,-2);
	char_win_draw_string(main_dc,to_rect,
		button_labels[which_position],1,18);
	OffsetRect(&to_rect,1,1);
	char_win_draw_string(main_dc,to_rect,
		button_labels[which_position],1,18);
	OffsetRect(&to_rect,-2,0);
	char_win_draw_string(main_dc,to_rect,
		button_labels[which_position],1,18);
	OffsetRect(&to_rect,1,0);
	SetTextColor(main_dc,PALETTEINDEX(c[2]));
	char_win_draw_string(main_dc,to_rect,
		button_labels[which_position],1,18);
	SelectObject(main_dc,font);
	SetTextColor(main_dc,PALETTEINDEX(c[0]));
	//ForeColor(blackColor);
	//TextFont(geneva_font_num);
	//TextFace(bold);
	//TextSize(10);

}

void main_button_click(short mode,RECT button_rect)
{
	long dummy;


	ClipRect(&button_rect);

	draw_buttons(1);
	if (play_sounds == TRUE)
		play_sound(37);
		else Delay(5,&dummy);
	draw_buttons(0);			
	undo_clip();
}

void arrow_button_click(RECT button_rect)
{
	long dummy;

	ClipRect(&button_rect);
	
	refresh_stat_areas(1);
	if (play_sounds == TRUE)
		play_sound(37);
		else Delay(5,&dummy);
	refresh_stat_areas(0);
	undo_clip();
}

void reload_startup()////
{
	if (startup_loaded == TRUE)
		return;
	
	DestroyWindow(modeless_dialogs[5]);
	modeless_exists[5] = FALSE;				
	if (text_sbar != NULL) {
 		ShowScrollBar(text_sbar,SB_CTL,FALSE);
		ShowScrollBar(item_sbar,SB_CTL,FALSE);
		}
		//	SelectWindow(mainPtr);
//	SetPort(mainPtr);
	init_startup();

	startup_loaded = TRUE;
	//load_main_screen();
}

void end_startup()
{
	if (startup_loaded == FALSE)
		return;

	DeleteObject(startup_gworld);
	DeleteObject(startup_button_orig);
	DeleteObject(startup_button_g);
	DeleteObject(anim_mess);

	startup_loaded = FALSE;
	load_main_screen();
}

// This loads the graphics at the top of the game.
void Set_up_win ()
{
	RECT temp_rect = {0,0,280,0};
	RECT map_rect = {0,0,384,384};
	RECT pc_rect = {0,0,113,216};
	HBITMAP temp_gworld;
	short i,j;
	RECT r = {0,0,280,180}; /**/
	RECT bg_from[9] = {{0,168,8,176},{23,206,31,214},{32,168,40,176},{102,174,110,182},	
		{173,191,181,199},{203,190,211,198},{273,183,281,191},{373,0,381,8},
		{380,17,388,25}};
	RECT map_from_orig = {372,0,380,8},map_from,brush_to = {0,0,8,8};
	WORD patbits[] = {0xAA,	0x55, 0xAA,	0x55, 0xAA, 0x55, 0xAA,	0x55};

	RECT pat_from = {96,168,160,232}, pat_to_orig = {0,0,64,64},pat_to;

	checker_bitmap = CreateBitmap(8,8,1,1,(LPSTR) patbits);
	checker_brush = CreatePatternBrush(checker_bitmap);
	bw_bitmap = CreateBitmap(72,72,1,1,NULL);

	temp_rect.bottom = (STORED_GRAPHICS / 10) * 36;
	storage_gworld = CreateCompatibleBitmap(main_dc,temp_rect.right,temp_rect.bottom);
	
	temp_gworld = load_pict(800,main_dc);
	rect_draw_some_item_bmp(temp_gworld,r,storage_gworld,r,0,0);
	DeleteObject(temp_gworld);
	
	terrain_screen_gworld = load_pict(705,main_dc);

	party_template_gworld = CreateCompatibleBitmap(main_dc,pc_rect.right,pc_rect.bottom);
	small_temp_gworld = CreateCompatibleBitmap(main_dc,28,36);

	items_gworld = load_pict(901,main_dc);
	tiny_obj_gworld = load_pict(900,main_dc);
	fields_gworld = load_pict(821,main_dc);
	missiles_gworld = load_pict(880,main_dc);
	dlogpics_gworld = load_pict(850,main_dc);
	mixed_gworld = load_pict(903,main_dc);

	dialog_pattern_gworld = CreateCompatibleBitmap(main_dc,192,256);
	pattern_gworld = CreateCompatibleBitmap(main_dc,192,256);
	status_pattern_gworld = CreateCompatibleBitmap(main_dc,256,128);
	for (i = 0; i < 4; i++)
		for (j = 0; j < 2; j++) {
			pat_to = pat_to_orig;
			OffsetRect(&pat_to,64 * i, 64 * j);
			rect_draw_some_item_bmp(mixed_gworld,pat_from,status_pattern_gworld,
				pat_to,0,0);
			}


	dlg_buttons_gworld = load_pict(2000,main_dc);

	map_gworld = CreateCompatibleBitmap(main_dc,map_rect.right,map_rect.bottom);

	// Create and initialize map gworld
	//err = NewGWorld(&map_gworld, 0,&map_rect, NIL, NIL, 0);
		
	for (i = 0; i < 9; i++) {
		bg_bitmap[i] = CreateCompatibleBitmap(main_dc,8,8);
		rect_draw_some_item_bmp(mixed_gworld,bg_from[i],bg_bitmap[i],brush_to,0,0);
		bg[i] = CreatePatternBrush(bg_bitmap[i]);
		}
	for (i = 0; i < 25; i++) {
		map_from = map_from_orig;
		OffsetRect(&map_from,8 * (i / 10),8 * (i % 10));
		map_bitmap[i] = CreateCompatibleBitmap(main_dc,8,8);
		rect_draw_some_item_bmp(mixed_gworld,map_from,map_bitmap[i],brush_to,0,0);
		map_brush[i] = CreatePatternBrush(map_bitmap[i]);
		}
		/*
		GetPort(&old_port);
		SetPort((GrafPtr) map_gworld);
		map_world_rect = map_gworld->portRect;
		ForeColor(whiteColor);
		PaintRect(&map_world_rect);
		ForeColor(blackColor);
		SetPort(old_port);
		*/
		
}

void lose_graphics() {
	short i,j;

	if (startup_loaded == TRUE) {
		DeleteObject(startup_gworld);
		}
		else {
			DeleteObject(pc_stats_gworld);
			DeleteObject(item_stats_gworld);
			DeleteObject(text_area_gworld);
			DeleteObject(text_bar_gworld);
			DeleteObject(orig_text_bar_gworld);
			DeleteObject(buttons_gworld);
			}
	DeleteObject(bw_bitmap);
	DeleteObject(dialog_pattern_gworld);
	DeleteObject(pattern_gworld);
	DeleteObject(status_pattern_gworld);

	DeleteObject(checker_bitmap);
	DeleteObject(checker_brush);
	DeleteObject(storage_gworld);
	DeleteObject(terrain_screen_gworld);
	DeleteObject(party_template_gworld);
	DeleteObject(items_gworld);
	DeleteObject(tiny_obj_gworld);
	DeleteObject(fields_gworld);
	DeleteObject(missiles_gworld);
	DeleteObject(dlogpics_gworld);
	DeleteObject(mixed_gworld);
	DeleteObject(dlg_buttons_gworld);
	DeleteObject(map_gworld);
	DeleteObject(small_temp_gworld);

	reset_palette();
	DeleteObject(hpal);
   DeleteObject(opening_palette);
	DeleteDC(main_dc);
	DeleteDC(main_dc2);
	DeleteDC(main_dc3);
	DeleteObject(font);
   DeleteObject(fantasy_font);
//	DeleteObject(small_bold_font);
	DeleteObject(underline_font);
	DeleteObject(italic_font);
	DeleteObject(bold_font);
	for (i = 0; i < 9; i++) {
		DeleteObject(bg_bitmap[i]);
		DeleteObject(bg[i]);
		}
	for (i = 0; i < 25; i++) {
		DeleteObject(map_bitmap[i]);
		DeleteObject(map_brush[i]);
		}
	for (i = 0; i < 3; i++)
		for (j = 0; j < 3; j++)
			DeleteObject(arrow_curs[i][j]);
	DeleteObject(talk_curs);
	DeleteObject(sword_curs);
	DeleteObject( key_curs);
	DeleteObject( target_curs);
	DeleteObject( look_curs);
	DeleteObject(gbrush);
	DeleteObject(gpen);

   kill_sound();
	}


void load_main_screen()
{
	
//	tiny_map_graphics = load_pict(904);
	HBITMAP temp_gworld;
	RECT from_rect = {0,0,271,116},to_rect = {0,0,0,0};
	
	// load in graphic with 3 right side status areas, and copy each of them into
	// its own private HBITMAP
	temp_gworld = load_pict(700,main_dc);

	pc_stats_gworld = CreateCompatibleBitmap(main_dc,271,116);
	to_rect.right = 271; to_rect.bottom = 116;
	rect_draw_some_item_bmp(temp_gworld,from_rect,pc_stats_gworld,to_rect,0,0);
	
	item_stats_gworld = CreateCompatibleBitmap(main_dc,271,144);
	to_rect.bottom = 144;
	from_rect.top = 116; from_rect.bottom = 260;
	rect_draw_some_item_bmp(temp_gworld,from_rect,item_stats_gworld,to_rect,0,0);

	text_area_gworld = CreateCompatibleBitmap(main_dc,256,138);
	to_rect.right = 256; to_rect.bottom = 138;
	from_rect.top = 260; from_rect.bottom = 398; from_rect.right = 256;
	rect_draw_some_item_bmp(temp_gworld,from_rect,text_area_gworld,to_rect,0,0);

	DeleteObject(temp_gworld);

	text_bar_gworld = load_pict(703,main_dc);
	orig_text_bar_gworld = load_pict(703,main_dc);
	buttons_gworld = load_pict(704,main_dc);

}

void create_clip_region()
{
	short i;
	RECT store_rect = {0,0,2000,2000};
	RECT scrollbar_rect;
	HRGN temp_rgn;

	DeleteObject(clip_region);
	GetWindowRect(GetDesktopWindow(),&store_rect);
	clip_region = CreateRectRgn(0,0,store_rect.right * 2,store_rect.bottom * 2);


	for (i = 0; i < 6; i++) {
		store_rect = win_to_rects[i];
		OffsetRect(&store_rect,ulx,uly);
		if ((is_out()) || (is_town()) || (is_combat()) ||
			(i == 2) || (i == 3) || (i == 5)) {
				temp_rgn = CreateRectRgn(store_rect.left,store_rect.top,store_rect.right,store_rect.bottom);
				CombineRgn(clip_region,clip_region,temp_rgn,RGN_DIFF);
				DeleteObject(temp_rgn);
				}
		}
	if ((overall_mode == 20) || (overall_mode == 21) ){
		scrollbar_rect = talk_area_rect;
		OffsetRect(&scrollbar_rect,ulx,uly);
		temp_rgn = CreateRectRgn(scrollbar_rect.left,scrollbar_rect.top,scrollbar_rect.right,scrollbar_rect.bottom);
		CombineRgn(clip_region,clip_region,temp_rgn,RGN_DIFF);
		DeleteObject(temp_rgn);
		}
	scrollbar_rect = sbar_rect;
	OffsetRect(&scrollbar_rect,ulx,uly);
	temp_rgn = CreateRectRgn(scrollbar_rect.left,scrollbar_rect.top,scrollbar_rect.right,scrollbar_rect.bottom);
	CombineRgn(clip_region,clip_region,temp_rgn,RGN_DIFF);
	DeleteObject(temp_rgn);
	scrollbar_rect = item_sbar_rect;
	OffsetRect(&scrollbar_rect,ulx,uly);
	temp_rgn = CreateRectRgn(scrollbar_rect.left,scrollbar_rect.top,scrollbar_rect.right,scrollbar_rect.bottom);
	CombineRgn(clip_region,clip_region,temp_rgn,RGN_DIFF);
	DeleteObject(temp_rgn);

}

// redraw_screen does the very first redraw, and any full redraw
void redraw_screen(short mode)
{
	if (text_sbar == NULL) {
		text_sbar = CreateWindow("scrollbar",NULL,
			WS_CHILD | WS_TABSTOP | SBS_VERT | WS_VISIBLE, sbar_rect.left,sbar_rect.top,sbar_rect.right,sbar_rect.bottom,
			mainPtr, reinterpret_cast<HMENU>(1),store_hInstance,NULL);
		SetScrollRange(text_sbar,SB_CTL,0,58,FALSE);
		item_sbar = CreateWindow("scrollbar",NULL,
			WS_CHILD | WS_TABSTOP | SBS_VERT | WS_VISIBLE, item_sbar_rect.left,item_sbar_rect.top,item_sbar_rect.right,item_sbar_rect.bottom,
			mainPtr, reinterpret_cast<HMENU>(2),store_hInstance,NULL);
		SetScrollRange(item_sbar,SB_CTL,0,16,FALSE);
 		ShowScrollBar(text_sbar,SB_CTL,TRUE);
		ShowScrollBar(item_sbar,SB_CTL,TRUE);
	MoveWindow(text_sbar,ulx + 547,uly + 283,15,138,TRUE);
	MoveWindow(item_sbar,ulx + 547,uly + 147,15,107,TRUE);
		}
		else {
			ShowScrollBar(text_sbar,SB_CTL,TRUE);
			ShowScrollBar(item_sbar,SB_CTL,TRUE);
			}
	//if (overall_mode == 21) {
	 //	ShowScrollBar(shop_sbar,SB_CTL,TRUE);
	 //	}
	 //	else ShowScrollBar(shop_sbar,SB_CTL,FALSE);
	switch (overall_mode) {
		case 20: case 21:
		put_background();
			break;
		default:
			//draw_main_screen();
			 draw_terrain(0);

			draw_buttons(0);
			draw_text_bar(1);
			if ((overall_mode == 10) && (current_pc < 6))
				draw_pcs(pc_pos[current_pc],1);
			if (overall_mode == 14)
			draw_targets(center);

			break;
		}

	put_pc_screen();

	if (current_pc < 6)
		put_item_screen(current_pc,0);

		print_buf();


	}

void draw_main_screen()
{
	if (overall_mode == 20) {
		put_background();
		}
		else {
			draw_buttons(0);
			rect_draw_some_item_bmp(terrain_screen_gworld, win_from_rects[0], terrain_screen_gworld, win_to_rects[0], 0, 1);


			if ((current_pc < 6) && (overall_mode == 10))
				draw_pcs(pc_pos[current_pc],1);
			if (overall_mode == 14)
				draw_targets(center);
			}
	draw_text_area(0);
	//ShowScrollBar(text_sbar,SB_CTL,TRUE);
	//ShowScrollBar(item_sbar,SB_CTL,TRUE);
	if (text_sbar == NULL) {
		text_sbar = CreateWindow("scrollbar",NULL,
			WS_CHILD | WS_TABSTOP | SBS_VERT | WS_VISIBLE, sbar_rect.left,sbar_rect.top,sbar_rect.right,sbar_rect.bottom,
			mainPtr, reinterpret_cast<HMENU>(1),store_hInstance,NULL);
		SetScrollRange(text_sbar,SB_CTL,0,58,FALSE);
		item_sbar = CreateWindow("scrollbar",NULL,
			WS_CHILD | WS_TABSTOP | SBS_VERT | WS_VISIBLE, item_sbar_rect.left,item_sbar_rect.top,item_sbar_rect.right,item_sbar_rect.bottom,
			mainPtr, reinterpret_cast<HMENU>(2),store_hInstance,NULL);
		SetScrollRange(item_sbar,SB_CTL,0,16,FALSE);
		ShowScrollBar(text_sbar,SB_CTL,TRUE);
		ShowScrollBar(item_sbar,SB_CTL,TRUE);
	MoveWindow(text_sbar,ulx + 547,uly + 283,15,138,TRUE);
	MoveWindow(item_sbar,ulx + 547,uly + 147,15,107,TRUE);
		}
	//if (overall_mode == 21) {
	//	ShowScrollBar(shop_sbar,SB_CTL,TRUE);
	//	}
	//	else ShowScrollBar(shop_sbar,SB_CTL,FALSE);

}

void refresh_screen(short mode)
{
	if (overall_mode == 20) {
		put_background();
		refresh_talking();
		}
	else if (overall_mode == 21) {
		put_background();
		refresh_shopping();
		}
		else {
		draw_buttons(0);
		redraw_terrain();
		if ((current_pc < 6) && (overall_mode == 10))
			draw_pcs(pc_pos[current_pc],1);
		if (overall_mode == 14)
			draw_targets(center);
		draw_text_bar(1);
		}

	draw_text_area(0);
	//ShowScrollBar(text_sbar,SB_CTL,TRUE);
	//ShowScrollBar(item_sbar,SB_CTL,TRUE);
	//if (overall_mode == 21) {
	//	ShowScrollBar(shop_sbar,SB_CTL,TRUE);
	//	}
	//	else ShowScrollBar(shop_sbar,SB_CTL,FALSE);

}

void put_background()
{
	short wp;
	RECT r;

	if (is_out()) {
		if (party.outdoor_corner.x >= 7)
			wp = 0;
			else wp = 7;
		}
	else if (is_combat()) {
		if (party.outdoor_corner.x >= 7)
			wp = 9;
			else wp = 6;
		}
	else {
		if (party.outdoor_corner.x >= 7)
			wp = 5;
			else wp = 8;
		}

	SelectClipRgn(main_dc,clip_region);
	GetClientRect(mainPtr,&r);
	paint_pattern_wnd(mainPtr,r,wp);
	undo_clip();

	//ShowScrollBar(text_sbar,SB_CTL,TRUE);
	//ShowScrollBar(item_sbar,SB_CTL,TRUE);

}

void flood_bg()
{
	short wp;
	RECT r;

	if (is_out()) {
		if (party.outdoor_corner.x >= 7)
			wp = 0;
			else wp = 7;
		}
	else if (is_combat()) {
		if (party.outdoor_corner.x >= 7)
			wp = 9;
			else wp = 6;
		}
	else {
		if ((party.outdoor_corner.x >= 7) && (c_town.town_num != 21))
			wp = 5;
			else wp = 4;
		}

	GetWindowRect(mainPtr,&r);
	paint_pattern_main(r,wp);
}

void draw_buttons(short mode)
//mode; // 0 - regular   1 - button action
{
	RECT	source_rect = {0,0,258,37}, dest_rec; /**/
	HBITMAP	buttons_to_draw;
	Boolean spec_draw = FALSE;

	if (mode == 1) {
		spec_draw = TRUE;
		mode -= 100;
		}

	buttons_to_draw = buttons_gworld;

	source_rect = win_to_rects[1];
	OffsetRect(&source_rect,-1 * source_rect.left, -1 * source_rect.top);
	if (is_combat()) {
		source_rect.top += 37;
		source_rect.bottom += 37;
		}
	if (is_town()) {
		source_rect.top += 74;
		source_rect.bottom += 74;
		}

	dest_rec = win_to_rects[1];
	rect_draw_some_item_bmp(buttons_gworld,source_rect,buttons_gworld,dest_rec,(spec_draw == TRUE) ? 2 : 0,1);

	put_background();

}

// In general, refreshes any area that has text in it, the stat areas, the text bar
void draw_text_area(short mode)
//short mode unused
{
	refresh_stat_areas(0);

	draw_text_bar(0);
}

void reset_text_bar()
{
remember_tiny_text = 300;
}


void draw_text_bar(short mode)
//short mode; // 0 - no redraw  1 - forced
{
	short i,num_rect[3] = {12,10,4};
	location loc;

	loc = (is_out()) ? global_to_local(party.p_loc) : c_town.p_loc;

	if (mode == 1)
		remember_tiny_text = 500;
	if ((PSD[305][0] > 0) ||  (PSD[305][1] > 0) ||(PSD[305][2] > 0) ||(PSD[305][3] > 0) )
		remember_tiny_text = 500;
	if (is_out()) {
		for (i = 0; i < 8; i++)
			if (pt_in_rect(loc,outdoors[party.i_w_c.x][party.i_w_c.y].info_rect[i])) 
				if ((remember_tiny_text == i) && (mode == 0))
					return;
					else {
						put_text_bar(data_store4.outdoor_text[party.i_w_c.x][party.i_w_c.y].out_strs[i + 1]);
						remember_tiny_text = i;
						return;
						}
		if (remember_tiny_text != 50 + party.i_w_c.x + party.i_w_c.y) {
			put_text_bar((char *) data_store4.outdoor_text[party.i_w_c.x][party.i_w_c.y].out_strs[0]);
			remember_tiny_text = 50 + party.i_w_c.x + party.i_w_c.y;
			}
		}
	if (is_town()) {
		for (i = 0; i < num_rect[town_type]; i++)
			if (pt_in_rect(loc,t_d.room_rect[i])) 
				if ((remember_tiny_text == 200 + i) && (mode == 0))
					return;
					else {
						put_text_bar(data_store.town_strs[i + 1]);
						remember_tiny_text = 200 + i;
						return;
						}
		if (remember_tiny_text != 250) {
			put_text_bar((char *) data_store.town_strs[0]); ////
			remember_tiny_text = 250;
			}
	
		}
  	if ((is_combat()) && (current_pc < 6) && (monsters_going == FALSE)) {
		sprintf(combat_string,"%s (ap: %d)",
			adven[current_pc].name,pc_moves[current_pc]);
		put_text_bar((char *) combat_string);
		remember_tiny_text = 500;
		}
	if ((is_combat()) && (monsters_going == TRUE))	// Print bar for 1st monster with >0 ap -
		// that is monster that is going
		for (i = 0; i < T_M; i++)
			if ((c_town.monst.dudes[i].active > 0) && (c_town.monst.dudes[i].m_d.ap > 0)) {
				print_monster_going((char *) combat_string,c_town.monst.dudes[i].number,c_town.monst.dudes[i].m_d.ap);
			put_text_bar((char *) combat_string);
			remember_tiny_text = 500;
			i = 400;
		}
}

void put_text_bar(char *str)
{
	char status_str[256];
	short xpos = 205;
	HDC hdc;
	HGDIOBJ store_bmp;
	RECT text_rect = {5,3,279,21};
	COLORREF x = RGB(0,0,0),y = RGB(255,255,255);
	UINT c;

	rect_draw_some_item_bmp(orig_text_bar_gworld, win_from_rects[4], text_bar_gworld, win_from_rects[4], 0, 0);

	hdc = CreateCompatibleDC(main_dc);
	SelectPalette(hdc,hpal,0);
	SetBkMode(hdc,TRANSPARENT);
	SelectObject(hdc,small_bold_font);
	store_bmp = SelectObject(hdc,text_bar_gworld);
	sprintf(status_str,"%s",str);
	c = GetNearestPaletteIndex(hpal,y);
	SetTextColor(hdc,PALETTEINDEX(c));
	win_draw_string(hdc,text_rect,status_str,2,9);
	c = GetNearestPaletteIndex(hpal,x);
	SetTextColor(hdc,PALETTEINDEX(c));

	if (monsters_going == FALSE) {
		if (PSD[305][0] > 0) {
			text_rect.left = xpos;
			sprintf(status_str,"Stealth");
			win_draw_string(hdc,text_rect,status_str,2,9);
			xpos -= 60;
			}
		if (PSD[305][1] > 0) {
			text_rect.left = xpos;
			sprintf(status_str,"Flying");
			win_draw_string(hdc,text_rect,status_str,2,9);
			xpos -= 60;
			}
		if (PSD[305][2] > 0) {
			text_rect.left = xpos;
			sprintf(status_str,"Detect Life");
			win_draw_string(hdc,text_rect,status_str,2,9);
			xpos -= 60;
			}
		if (PSD[305][3] > 0) {
			text_rect.left = xpos;
			sprintf(status_str,"Firewalk");
			win_draw_string(hdc,text_rect,status_str,2,9);
			xpos -= 60;
			}
		}

	SelectObject(hdc,store_bmp);
	DeleteObject(hdc);
	rect_draw_some_item_bmp(text_bar_gworld, win_from_rects[4], text_bar_gworld, win_to_rects[4], 0, 1);
}

// This is called when a new situation is entered. It figures out what graphics are needed,
// sets up which_g_stored, and loads them.
void load_area_graphics()
{
	short i;
	
	currently_loading_graphics = TRUE;
	
	// Set all graphics as loseable
	for (i = 50; i < STORED_GRAPHICS; i++)
		if (storage_status[i] == 1)
			storage_status[i] = 2;
	for (i = 0; i < STORED_GRAPHICS; i++)
		wish_list[i] = 0;
		
	// Build wish list
	if (is_out())
		load_outdoor_graphics();
	if ((is_town()) || (is_combat()))
		load_town_graphics();
	
	// Reserve all in wish list not taken
	for (i = 0; i < STORED_GRAPHICS; i++)
		if (wish_list[i] > 49)
			if (reserve_graphic_num_in_array(wish_list[i]) == TRUE)
				wish_list[i] = 0;
				
	// Place all graphics not found in array.
	for (i = 0; i < STORED_GRAPHICS; i++)
		if (wish_list[i] > 49) {
			place_graphic_num_in_array(wish_list[i]);
			wish_list[i] = 0;
			}
			
	// Finally, load graphics.
	put_graphics_in_template();
	
	currently_loading_graphics = FALSE;
}

void add_to_wish_list(short which_g)
{
	short i;

	
	if (which_g < 50)
		return;
	for (i = 0; i < STORED_GRAPHICS; i++) {
		if (wish_list[i] == which_g)
			return;
		if (wish_list[i] == 0) {
			wish_list[i] = which_g;
			return;
			}
		}
	add_string_to_buf("No room for graphic.");
}

// Used to set up array. If graphic there, sets it to be saved, otherwise leaves.
// Returns TRUE is already there
Boolean reserve_graphic_num_in_array(short which_g)
{
	short i;

	if (which_g < 50)
		return TRUE;
	for (i = 50; i < STORED_GRAPHICS; i++)
		if (which_g_stored[i] == which_g) {
			storage_status[i] = 1;
			return TRUE;
			}
	return FALSE;
}

// Otherwise, puts in array. Note ... if graphic is alreayd here and locked (i.e. 
// storage status is 1, this will add a new copy.
void place_graphic_num_in_array(short which_g)
{
	short i;

	for (i = 50; i < STORED_GRAPHICS; i++)
		if ((storage_status[i] == 2) || (storage_status[i] == 0)) {
			which_g_stored[i] = which_g;
			storage_status[i] = 3;
			return;
			}
	// if we get here, couldn't find a space. Time to flush excess crap.
	
	if (currently_loading_graphics == FALSE)
		load_area_graphics(); // calling this is nice and fast, because game won't try to reload 
			// graphics already there. It'll only purge the trash.
	
	// try again
	for (i = 50; i < STORED_GRAPHICS; i++)
		if ((storage_status[i] == 2) || (storage_status[i] == 0)) {
			which_g_stored[i] = which_g;
			storage_status[i] = 3;
			return;
			}
	add_string_to_buf("No room for graphic.");
	print_nums(0,0,which_g);
}

void add_one_graphic(short which_g)
{
	short i;
	for (i = 0; i < STORED_GRAPHICS; i++) 
		if (which_g_stored[i] == which_g) {
			// Good. We got it. Now lock it and leave
			storage_status[i] = 1;
			return;	
			}
			
	// otherwise, load it in
	place_graphic_num_in_array(which_g);
	put_graphics_in_template();
}


void add_terrain_to_wish_list(unsigned char ter)
{
	if (terrain_pic[ter] >= 1000) 
		return;
	else if (terrain_pic[ter] >= 400) {
		add_to_wish_list(600 + terrain_pic[ter]);
		add_to_wish_list(700 + terrain_pic[ter]);
		add_to_wish_list(800 + terrain_pic[ter]);
		add_to_wish_list(900 + terrain_pic[ter]);
		}
		else switch (terrain_pic[ter]) {

			case 143:
				add_to_wish_list(230);			
				add_to_wish_list(143);			
				break;
			case 213: case 214:
				add_to_wish_list(213);			
				add_to_wish_list(214);						
				break;

			case 215:
				add_to_wish_list(215);
				add_to_wish_list(218);
				add_to_wish_list(219);
				add_to_wish_list(220);
				add_to_wish_list(221);
				break;
			case 216:
				add_to_wish_list(216);
				add_to_wish_list(222);
				add_to_wish_list(223);
				add_to_wish_list(224);
				add_to_wish_list(225);
				break;
			case 68: case 69:
				add_to_wish_list(68);			
				add_to_wish_list(69);						
				break;
			case 86: case 87:
				add_to_wish_list(86);			
				add_to_wish_list(87);						
				break;
				
			default:
				add_to_wish_list((short) terrain_pic[ter]);
				break;
			}
}

void load_outdoor_graphics()
{
	short l,m,i,j;
	short pict;

	for (i = 0; i < 96; i++)
		for (j = 0; j < 96; j++) 
			add_terrain_to_wish_list(out[i][j]);

	for (l = 0; l < 2; l++)
	for (m = 0; m < 2; m++) 
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 7; j++)
			if (outdoors[l][m].wandering[i].monst[j] != 0) {
				pict = get_monst_picnum(outdoors[l][m].wandering[i].monst[j]);
				//add_monst_graphic(pict,0);
				add_monst_graphic(outdoors[l][m].wandering[i].monst[j],0);

				j = 8;
				}
		for (j = 0; j < 7; j++)
			if (outdoors[l][m].special_enc[i].monst[j] != 0) {
				pict = get_monst_picnum(outdoors[l][m].special_enc[i].monst[j]);
				//add_monst_graphic(pict,0);
				add_monst_graphic(outdoors[l][m].special_enc[i].monst[j],0);
				j = 8;
				}
		}

	for (i = 0; i < 10; i++)
		if (party.out_c[i].exists == TRUE)
			for (j = 0; j < 7; j++)
				if (party.out_c[i].what_monst.monst[j] != 0) {
					pict = get_monst_picnum(party.out_c[i].what_monst.monst[j]);
					//add_monst_graphic(pict, 0);
					add_monst_graphic(party.out_c[i].what_monst.monst[j], 0);
					j = 8;
					}
}

// Returns TRUE if the picture given is unneeded, and can be discarded. 
/*Boolean monster_pict_not_needed(m_pict)
short m_pict;
{
	short i,j;
	
	if (is_out())
		return FALSE;
		
	for (i = 0; i < T_M; i++)
		if ((c_town.monst.dudes[i].active > 0) && (c_town.monst.dudes[i].m_d.picture_num == m_pict))
			return FALSE;

	if ((is_town()) || ((is_combat()) && (which_combat_type == 1))) {
		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++)
			if (get_monst_picnum(c_town.town.wandering[i].monst[j]) == m_pict)
				return FALSE;
			}
	return TRUE;
} */

// returns 50 is no room, returns 60 is already there, OW returns spot to put in
/*short add_monst_to_template(m_num) 
unsigned char m_num;
{
	short i,p_num;
	
	p_num = get_monst_picnum(m_num);
	if (m_num == 0)
		return 60;
	for (i = 0; i < 21; i++)
		if (monster_index[i] == p_num) 
			return 60;
	for (i = 0; i < 21; i++)
		if ((monster_index[i] == 150) || (monster_pict_not_needed(monster_index[i]) == TRUE)) {
			monster_index[i] = p_num;
			return i;
			}
	return 50;	
			
}*/

void add_monst_graphic(unsigned char m,short mode)
// mode 0 - just put in list, 1 - actually add graphics
{
	short x,y,i,pict;
	get_monst_dims(m,&x,&y);
	for (i = 0; i < x * y; i++) {
		pict = get_monst_picnum(m);
		if (pict >= NUM_MONST_G)
			return;
		pict = m_pic_index[pict];
		if (mode == 0) {
			add_to_wish_list(300 + pict + i);
			add_to_wish_list(600 + pict + i);
			add_to_wish_list(2300 + pict + i);
			add_to_wish_list(2600 + pict + i);
			}
			else {
				add_one_graphic(300 + pict + i);
				add_one_graphic(600 + pict + i);
				add_one_graphic(2300 + pict + i);
				add_one_graphic(2600 + pict + i);
				}
		}
}

void load_town_graphics() // Setting up town monsters takes some finess, due to the flexibility
					// of the situation
// This can be used for town or beginning out outdoor combat
{
	short i,j;
	
	for (i = 0; i < town_size[town_type]; i++)
		for (j = 0; j < town_size[town_type]; j++) 
			if (is_combat())
				add_terrain_to_wish_list(combat_terrain[i][j]);
				else add_terrain_to_wish_list(t_d.terrain[i][j]);

	for (i = 0; i < T_M; i++)
		if ((c_town.monst.dudes[i].number != 0) && (c_town.monst.dudes[i].active > 0))
			add_monst_graphic(c_town.monst.dudes[i].number,0);
	if (is_town())
		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++) {
				add_monst_graphic(c_town.town.wandering[i].monst[j],0);
				}
}

void update_pc_graphics()
{
	short i;
	HBITMAP temp_gworld;
	RECT template_rect = {0,0,28,36}; /**/
	RECT	source_rect;

	if (party_in_memory == FALSE)
		return;

	temp_gworld = load_pict(902,main_dc);

	for (i = 0; i < 6; i++)
		if (adven[i].main_status > 0) 
			if (adven[i].which_graphic != which_graphic_index[i]) {
				template_rect.left = (i / 3) * 56;
				template_rect.right = template_rect.left + 56;
				template_rect.top = (i % 3) * 36;
				template_rect.bottom = template_rect.top + 36;

				source_rect.left = (adven[i].which_graphic / 8) * 56;
				source_rect.right = source_rect.left + 56;
				source_rect.top = 36 * (adven[i].which_graphic % 8);
				source_rect.bottom = 36 * (adven[i].which_graphic % 8) + 36;
				
				rect_draw_some_item_bmp(temp_gworld,source_rect,party_template_gworld,template_rect,0,0);
				
				OffsetRect(&source_rect,280,0);
				OffsetRect(&template_rect,0,108);
				rect_draw_some_item_bmp(temp_gworld,source_rect,party_template_gworld,template_rect,0,0);
			
				which_graphic_index[i] = adven[i].which_graphic;
				}
	DeleteObject (temp_gworld);

}


// This one is complicated, but that's because it's optimized for efficiency.
// Goes through, and loads graphics for anything with storage_status of 3
void put_graphics_in_template()
{
	HBITMAP temp_gworld;
	short i,j,which_position,offset;
	Boolean this_graphic_needed = FALSE;
	RECT from_rect,to_rect;
	
	// First, load all terrains
	for (j = 1; j < 6; j++) {
		for (i = 50; i < STORED_GRAPHICS; i++)
			if ((which_g_stored[i] >= j * 50) && (which_g_stored[i] < j * 50 + 50) &&
				(storage_status[i] == 3)) {
					this_graphic_needed = TRUE;
					}
		if (this_graphic_needed == TRUE) {
			temp_gworld = load_pict(800 + j,main_dc);
			for (i = 50; i < STORED_GRAPHICS; i++)
				if ((which_g_stored[i] >= j * 50) && (which_g_stored[i] < j * 50 + 50) &&
					(storage_status[i] == 3)) {
					which_position = which_g_stored[i] - j * 50;
					from_rect = calc_rect(which_position % 10,which_position / 10);
					to_rect = calc_rect(i % 10,i / 10);
				
					rect_draw_some_item_bmp(temp_gworld,from_rect,storage_gworld,to_rect,0,0);
					storage_status[i] = 1;
					}
			DisposeGWorld (temp_gworld);		
			}
		this_graphic_needed = FALSE;
		}

	// Now, load all monsters
	for (j = 0; j < 10; j++) {
		for (i = 50; i < STORED_GRAPHICS; i++)
			if ((which_g_stored[i] >= 300 + j * 20) && (which_g_stored[i] < 300 + j * 20 + 20) &&
				(storage_status[i] == 3))
					this_graphic_needed = TRUE;
		for (i = 50; i < STORED_GRAPHICS; i++)
			if ((which_g_stored[i] >= 600 + j * 20) && (which_g_stored[i] < 600 + j * 20 + 20) &&
				(storage_status[i] == 3))
					this_graphic_needed = TRUE;

		if (this_graphic_needed == TRUE) {
			temp_gworld = load_pict(1100 + j,main_dc);
			for (i = 50; i < STORED_GRAPHICS; i++)
				if ((which_g_stored[i] >= 300 + j * 20) && (which_g_stored[i] < 300 + j * 20 + 20) &&
					(storage_status[i] == 3)) {
					which_position = which_g_stored[i] % 20;
					from_rect = calc_rect((which_position / 10) * 2,which_position % 10);
					to_rect = calc_rect(i % 10,i / 10);
				
					rect_draw_some_item_bmp(temp_gworld,from_rect,storage_gworld,to_rect,0,0);
				
					storage_status[i] = 1;
					}
			for (i = 50; i < STORED_GRAPHICS; i++)
				if ((which_g_stored[i] >= 600 + j * 20) && (which_g_stored[i] < 600 + j * 20 + 20) &&
					(storage_status[i] == 3)) {
					which_position = which_g_stored[i] % 20;
					from_rect = calc_rect((which_position / 10) * 2 + 1,which_position % 10);
					to_rect = calc_rect(i % 10,i / 10);
				
					rect_draw_some_item_bmp(temp_gworld,from_rect,storage_gworld,to_rect,0,0);
				
					storage_status[i] = 1;
					}
			DisposeGWorld (temp_gworld);		
			}
		this_graphic_needed = FALSE;
		}

	// Now, load all monster combat poses
	for (j = 0; j < 10; j++) {
		for (i = 50; i < STORED_GRAPHICS; i++)
			if ((which_g_stored[i] >= 2300 + j * 20) && (which_g_stored[i] < 2300 + j * 20 + 20) &&
				(storage_status[i] == 3))
					this_graphic_needed = TRUE;
		for (i = 50; i < STORED_GRAPHICS; i++)
			if ((which_g_stored[i] >= 2600 + j * 20) && (which_g_stored[i] < 2600 + j * 20 + 20) &&
				(storage_status[i] == 3))
					this_graphic_needed = TRUE;

		if (this_graphic_needed == TRUE) {
			temp_gworld = load_pict(1200 + j,main_dc);
			for (i = 50; i < STORED_GRAPHICS; i++)
				if ((which_g_stored[i] >= 2300 + j * 20) && (which_g_stored[i] < 2300 + j * 20 + 20) &&
					(storage_status[i] == 3)) {
					which_position = which_g_stored[i] % 20;
					from_rect = calc_rect((which_position / 10) * 2 + 4,which_position % 10);
					to_rect = calc_rect(i % 10,i / 10);
				
					rect_draw_some_item_bmp(temp_gworld,from_rect,storage_gworld,to_rect,0,0);
				
					storage_status[i] = 1;
					}
			for (i = 50; i < STORED_GRAPHICS; i++)
				if ((which_g_stored[i] >= 2600 + j * 20) && (which_g_stored[i] < 2600 + j * 20 + 20) &&
					(storage_status[i] == 3)) {
					which_position = which_g_stored[i] % 20;
					from_rect = calc_rect((which_position / 10) * 2 + 5,which_position % 10);
					to_rect = calc_rect(i % 10,i / 10);
				
					rect_draw_some_item_bmp(temp_gworld,from_rect,storage_gworld,to_rect,0,0);
				
					storage_status[i] = 1;
					}
			DisposeGWorld (temp_gworld);		
			}
		this_graphic_needed = FALSE;
		}


	// Now, anim terrains
	for (i = 50; i < STORED_GRAPHICS; i++) 
		if ((which_g_stored[i] >= 1000) && (which_g_stored[i] < 1400) &&
			(storage_status[i] == 3))
				this_graphic_needed = TRUE;
	if (this_graphic_needed == TRUE) {
		temp_gworld = load_pict(820,main_dc);
		for (i = 50; i < STORED_GRAPHICS; i++)
			if ((which_g_stored[i] >= 1000) && (which_g_stored[i] < 1400) &&
				(storage_status[i] == 3)) {
				which_position = which_g_stored[i] % 100;
				offset = (which_g_stored[i] - 1000) / 100;
				from_rect = calc_rect(4 * (which_position / 5) + offset,which_position % 5);
				to_rect = calc_rect(i % 10,i / 10);
				
				rect_draw_some_item_bmp(temp_gworld,from_rect,storage_gworld,to_rect,0,0);
			
				storage_status[i] = 1;
				}
		DisposeGWorld (temp_gworld);		
		}
}

// this is used for determinign whether to round off walkway corners
// right now, trying a restrictive rule (just cave floor and grass, mainly)
Boolean is_nature(char x, char y)
{
	short pic;
	unsigned char ter_type;
	
	ter_type = coord_to_ter((short) x,(short) y);
	pic = scenario.ter_types[ter_type].picture;
	if ((pic >= 0) && (pic <= 45))
		return TRUE;
	if ((pic >= 67) && (pic <= 73))
		return TRUE;
	if ((pic >= 75) && (pic <= 87))
		return TRUE;
	if ((pic >= 121) && (pic <= 122))
		return TRUE;
	if ((pic >= 179) && (pic <= 208))
		return TRUE;
	if ((pic >= 211) && (pic <= 212))
		return TRUE;
	if ((pic >= 217) && (pic <= 246))
		return TRUE;

	return FALSE;
}



void draw_terrain(short	mode)
//mode ... if 1, don't place on screen after redoing
// if 2, only redraw over active monst
{
	char q,r;
	location where_draw;
	location sector_p_in,view_loc;
	char can_draw;
	unsigned char spec_terrain;
	Boolean off_terrain = FALSE,draw_trim = TRUE;
	short i,j,short_spec_terrain;

	if (mode == 2) {
		if (current_working_monster < 0) return;
		supressing_some_spaces = TRUE;
		for (i = 0; i < 4; i++) ok_space[i].x = -1;
		if (current_working_monster >= 100) {
			for (i = 0; i < c_town.monst.dudes[current_working_monster - 100].m_d.x_width; i++)
				for (j = 0; j < c_town.monst.dudes[current_working_monster - 100].m_d.y_width; j++) {
					ok_space[i + 2 * j].x = c_town.monst.dudes[current_working_monster - 100].m_loc.x + i;
					ok_space[i + 2 * j].y = c_town.monst.dudes[current_working_monster - 100].m_loc.y + j;
					ok_space[i + 2 * j].x = ok_space[i + 2 * j].x - center.x + 4;
					ok_space[i + 2 * j].y = ok_space[i + 2 * j].y - center.y + 4;
					}
			}
		if (current_working_monster < 6) {
			ok_space[0] = pc_pos[current_working_monster];
			ok_space[0].x = ok_space[0].x - center.x + 4;
			ok_space[0].y = ok_space[0].y - center.y + 4;	
			}
		mode = 0;
		}
	
	for (i = 0; i < 13; i++)
		for (j = 0; j < 13; j++) {
			light_area[i][j] = 0;unexplored_area[i][j] = 0;
			}
	
	last_redraw_time = GetCurrentTime();
	sector_p_in.x = party.outdoor_corner.x + party.i_w_c.x;
	sector_p_in.y = party.outdoor_corner.y + party.i_w_c.y;

	anim_onscreen = FALSE;

	if (is_town())
		view_loc = c_town.p_loc;
	if (is_combat())
		view_loc = pc_pos[(current_pc < 6) ? current_pc : first_active_pc()];
	
	for (i = 0; i < 13; i++)
		for (j = 0; j < 13; j++) {
			where_draw =  (is_out()) ? party.p_loc : center;
			where_draw.x += i - 6;
			where_draw.y += j - 6;
			if (is_out() == FALSE)
				light_area[i][j] = (is_town()) ? pt_in_light(view_loc,where_draw) : combat_pt_in_light(where_draw);
			if ((is_out() == FALSE) && ((where_draw.x < 0) || (where_draw.x > town_size[town_type] - 1)
				|| (where_draw.y < 0) || (where_draw.y > town_size[town_type] - 1)))
					unexplored_area[i][j] = 0;
				else unexplored_area[i][j] = 1 - is_explored(where_draw.x,where_draw.y);
			}


	for (q = 0; q < 9; q++) {
		for (r = 0; r < 9; r++)
			{
				where_draw = (is_out()) ? party.p_loc : center;
				where_draw.x += q - 4;
				where_draw.y += r - 4;
				off_terrain = FALSE;
				
				draw_trim = TRUE;
				if ((is_out() == FALSE) && ((where_draw.x < 0) || (where_draw.x > town_size[town_type] - 1)
						|| (where_draw.y < 0) || (where_draw.y > town_size[town_type] - 1))) {
							draw_trim = FALSE;
							// Warning - this section changes where_draw
							if (where_draw.x < 0)
								where_draw.x = -1;
							if (where_draw.x > town_size[town_type] - 1) 
								where_draw.x = town_size[town_type];
							if (where_draw.y < 0)
								where_draw.y = -1;
							if (where_draw.y > town_size[town_type] - 1)
								where_draw.y = town_size[town_type];
							if (can_see(view_loc,where_draw,0) < 5)
								can_draw = 1;
								else can_draw = 0;							
							spec_terrain = 0;
							}
				else if (is_out()) {
					if ((where_draw.x < 0) || (where_draw.x > 95)
						|| (where_draw.y < 0) || (where_draw.y > 95))
						can_draw = 0;
						else {
							spec_terrain = out[where_draw.x][where_draw.y];
							can_draw = out_e[where_draw.x][where_draw.y];
							}						
						}
					else if (is_combat()) {

								spec_terrain = combat_terrain[where_draw.x][where_draw.y];
								if (cartoon_happening == TRUE)
									can_draw = TRUE;
									else can_draw = (((is_explored(where_draw.x,where_draw.y)) ||
									(which_combat_type == 0) || (monsters_going == TRUE) || (overall_mode != 10))
									  && (party_can_see(where_draw) < 6)) ? 1 : 0;

						}
					else {
						spec_terrain = t_d.terrain[where_draw.x][where_draw.y];
						can_draw = is_explored(where_draw.x,where_draw.y);

						if (can_draw > 0) {
							if (pt_in_light(c_town.p_loc,where_draw) == FALSE)
								can_draw = 0;	
							}	
						if ((overall_mode == 36) && (can_draw == 0))
							can_draw = (party_can_see(where_draw) < 6) ? 1 : 0;
						}
				spot_seen[q][r] = can_draw;

						
				if ((can_draw != 0) && (overall_mode != 50)) { // if can see, not a pit, and not resting
					if ((is_combat()) && (cartoon_happening == FALSE)) {
						anim_ticks = 0;
						}
						
					short_spec_terrain = spec_terrain;

					// Finally, draw this terrain spot

					switch (short_spec_terrain) {
						case 82: // cave wway
							 if (loc_off_act_area(where_draw) == FALSE) {
								if ((is_nature(where_draw.x - 1,where_draw.y)) &&
								 (is_nature(where_draw.x,where_draw.y - 1) )) 
									short_spec_terrain = 10219;
								if ((is_nature(where_draw.x + 1,where_draw.y) ) &&
								 (is_nature(where_draw.x,where_draw.y - 1) )) 
									short_spec_terrain = 10220;
								if ((is_nature(where_draw.x + 1,where_draw.y) ) &&
								 (is_nature(where_draw.x,where_draw.y + 1) )) 
									short_spec_terrain = 10221;
								if ((is_nature(where_draw.x - 1,where_draw.y) ) &&
								 (is_nature(where_draw.x,where_draw.y + 1) )) 
									short_spec_terrain = 10218;
								}
							draw_one_terrain_spot(q,r,short_spec_terrain,0);
							break;
						case 83: // ground wway
							if (loc_off_act_area(where_draw) == FALSE) {
								if ((is_nature(where_draw.x - 1,where_draw.y))  &&
								 (is_nature(where_draw.x,where_draw.y - 1)) ) 
									short_spec_terrain = 10223;
								if ((is_nature(where_draw.x + 1,where_draw.y) ) &&
								 (is_nature(where_draw.x,where_draw.y - 1) ))
									short_spec_terrain = 10224;
								if ((is_nature(where_draw.x + 1,where_draw.y) ) &&
								 (is_nature(where_draw.x,where_draw.y + 1) )) 
									short_spec_terrain = 10225;
								if ((is_nature(where_draw.x - 1,where_draw.y) ) &&
								 (is_nature(where_draw.x,where_draw.y + 1) )) 
									short_spec_terrain = 10222;
								}
							draw_one_terrain_spot(q,r,short_spec_terrain,0);
							break;

						case 79: case 80: case 81:
							if ((short_spec_terrain == 81) 
								&& ((out[where_draw.x][where_draw.y - 1] == 80) || (out[where_draw.x][where_draw.y - 1] == 79)))
										short_spec_terrain = 42;
							if ((short_spec_terrain == 81) 
								&& ((out[where_draw.x][where_draw.y + 1] == 80) || (out[where_draw.x][where_draw.y + 1] == 79)))
										short_spec_terrain = 38;
							if ((short_spec_terrain == 81) 
								&& ((out[where_draw.x - 1][where_draw.y] == 80) || (out[where_draw.x - 1][where_draw.y] == 79)))
										short_spec_terrain = 44;
							if ((short_spec_terrain == 81) 
								&& ((out[where_draw.x + 1][where_draw.y ] == 80) || (out[where_draw.x + 1][where_draw.y] == 79)))
										short_spec_terrain = 40;
							/*if ((short_spec_terrain == 81) 
								&& ((out[where_draw.x][where_draw.y - 1] != 234) && (out[where_draw.x][where_draw.y - 1] != 81) &&
									((out[where_draw.x][where_draw.y - 1] < 36) || (out[where_draw.x][where_draw.y - 1] > 49))))
										short_spec_terrain = 42;
							if ((short_spec_terrain == 81) 
								&& ((out[where_draw.x][where_draw.y + 1] != 234) && (out[where_draw.x][where_draw.y + 1] != 81) &&
									((out[where_draw.x][where_draw.y + 1] < 36) || (out[where_draw.x][where_draw.y + 1] > 49))))
										short_spec_terrain = 38;
							if ((short_spec_terrain == 81) 
								&& ((out[where_draw.x - 1][where_draw.y] != 234) &&(out[where_draw.x - 1][where_draw.y] != 81) &&
									((out[where_draw.x - 1][where_draw.y] < 36) || (out[where_draw.x - 1][where_draw.y] > 49))))
										short_spec_terrain = 44;
							if ((short_spec_terrain == 81) 
								&& ((out[where_draw.x + 1][where_draw.y] != 234) && (out[where_draw.x + 1][where_draw.y] != 81) &&
									((out[where_draw.x + 1][where_draw.y] < 36) || (out[where_draw.x + 1][where_draw.y] > 49))))
										short_spec_terrain = 40;*/
							draw_one_terrain_spot(q,r,short_spec_terrain,0);	
							place_road(q,r,where_draw);
							break;
						case 90:
							draw_one_terrain_spot(q,r,-1,0);	
							break;
						default:
							if (short_spec_terrain < 2)
								current_ground = 0;
							if ((short_spec_terrain == 2) || (
								(short_spec_terrain >= 22) && (short_spec_terrain <= 49)))
								current_ground = 2;
							draw_one_terrain_spot(q,r,short_spec_terrain,0);
							break;
						}

					}
					else {  // Can't see. Place darkness.
						draw_one_terrain_spot(q,r,-1,0);
						}

				if ((can_draw != 0) && (overall_mode != 50) && (frills_on == TRUE)
				 && (draw_trim == TRUE) && (cartoon_happening == FALSE)) {  // Place the trim
					place_trim((short) q,(short) r,where_draw,spec_terrain);
					}
			}
		}

	if ((overall_mode != 50) && (!is_out()))
		draw_sfx();

	// Now place items
	if ((overall_mode > 0) && (overall_mode != 35) && (overall_mode != 50))
		draw_items();

	// Now place fields
	if ((overall_mode != 50) && (!is_out())) {
		draw_fields();
		draw_spec_items();
		}

	// Not camping. Place misc. shit.
	if (overall_mode != 50) {
		if (is_out())
			draw_outd_boats(party.p_loc);
			else if ((is_town()) || (which_combat_type == 1))
				draw_town_boat(center);
		draw_monsters();
		}


	if ((overall_mode < 10) || (overall_mode == 35) || ((overall_mode == 36) && (point_onscreen(c_town.p_loc,center) == TRUE))
		|| (overall_mode == 50))
		draw_party_symbol(mode,center);
		else if (overall_mode != 36)
			draw_pcs(center,0);


		apply_light_mask();
	apply_unseen_mask();
	  //	  }
	if (mode == 0) {
		redraw_terrain();

		if (cartoon_happening == FALSE) {
			draw_text_bar(0);
			if ((overall_mode > 9) && (overall_mode != 35) && (overall_mode != 36) && (overall_mode != 50))
				draw_pcs(center,1);
			if (overall_mode == 14)
				draw_targets(center);

			}

		}

   
	supressing_some_spaces = FALSE;
}

void place_trim(short q,short r,location where,unsigned char ter_type)
{
	Boolean at_top = FALSE,at_bot = FALSE,at_left = FALSE,at_right = FALSE;
	unsigned char store,store2,store3,store1;
	location targ;
	
	// FIrst quick check ... if a pit or barrier in outdoor combat, no trim
	if ((is_combat()) && (which_combat_type == 0) && (ter_type == 86))
		return;
	if (PSD[296][0] > 0)
		return;
		
	targ.x = q;
	targ.y = r;
	if ((supressing_some_spaces == TRUE) && 
		(same_point(targ,ok_space[0]) == FALSE) &&
		(same_point(targ,ok_space[1]) == FALSE) &&
		(same_point(targ,ok_space[2]) == FALSE) &&
		(same_point(targ,ok_space[3]) == FALSE))
			return;
	
					
	if (where.x == 0)
		at_left = TRUE;
	if (where.y == 0)
		at_top = TRUE;
	if ((overall_mode == 0) || (overall_mode == 35)) {
		if (where.x == 95)
			at_right = TRUE;
		if (where.y == 95)
			at_bot = TRUE;
		}
		else {
		if (where.x == town_size[town_type])
			at_right = TRUE;
		if (where.y == town_size[town_type])
			at_bot = TRUE;
		}
		
	// First, trim for fluids
	if (((is_town()) || (is_combat())) && (town_trim[where.x][where.y] != 0)) {

		if (town_trim[where.x][where.y] & 1)
				draw_trim(q,r,1,0);	
		if (town_trim[where.x][where.y] & 2)
				draw_trim(q,r,2,5);	
		if (town_trim[where.x][where.y] & 4)
				draw_trim(q,r,0,3);
		if (town_trim[where.x][where.y] & 8)
				draw_trim(q,r,2,6);
		if (town_trim[where.x][where.y] & 16)
				draw_trim(q,r,1,1);
		if (town_trim[where.x][where.y] & 32)
				draw_trim(q,r,2,7);
		if (town_trim[where.x][where.y] & 64)
				draw_trim(q,r,0,2);
		if (town_trim[where.x][where.y] & 128)
				draw_trim(q,r,2,4);			
		}
	if ((is_out()) && (out_trim[where.x][where.y] != 0)) {
		if (out_trim[where.x][where.y] & 1)
				draw_trim(q,r,1,0);	
		if (out_trim[where.x][where.y] & 2)
				draw_trim(q,r,2,5);	
		if (out_trim[where.x][where.y] & 4)
				draw_trim(q,r,0,3);
		if (out_trim[where.x][where.y] & 8)
				draw_trim(q,r,2,6);
		if (out_trim[where.x][where.y] & 16)
				draw_trim(q,r,1,1);
		if (out_trim[where.x][where.y] & 32)
				draw_trim(q,r,2,7);
		if (out_trim[where.x][where.y] & 64)
				draw_trim(q,r,0,2);
		if (out_trim[where.x][where.y] & 128)
				draw_trim(q,r,2,4);			
		}

				
	if (((ter_type >= 100) && (ter_type <= 137)) && (at_top == FALSE) &&
		(at_bot == FALSE) && (at_left == FALSE) && (at_right == FALSE)) {
		store = get_t_t(where.x - 1,where.y);
		store1 = get_t_t(where.x,where.y - 1);
		store2 = get_t_t(where.x + 1,where.y);
		store3 = get_t_t(where.x,where.y + 1);
		if ((is_wall(store) == TRUE) 
			&& (is_wall(store1) == TRUE) &&
			(is_ground(store2) == TRUE) 
			&& (is_ground(store3) == TRUE))
				draw_trim(q,r,3,6);		

		if ((is_wall(store) == TRUE) 
			&& (is_wall(store3) == TRUE) &&
			(is_ground(store2) == TRUE) 
			&& (is_ground(store1) == TRUE))
				draw_trim(q,r,3,5);		

		if ((is_wall(store2) == TRUE) 
			&& (is_wall(store1) == TRUE) &&
			(is_ground(store) == TRUE) 
			&& (is_ground(store3) == TRUE))
				draw_trim(q,r,3,7);		

		if ((is_wall(store2) == TRUE) 
			&& (is_wall(store3) == TRUE) &&
			(is_ground(store) == TRUE) 
			&& (is_ground(store1) == TRUE))
				draw_trim(q,r,3,4);		


		if ((is_ground(store) == TRUE) 
			&& (is_ground(store1) == TRUE) &&
			(is_ground(store2) == TRUE) 
			&& (is_wall(store3) == TRUE)) {
				draw_trim(q,r,3,4);		
				draw_trim(q,r,3,5);						
				}

		if ((is_wall(store) == TRUE) 
			&& (is_ground(store3) == TRUE) &&
			(is_ground(store2) == TRUE) 
			&& (is_ground(store1) == TRUE)) {
				draw_trim(q,r,3,5);		
				draw_trim(q,r,3,6);						
				}

		if ((is_ground(store2) == TRUE) 
			&& (is_wall(store1) == TRUE) &&
			(is_ground(store) == TRUE) 
			&& (is_ground(store3) == TRUE)) {
				draw_trim(q,r,3,6);		
				draw_trim(q,r,3,7);		
				}

		if ((is_wall(store2) == TRUE) 
			&& (is_ground(store3) == TRUE) &&
			(is_ground(store) == TRUE) 
			&& (is_ground(store1) == TRUE)) {
				draw_trim(q,r,3,4);		
				draw_trim(q,r,3,7);		
				}
		}
}

void draw_trim(short q,short r,short which_trim,short which_mode)
//which_trim is 3 -> drawing wall trim -> might shift down if ground is grass
//short which_mode;  // 0 top 1 bottom 2 left 3 right 4 up left 5 up right 6 down right 7 down left
{
	RECT from_rect = {0,0,28,36},to_rect;


	if (frills_on == FALSE)
		return;

	// if current ground is grass, forget trim
	if ((current_ground == 2) && (which_trim < 3))
		return;

	terrain_there[q][r] = -1;

	from_rect.left = 28 * which_trim + trim_rects[which_mode].left;
	from_rect.right = 28 * which_trim + trim_rects[which_mode].right;
	from_rect.top = trim_rects[which_mode].top;
	from_rect.bottom = trim_rects[which_mode].bottom;

	if ((which_trim == 3) && (current_ground == 2)) // trim corner walls with grass instead of cave floor
		OffsetRect(&from_rect,0,36);
	to_rect = coord_to_rect(q,r);
	to_rect.right = to_rect.left + trim_rects[which_mode].right;
	to_rect.left = to_rect.left + trim_rects[which_mode].left;
	to_rect.bottom = to_rect.top + trim_rects[which_mode].bottom;
	to_rect.top = to_rect.top + trim_rects[which_mode].top;
	OffsetRect(&to_rect,-61,-37);

	rect_draw_some_item_bmp(mixed_gworld,from_rect,terrain_screen_gworld,to_rect,1,0);
}


Boolean extend_road_terrain(unsigned char ter)
{
	short i;
	short extend_pics[39] = {61,62,63,64,65, 66,401,402,406,202,
							203,204,215,216,90, 91,92,93,102,103,
							104,105,112,113,114, 115,187,188,189,190,
							192,193,194,195,196, 197,191,200,201};
	
	for (i = 0; i < 39; i++)
		if (scenario.ter_types[ter].picture == extend_pics[i])
			return TRUE;
	return FALSE;
}

void place_road(short q,short r,location where)
{
	location draw_loc;
	unsigned char ter;
	RECT to_rect;
	RECT road_rects[2] = {{112,76,125,80},{144,72,148,90}}; // 0 - rl partial  1 - ud partial
	RECT road_dest_rects[4] = {{12,0,16,18},{15,16,28,20},{12,18,16,36},{0,16,13,20}}; // top right bottom left
	/**/

	draw_loc.x = q; draw_loc.y = r;

	terrain_there[q][r] = -1;

	if (where.y > 0)
		ter = coord_to_ter(where.x,where.y - 1);
	if ((where.y == 0) || (extend_road_terrain(ter) == TRUE)) {
		to_rect = road_dest_rects[0];
		OffsetRect(&to_rect,13 + q * 28,13 + r * 36);
		rect_draw_some_item_bmp(fields_gworld, road_rects[1], terrain_screen_gworld, to_rect, 0, 0);
		}

	if (((is_out()) && (where.x < 96)) || (!(is_out()) && (where.x < town_size[town_type] - 1)))
		ter = coord_to_ter(where.x + 1,where.y);
	if (((is_out()) && (where.x == 96)) || (!(is_out()) && (where.x == town_size[town_type] - 1))
	 || (extend_road_terrain(ter) == TRUE)) {
		to_rect = road_dest_rects[1];
		OffsetRect(&to_rect,13 + q * 28,13 + r * 36);
		rect_draw_some_item_bmp(fields_gworld, road_rects[0], terrain_screen_gworld, to_rect, 0, 0);
		}

	if (((is_out()) && (where.y < 96)) || (!(is_out()) && (where.y < town_size[town_type] - 1)))
		ter = coord_to_ter(where.x,where.y + 1);
	if (((is_out()) && (where.y == 96)) || (!(is_out()) && (where.y == town_size[town_type] - 1))
	 || (extend_road_terrain(ter) == TRUE)) {
		to_rect = road_dest_rects[2];
		OffsetRect(&to_rect,13 + q * 28,13 + r * 36);
		rect_draw_some_item_bmp(fields_gworld, road_rects[1], terrain_screen_gworld, to_rect, 0, 0);
		}

	if (where.x > 0)
		ter = coord_to_ter(where.x - 1,where.y);
	if ((where.x == 0) || (extend_road_terrain(ter) == TRUE)) {
		to_rect = road_dest_rects[3];
		OffsetRect(&to_rect,13 + q * 28,13 + r * 36);
		rect_draw_some_item_bmp(fields_gworld, road_rects[0], terrain_screen_gworld, to_rect, 0, 0);
		}

}

void draw_rest_screen()
{
	short store_mode;

	store_mode = overall_mode;
	overall_mode = 50;
	draw_terrain(0);
	overall_mode = store_mode ;
}

void pre_boom_space(location where,short mode,short type,short damage,short sound)
// if mode is 100, force
//short type; // 0 - flame 1 - magic 2 - poison 3 - blood 4 - cold
	// 10s digit indicates sound  0 - normal ouch  1 - small sword  2 - loud sword
	// 3 - pole  4 - club  5 - fireball hit  6 - squish  7 - cold
	// 8 - acid  9 - claw  10 - bite  11 - slime  12 - zap  13 - missile hit
{
	short sound_key;
	short sound_to_play[20] = {97,69,70,71,72, 73,55,75,42,86,
			87,88,89,98,0, 0,0,0,0,0};

	sound_key = type / 10;
	type = type % 10;

//	if ((cartoon_happening == TRUE) && (anim_step < 140))
//		return;
	if ((cartoon_happening == FALSE) && ((mode != 100) && (party_can_see(where) == 6)))
		return;
	if ((type < 0) || (type > 4))
		return;
	if ((boom_anim_active == TRUE) && (type != 3))
		return;
	if ((cartoon_happening == FALSE) && (PSD[306][2] > 0) && (fast_bang == TRUE))
		return;
	if (is_out())
		return;

	// Redraw terrain in proper position
	if ((((point_onscreen(center,where) == FALSE) && (overall_mode >= 10)) || (overall_mode == 0))
		) {
		play_sound(sound_to_play[sound]);

		return;
		}
		else if (is_combat()) {
			if (which_combat_type == 1)
				draw_terrain(0);
				else draw_terrain(0);
			}
			else if (fast_bang < 2) {
				draw_terrain(0);
				if (fast_bang == 1)
					fast_bang = 2;
				}

	boom_space(where,mode,type,damage,sound);
 }

 void boom_space(location where,short mode,short type,short damage,short sound)
// if mode is 100, force
//short type; // 0 - flame 1 - magic 2 - poison 3 - blood 4 - cold
	// 10s digit indicates sound  0 - normal ouch  1 - small sword  2 - loud sword
	// 3 - pole  4 - club  5 - fireball hit  6 - squish  7 - cold
	// 8 - acid  9 - claw  10 - bite  11 - slime  12 - zap  13 - missile hit
{
	location where_draw = {4,4};
	RECT source_rect = {0,0,28,36},text_rect,dest_rect = {13,13,41,49},big_to = {13,13,265,337},store_rect;
	/**/
	RECT terrain_from;
	long dummy;
	short del_len,sound_key;
	char dam_str[20];
	short x_adj = 0,y_adj = 0,which_m;
	RECT mixed_square = {353,169,381,205};
	short sound_to_play[20] = {97,69,70,71,72, 73,55,75,42,86,
			87,88,89,98,0, 0,0,0,0,0};

 	sound_key = type / 10;
	type = type % 10;

	where_draw.x = where.x - center.x + 4;
	where_draw.y = where.y - center.y + 4;
//	source_rect.left += 28 * type;
//	source_rect.right += 28 * type;
	
	// adjust for possible big monster
	which_m = monst_there(where);
	if (which_m < 90) {
		x_adj += 14 * (c_town.monst.dudes[which_m].m_d.x_width - 1);
		y_adj += 18 * (c_town.monst.dudes[which_m].m_d.y_width - 1);
		}
	OffsetRect(&dest_rect,where_draw.x * 28,where_draw.y * 36);
	terrain_from = dest_rect;
	source_rect = store_rect = dest_rect;
	OffsetRect(&dest_rect,x_adj,y_adj);
	SectRect(&dest_rect,&big_to,&dest_rect);

	if (cartoon_happening == FALSE)
		OffsetRect(&dest_rect,win_to_rects[0].left,win_to_rects[0].top);
		else if (store_anim_type == 0)
			OffsetRect(&dest_rect,306,5);
			else OffsetRect(&dest_rect,store_anim_ul.x,store_anim_ul.y);

	OffsetRect(&source_rect,-1 * store_rect.left + 28 * type,-1 * store_rect.top);
//	rect_draw_some_item_bmp(fields_gworld,source_rect,terrain_screen_gworld,dest_rect,1,1);
//	print_nums(0,source_rect.left,source_rect.top);
//   print_nums(1,dest_rect.left,dest_rect.top);

	OffsetRect(&terrain_from,x_adj,y_adj);
	rect_draw_some_item_bmp(terrain_screen_gworld,terrain_from,mixed_gworld,mixed_square,0,0);
	rect_draw_some_item_bmp(fields_gworld,source_rect,mixed_gworld,mixed_square,1,0);
	rect_draw_some_item_bmp(mixed_gworld,mixed_square,mixed_gworld,dest_rect,0,1);

	if ((cartoon_happening == FALSE) && (dest_rect.right - dest_rect.left >= 28)
		&& (dest_rect.bottom - dest_rect.top >= 36)) {
				sprintf(dam_str,"%d",damage);
		text_rect = dest_rect;
		text_rect.top += 10;
		if ((damage < 10) && (dest_rect.right - dest_rect.left > 19))
			text_rect.left += 2;
		OffsetRect(&text_rect,-4,-5);
		//char_win_draw_string(main_dc,text_rect,(char *) dam_str,1,10);
		if ((type == 1) || (type == 4))
			WinBlackDrawString(dam_str,text_rect.left + 12,text_rect.top + 6);
			else WinDrawString(dam_str,text_rect.left + 12,text_rect.top + 6);
		}
		play_sound(sound_to_play[sound]);
		if ((sound == 6) && (fast_bang == 0))
			Delay(12, &dummy);

	Delay(10,&dummy);
	if (fast_bang == 0) {
		del_len = PSD[306][6] * 3 + 4;
		if (play_sounds == FALSE)
			Delay(del_len, &dummy);
		}
	redraw_terrain();
	if ((cartoon_happening == FALSE) && (overall_mode > 9) && (overall_mode != 35) && (overall_mode != 36) && (overall_mode != 50))
		draw_pcs(center,1);	
}
	

void draw_pointing_arrows() 
{
	RECT sources[4] = {{65,46,73,54},{56,46,64,54},{56,37,64,45},{65,37,73,45}};
	RECT dests[8] = {{7,100,15,108},{7,170,15,178},{140,7,148,15},{212,7,220,15},
		{346,100,354,108},{346,170,354,178},{140,274,148,282},{212,274,220,282}};
	// rects modified below
	short i;
	
	for (i = 0; i < 4; i++)
		alter_rect(&sources[i]);
	for (i = 0; i < 8; i++)
		alter_rect(&dests[i]);
	if ((monsters_going == TRUE) || (overall_mode <= 1) || (overall_mode <= 10)
		|| (overall_mode == 35)) 
			return;
	for (i = 0; i < 4; i++) {
		rect_draw_some_item_bmp(mixed_gworld,sources[i],mixed_gworld,dests[i * 2],1,1);
		rect_draw_some_item_bmp(mixed_gworld,sources[i],mixed_gworld,dests[i * 2 + 1],1,1);
		}
}

void redraw_terrain()
{
	RECT to_rect;
	RECT ter_scrn_rect = {0,0,279,351};

	if (cartoon_happening == FALSE)
		to_rect = win_to_rects[0];
		else {
			to_rect = ter_scrn_rect;
			if (store_anim_type == 0)
				OffsetRect(&to_rect,306,5);
				else OffsetRect(&to_rect,store_anim_ul.x,store_anim_ul.y);
			}
	rect_draw_some_item_bmp(terrain_screen_gworld, win_from_rects[0], terrain_screen_gworld, to_rect, 0, 1);


	// Now place arrows
	draw_pointing_arrows();
	

}


void draw_targets(location center)
{
	RECT source_rect = {36,74,47,85},dest_rect; /**/
	short i = 0;

	if (party_toast() == TRUE)
		return;

	for (i = 0; i < 8; i++)
		if ((spell_targets[i].x != 120) && (point_onscreen(center,spell_targets[i]) == TRUE)) {
			dest_rect = coord_to_rect(spell_targets[i].x - center.x + 4,spell_targets[i].y - center.y + 4);
			OffsetRect(&dest_rect,5,5);
			InflateRect(&dest_rect,-8,-12);
			rect_draw_some_item_bmp(mixed_gworld,source_rect,mixed_gworld,dest_rect,1,1);
			}
}


void draw_targeting_line(POINT where_curs)
{
	location which_space,store_loc;
	short i,j,k,l;
	long dummy;
	RECT redraw_rect,redraw_rect2,terrain_rect = {0,0,279,351},target_rect;
	location from_loc;
	RECT on_screen_terrain_area = {18, 18, 269,341};
	char dam_str[20];
	HPEN white_pen;
	HGDIOBJ store_pen;
	static LOGPEN white_pen_data = { PS_SOLID,{2,2},RGB(255,255,255) };
	COLORREF colors[2] = {RGB(0,0,0),RGB(255,255,255)},storec;
	UINT c[2];

	if (party.stuff_done[305][6] > 0)
		return;

	c[0] = GetNearestPaletteIndex(hpal,colors[0]);
	c[1] = GetNearestPaletteIndex(hpal,colors[1]);

	if (overall_mode >= 10)
		from_loc = pc_pos[current_pc];
		else from_loc = c_town.p_loc;
	if ((overall_mode == 11) || (overall_mode == 12) || (overall_mode == 13) || (overall_mode == 14)
	  || ((overall_mode == 3) && (current_pat.pattern[4][4] != 0))) {

		OffsetRect(&on_screen_terrain_area,ulx,uly);
		OffsetRect(&terrain_rect,18,18);
		if (PtInRect (&on_screen_terrain_area,where_curs) == TRUE) {
		// && (point_onscreen(center,pc_pos[current_pc]) == TRUE)){
			i = (where_curs.x - 18 - ulx) / 28;
			j = (where_curs.y - 18 - uly) / 36;
			which_space.x = center.x + (short) i - 4;
			which_space.y = center.y + (short) j - 4;

			k = (short) (from_loc.x - center.x + 4);
			l = (short) (from_loc.y - center.y + 4);
			k = (k * 28) + 32 ;//+ ulx;
			l = (l * 36) + 36 ;

			if ((loc_off_act_area(which_space) == FALSE) && 
				(can_see(from_loc,which_space,0) < 5)
			 && (dist(from_loc,which_space) <= current_spell_range)) {
				SetViewportOrgEx(main_dc,ulx,uly,nullptr);
				white_pen = CreatePenIndirect(&white_pen_data);
				store_pen = SelectObject(main_dc,white_pen);

				//InsetRect(&terrain_rect,13,13);
				//OffsetRect(&terrain_rect,5 + ulx,5 + uly);
				OffsetRect(&on_screen_terrain_area,-1 * ulx,-1 * uly);
				ClipRect(&on_screen_terrain_area);
				MoveToEx(main_dc,where_curs.x - ulx,where_curs.y - uly, NULL);
				LineTo(main_dc,k,l);

				redraw_rect.left = min(where_curs.x,k) - 4;
				redraw_rect.right = max(where_curs.x,k) + 4;
				redraw_rect.top = min(where_curs.y,l) - 4;
				redraw_rect.bottom = max(where_curs.y,l) + 4;
				//IntersectRect(&redraw_rect2,&redraw_rect,&terrain_rect);
				//OffsetRect(&on_screen_terrain_area,ulx,uly);
				IntersectRect(&redraw_rect2,&redraw_rect,&on_screen_terrain_area);

				// Now place targeting pattern
				for (i = 0; i < 9; i++)
					for (j = 0; j < 9; j++) {
						store_loc.x = center.x + i - 4;
						store_loc.y = center.y + j - 4;
						if ((a_v(store_loc.x - which_space.x) <= 4) &&
							(a_v(store_loc.y - which_space.y) <= 4) &&
							(current_pat.pattern[store_loc.x - which_space.x + 4][store_loc.y - which_space.y + 4] != 0)) {
								target_rect.left = 13 + BITMAP_WIDTH * i + 5;// + ulx;
								target_rect.right = target_rect.left + BITMAP_WIDTH;
								target_rect.top = 13 + BITMAP_HEIGHT * j + 5;// + uly;
								target_rect.bottom = target_rect.top + BITMAP_HEIGHT;

								MoveToEx(main_dc,target_rect.left,target_rect.top, NULL);
								LineTo(main_dc,target_rect.right,target_rect.top);
								LineTo(main_dc,target_rect.right,target_rect.bottom);
								LineTo(main_dc,target_rect.left,target_rect.bottom);
								LineTo(main_dc,target_rect.left,target_rect.top);

								InflateRect(&target_rect,5,5);
								UnionRect(&redraw_rect2,&target_rect,&redraw_rect2);

								// Now place number of shots left, if drawing center of target
								if ((overall_mode == 14) && (store_loc.x - which_space.x + 4 == 4)
								 && (store_loc.y - which_space.y + 4 == 4)) {
								 storec = GetTextColor(main_dc);
								 SetTextColor(main_dc,PALETTEINDEX(c[1]));
									sprintf(dam_str,"%d  ",num_targets_left);
									DrawText(main_dc,dam_str,-1,&target_rect,DT_SINGLELINE | DT_VCENTER | DT_CENTER);
									//WinDrawString(dam_str,((target_rect.left + target_rect.right) / 2) - 3,
									// (target_rect.top + target_rect.bottom) / 2);
									SetTextColor(main_dc,storec);
									}

								}
						}
				SelectObject(main_dc,store_pen);
				DeleteObject(white_pen);

				Delay(4,&dummy);

				InflateRect(&redraw_rect2,5,5);
				redraw_partial_terrain(redraw_rect2);
				undo_clip();
				if (is_combat())
					draw_pcs(center,1);
					else draw_party_symbol(0,center);
				if (overall_mode == 14)
					draw_targets(center);	
				}
			}
	}
}


Boolean party_toast()
{
	short i;
	
	for (i = 0; i < 6; i++)
		if (adven[i].main_status == 1)
			return FALSE;
	return TRUE;
}

void redraw_partial_terrain(RECT redraw_rect)
{
	RECT from_rect;

	from_rect = redraw_rect;
	//OffsetRect(&from_rect,-1 * ulx,-1 * uly);

	OffsetRect(&redraw_rect,5, 5);
	
	rect_draw_some_item_bmp(terrain_screen_gworld,from_rect,terrain_screen_gworld,redraw_rect,0,1);

}


// This tells the dialog engine to kill the dialog, and refreshes the screen
// will probably need to be modified for windows
void final_process_dialog(short which_dlog)
{

	cd_kill_dialog(which_dlog,0);

	if (in_startup_mode == FALSE)
		refresh_screen(0);
		else draw_startup(0);
}
