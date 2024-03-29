#include <Windows.h>
#include <array>
#include <cstring>
#include <cstdio>
#include "global.h"
#include "graphics.h"
#include "gutils.h"
#include "monster.h"
#include "dlogtool.h"
#include "dlogtool_helpers.hpp"
#include "newgraph.h"
#include "fileio.h"
#include "locutils.h"
#include "fields.h"
#include "text.h"
#include "exlsound.h"
#include "graphutl.h"
#include "graphutl_helpers.hpp"
#include "boe/utility.hpp"
#include "boe/item.hpp"
#include "game_globals.hpp"

static const std::array heal_types{"Heal Damage","Cure Poison","Cure Disease","Cure Paralysis",
		"Uncurse Items","Cure Stoned Character","Raise Dead","Resurrection","Cure Dumbfounding"};

extern short ulx,uly;
extern RECT	windRect;
extern HBRUSH bg[14];
extern short dungeon_font_num,geneva_font_num,overall_mode,town_type,which_combat_type;
extern Boolean play_sounds,boom_anim_active,cartoon_happening,in_startup_mode;
extern HBITMAP fields_gworld,mixed_gworld,dlg_buttons_gworld,terrain_screen_gworld,missiles_gworld;
extern party_record_type party;
extern talking_record_type talking;

extern Adventurers adven;
extern RECT sbar_rect,item_sbar_rect,shop_sbar_rect;
extern HWND text_sbar,item_sbar,shop_sbar;
extern location center;
extern short pc_marked_damage[6],pc_dir[6],store_talk_face_pic;
extern short monst_marked_damage[T_M];
extern location pc_pos[6];
extern current_town_type c_town;
extern big_tr_type  t_d;
extern town_item_list 	t_i;
extern unsigned char combat_terrain[64][64];
extern unsigned char misc_i[64][64],sfx[64][64];
extern POINT store_anim_ul;
extern char light_area[13][13];
extern short terrain_there[9][9];
extern char unexplored_area[13][13];
extern short combat_posing_monster , current_working_monster ; // 0-5 PC 100 + x - monster x
   extern HFONT fantasy_font,font,italic_font,underline_font,bold_font,tiny_font,small_bold_font;

HRGN oval_region = NULL,dark_mask_region,temp_rect_rgn;

// Talk vars
extern word_rect_type store_words[50];
extern short store_pre_talk_mode,store_personality,store_personality_graphic,current_pc;
extern HBITMAP talk_gworld;
extern Boolean talk_end_forced;
extern char old_str1[256];
extern char old_str2[256];
extern char one_back1[256];
extern char one_back2[256];
extern const std::array<word_rect_type, 9> preset_words;
extern const RECT talk_area_rect, word_place_rect;
extern RECT talk_help_rect;
extern char title_string[50];
extern unsigned char store_monst_type;
extern hold_responses store_resp[83];

// Shop vars
extern short store_shop_items[30];
extern short store_shop_costs[30];
extern short store_shop_type,store_shop_min,store_shop_max,store_pre_shop_mode,store_cost_mult;
extern char store_store_name[256];
extern RECT shopping_rects[8][7];
extern const RECT bottom_help_rects[4];
extern const RECT shop_frame ;
extern const RECT shop_done_rect;
extern item_record_type food_types[15];
extern short heal_costs[8];
extern short terrain_there[9][9];

extern HPALETTE hpal;
extern PALETTEENTRY ape[256];
extern HDC main_dc,main_dc2,main_dc3;

// Missile anim vars
typedef struct {
	location dest;
	short missile_type; // -1 no miss
	short path_type,x_adj,y_adj;
	} store_missile_type;
typedef struct {
	location dest;
	short val_to_place,offset;
	short place_type; // 0 - on spot, 1 - random area near spot
	short boom_type;  // -1 no miss
	short x_adj,y_adj;
	} store_boom_type;
store_missile_type store_missiles[30];
store_boom_type store_booms[30];
Boolean have_missile,have_boom;
RECT explode_place_rect[30];


// Animation vars
extern town_record_type anim_town;
extern tiny_tr_type anim_t_d;
extern short anim_step;
short store_anim_type;
extern char anim_str[256];
extern location anim_str_loc;

// if < 6; target PC, if >= 100, targ monst, if 6, go to anim_pc_loc
short anim_pc_targets[6];
short anim_monst_targets[T_M];

short anim_pcs[6];

location anim_pc_locs[6],anim_monst_locs[T_M];
location anim_string_loc;
char anim_string[60];
char last_light_mask[13][13];

extern HBRUSH checker_brush;
extern HBITMAP checker_bitmap;

void apply_unseen_mask()
{
	RECT base_rect = {8,10,43,53}/*{9,9,43,53}*/,to_rect,big_to = {13,13,265,337}; /**/
	short i,j,k,l;
	Boolean need_bother = FALSE;
	HDC hdc;
	HGDIOBJ store_bmp;
	HGDIOBJ old_brush;
	HGDIOBJ old_pen;

	if (PSD[306][2] > 0)
	{
		return;
	}
	if (is_combat() && (which_combat_type == 0))
	{
		return;
	}
	if (is_not_out() && (c_town.town.lighting > 0))
	{
		return;
	}
		
	for (i = 0; i < 11; i++)
	{
		for (j = 0; j < 11; j++)
		{
			if (unexplored_area[i + 1][j + 1] == 1)
			{
				need_bother = TRUE;
			}
		}
	}

	if (need_bother == FALSE)
	{
		return;
	}
				
	hdc = CreateCompatibleDC(main_dc);
	SetBkMode(hdc,TRANSPARENT);
	old_brush = SelectObject(hdc,checker_brush);
	old_pen = SelectObject(hdc,GetStockObject(NULL_PEN));
	SetROP2(hdc,R2_MASKPEN);

	store_bmp = SelectObject(hdc,terrain_screen_gworld);

	for (i = 0; i < 11; i++)
	{
		for (j = 0; j < 11; j++)
		{
			if (unexplored_area[i + 1][j + 1] == 1)
			{
				to_rect = base_rect;
				OffsetRect(&to_rect, -28 + i * 28 + 2, -36 + 36 * j);
				IntersectRect(&to_rect, &to_rect, &big_to);
				Rectangle(hdc, to_rect.left, to_rect.top, to_rect.right, to_rect.bottom);
				//PaintRect(&to_rect);
				for (k = i - 2; k < i + 1; k++)
				{
					for (l = j - 2; l < j + 1; l++)
					{
						if ((k >= 0) && (l >= 0) && (k < 9) && (l < 9) && ((k != i - 1) || (l != j - 1)))
						{
							terrain_there[k][l] = -1;
						}
					}
				}
			}
		}
	}
	
	// Now put text on window.
	SelectObject(hdc,old_brush);
	SelectObject(hdc,old_pen);
	SelectObject(hdc,store_bmp);
	DeleteObject(hdc);
}
 
void apply_light_mask() 
{
	RECT temp = {0,0,84,108},paint_rect,base_rect = {0,0,28,36};
	RECT big_to = {13,13,265,337}; /**/
	short i,j;
	Boolean is_dark = FALSE,same_mask = TRUE;
	
	HDC hdc;
	HGDIOBJ store_bmp;
	HGDIOBJ old_brush;
	
	if (PSD[306][2] > 0)
	{
		return;
	}
	if (is_out())
	{
		return;
	}
	if (c_town.town.lighting == 0)
	{
		return;
	}
	
	if (oval_region == NULL)
	{
		temp_rect_rgn = CreateRectRgn(0,0,0,0);
		dark_mask_region = CreateRectRgn(0,0,0,0);
		oval_region = CreateEllipticRgnIndirect(&temp);
	}
	
	// Process the light array
	for (i = 2; i < 11; i++)
	{
		for (j = 2; j < 11; j++)
		{
			if (light_area[i][j] == 0)
			{
				is_dark = TRUE;
			}
		}
	}

	if (is_dark == FALSE)
	{ 
		for (i = 2; i < 11; i++)
		{
			for (j = 2; j < 11; j++)
			{
				last_light_mask[i][j] = 0;
			}
		}
		return;
	}

	for (i = 1; i < 12; i++)
	{
		for (j = 1; j < 12; j++)
		{
			if ((light_area[i - 1][j - 1] >= 1) && (light_area[i + 1][j - 1] >= 1) &&
				(light_area[i - 1][j] >= 1) && (light_area[i + 1][j] >= 1) &&
				(light_area[i - 1][j + 1] >= 1) && (light_area[i + 1][j + 1] >= 1) &&
				(light_area[i][j - 1] >= 1) && (light_area[i][j + 1] >= 1))
			{
				light_area[i][j] = 2;
			}
		}
	}

	for (i = 1; i < 12; i++)
	{
		for (j = 1; j < 12; j++)
		{
			if ((light_area[i - 1][j - 1] >= 2) && (light_area[i + 1][j - 1] >= 2) &&
				(light_area[i - 1][j] >= 2) && (light_area[i + 1][j] >= 2) &&
				(light_area[i - 1][j + 1] >= 2) && (light_area[i + 1][j + 1] >= 2) &&
				(light_area[i][j - 1] >= 2) && (light_area[i][j + 1] >= 2))
			{
				light_area[i][j] = 3;
			}
		}
	}
		
	hdc = CreateCompatibleDC(main_dc);
	SetBkMode(hdc,TRANSPARENT);
	//old_brush = SelectObject(hdc,checker_brush);
	//old_pen = SelectObject(hdc,GetStockObject(NULL_PEN));
	store_bmp = SelectObject(hdc,terrain_screen_gworld);
	old_brush = SelectObject(hdc,GetStockObject(BLACK_BRUSH));

	for (i = 2; i < 11; i++)
	{
		for (j = 2; j < 11; j++)
		{
			if (light_area[i][j] == 1)
			{
				terrain_there[i - 2][j - 2] = -1;
			}
		}
	}

	for (i = 0; i < 13; i++)
	{
		for (j = 0; j < 13; j++)
		{
			if (last_light_mask[i][j] != light_area[i][j])
			{
				same_mask = FALSE;
			}
		}
	}

	if (same_mask == TRUE)
	{
		PaintRgn(hdc,dark_mask_region);
		SelectObject(hdc,store_bmp);
		DeleteObject(hdc);
		return;
	}

	SetRectRgn(dark_mask_region,big_to.left,big_to.top,big_to.right,big_to.bottom);

	for (i = 0; i < 13; i++)
	{
		for (j = 0; j < 13; j++)
		{
			last_light_mask[i][j] = light_area[i][j];
		}
	}

	for (i = 1; i < 12; i++)
	{
		for (j = 1; j < 12; j++)
		{
			if (light_area[i][j] == 2)
			{
				OffsetRgn(oval_region, 13 + 28 * (i - 3), 13 + 36 * (j - 3));
				CombineRgn(dark_mask_region, dark_mask_region, oval_region, RGN_DIFF);
				OffsetRgn(oval_region, -13 + -1 * (28 * (i - 3)), -13 + -1 * (36 * (j - 3)));
				//PaintRect(&paint_rect);
			}
			if (light_area[i][j] == 3)
			{
				paint_rect = base_rect;
				OffsetRect(&paint_rect, 13 + 28 * (i - 2), 13 + 36 * (j - 2));
				SetRectRgn(temp_rect_rgn, paint_rect.left, paint_rect.top, paint_rect.right + 28, paint_rect.bottom + 36);
				CombineRgn(dark_mask_region, dark_mask_region, temp_rect_rgn, RGN_DIFF);
				if (light_area[i + 1][j] == 3) light_area[i + 1][j] = 0;
				if (light_area[i + 1][j + 1] == 3) light_area[i + 1][j + 1] = 0;
				if (light_area[i][j + 1] == 3) light_area[i][j + 1] = 0;
			}
		}
	}

	//rect_draw_some_item_bmp(light_mask_gworld,big_from,terrain_screen_gworld,big_to,0,0);
	PaintRgn(hdc,dark_mask_region);
	SelectObject(hdc,store_bmp);
	DeleteObject(hdc);
}

void start_missile_anim()
{
	short i;
	if (boom_anim_active == TRUE)
	{
		return;
	}
	boom_anim_active = TRUE;
	for (i = 0; i < 30; i++)
	{
		store_missiles[i].missile_type = -1;
		store_booms[i].boom_type = -1;
	}
	for (i = 0; i < 6; i++)
	{
		pc_marked_damage[i] = 0;
	}
	for (i = 0; i < T_M; i++)
	{
		monst_marked_damage[i] = 0;
	}
	have_missile = FALSE;
	have_boom = FALSE;
}

void end_missile_anim() 
{
	boom_anim_active = FALSE;
}
	
void add_missile(location dest,short missile_type,short path_type,short x_adj,short y_adj)
{
	short i;
	if (boom_anim_active == FALSE)
	{
		return;
	}
	if (PSD[306][2] > 0)
	{
		return;
	}
	// lose redundant missiles
	for (i = 0; i < 30; i++)
	{
		if ((store_missiles[i].missile_type >= 0) && (same_point(dest, store_missiles[i].dest)))
		{
			return;
		}
	}
	for (i = 0; i < 30; i++)
	{
		if (store_missiles[i].missile_type < 0)
		{
			have_missile = TRUE;
			store_missiles[i].dest = dest;
			store_missiles[i].missile_type = missile_type;
			store_missiles[i].path_type = path_type;
			store_missiles[i].x_adj = x_adj;
			store_missiles[i].y_adj = y_adj;
			return;
		}
	}
}

void run_a_missile(location from,location fire_to,short miss_type,short path,short sound_num,short x_adj,short y_adj,short len)
{
//	if ((cartoon_happening == TRUE) && (anim_step < 140))
//		return;
	start_missile_anim();
	add_missile(fire_to,miss_type,path, x_adj, y_adj);
	do_missile_anim(len,from, sound_num);
	end_missile_anim();
}

void run_a_boom(location boom_where,short type,short x_adj,short y_adj)
{

//	if ((cartoon_happening == TRUE) && (anim_step < 140))
//		return;
	if ((type < 0) || (type > 2))
	{
		return;
	}
	start_missile_anim();
	add_explosion(boom_where,-1,0,type, x_adj, y_adj);
	do_explosion_anim(5,0);
	end_missile_anim();
}
	
void mondo_boom(location l,short type)
{
	short i;
	
	start_missile_anim();
	for (i = 0; i < 12; i++)
	{
		add_explosion(l, -1, 1, type, 0, 0);
	}
	do_explosion_anim(5,0);

	end_missile_anim();	
}
		
void add_explosion(location dest,short val_to_place,short place_type,short boom_type,short x_adj,short y_adj)
{
	short i;
	if (PSD[306][2] > 0)
	{
		return;
	}
	if (boom_anim_active == FALSE)
	{
		return;
	}
	// lose redundant explosions
	for (i = 0; i < 30; i++)
	{
		if ((store_booms[i].boom_type >= 0) && (same_point(dest, store_booms[i].dest))
			&& (place_type == 0))
		{
			if (val_to_place > 0)
			{
				store_booms[i].val_to_place = val_to_place;
			}
			return;
		}
	}
	for (i = 0; i < 30; i++)
	{
		if (store_booms[i].boom_type < 0)
		{
			have_boom = TRUE;
			store_booms[i].offset = (i == 0) ? 0 : -1 * rand_short(0, 2);
			store_booms[i].dest = dest;
			store_booms[i].val_to_place = val_to_place;
			store_booms[i].place_type = place_type;
			store_booms[i].boom_type = boom_type;
			store_booms[i].x_adj = x_adj;
			store_booms[i].y_adj = y_adj;
			return;
		}
	}
}		

void do_missile_anim(short num_steps,location missile_origin,short sound_num) 
{
	RECT temp_rect,missile_origin_base = {1,1,17,17},active_area_rect,to_rect,from_rect;
	short i,store_missile_dir;
	POINT start_point,finish_point[30];
	location screen_ul;
	
	short x1[30],x2[30],y1[30],y2[30],t; // for path paramaterization
	RECT missile_place_rect[30],missile_origin_rect[30],store_erase_rect[30];
	POINT current_terrain_ul; 
	HBITMAP temp_gworld;
	RECT ter_scrn_rect = {0,0,279,351};
	DWORD t1,t2;
	DWORD pause_len = 0;

	t2 = t1 = GetCurrentTime();
	if (sound_num == 11) { pause_len = 660; }
	if (sound_num == 12) { pause_len = 410; }
	if (sound_num == 14) { pause_len = 200; }
	if (sound_num == 53) { pause_len = 1000; }
	if (sound_num == 64) { pause_len = 500; }

	if ((have_missile == FALSE) || (boom_anim_active == FALSE))
	{
		boom_anim_active = FALSE;
		return;
	}
	
	for (i = 0; i < 30; i++)
	{
		if (store_missiles[i].missile_type >= 0)
		{
			i = 50;
		}
	}

	if (i == 30)
	{
		return;
	}

	// initialize general data
	if ((in_startup_mode == TRUE) && (store_anim_type == 0))
	{
		current_terrain_ul.x = 306;
		current_terrain_ul.y = 5;
	}
	else if ((cartoon_happening == TRUE) && (store_anim_type > 0))
	{
		current_terrain_ul = store_anim_ul;
	}
	else
	{
		current_terrain_ul.x = current_terrain_ul.y = 5;
	}

	// make terrain_template contain current terrain all nicely
	draw_terrain(1);
	to_rect = ter_scrn_rect;
	OffsetRect(&to_rect,current_terrain_ul.x, current_terrain_ul.y);
	rect_draw_some_item_bmp(terrain_screen_gworld,ter_scrn_rect,terrain_screen_gworld,to_rect,0,1);
							
	// create and clip temporary anim template 
	temp_rect = ter_scrn_rect;
	temp_gworld = CreateCompatibleBitmap(main_dc,ter_scrn_rect.right,ter_scrn_rect.bottom);
	/*SetPort((GrafPtr) temp_gworld);    */
	active_area_rect = temp_rect;
	InflateRect(&active_area_rect,-13,-13);
	/*ClipRect(&active_area_rect); */ //Deleting this clipping ... I suspect will still work

	// init missile paths
	for (i = 0; i < 30; i++)
	{
		SetRectEmpty(&store_erase_rect[i]);
		if ((store_missiles[i].missile_type >= 0) && (same_point(missile_origin, store_missiles[i].dest)))
		{
			store_missiles[i].missile_type = -1;
		}
	}

	screen_ul.x = center.x - 4; screen_ul.y = center.y - 4;
	start_point.x = 13 + 14 + 28 * (short) (missile_origin.x - screen_ul.x);
	start_point.y = 13 + 18 + 36 * (short) (missile_origin.y - screen_ul.y);
	for (i = 0; i < 30; i++)
	{
		if (store_missiles[i].missile_type >= 0)
		{
			finish_point[i].x = 1 + 13 + 14 + store_missiles[i].x_adj + 28 * (short) (store_missiles[i].dest.x - screen_ul.x);
			finish_point[i].y = 1 + 13 + 18 + store_missiles[i].y_adj + 36 * (short) (store_missiles[i].dest.y - screen_ul.y);
			// note ... +1 at beginning is put in to prevent infinite slope

			if (store_missiles[i].missile_type < 7)
			{
				store_missile_dir = get_missile_direction(start_point,finish_point[i]);
				missile_origin_rect[i] = missile_origin_base;
				OffsetRect(&missile_origin_rect[i],18 * store_missile_dir,18 * store_missiles[i].missile_type);
			}
			else
			{
				missile_origin_rect[i] = missile_origin_base;
				OffsetRect(&missile_origin_rect[i],0,18 * store_missiles[i].missile_type);
			}

			// x1 slope x2 start pt
			x1[i] = finish_point[i].x - start_point.x;
			x2[i] = start_point.x;
			y1[i] = finish_point[i].y - start_point.y;
			y2[i] = start_point.y;
		}
		else
		{
			missile_place_rect[i].top = missile_place_rect[i].left = missile_place_rect[i].bottom = missile_place_rect[i].right = 0;
		}
	}

	play_sound(-1 * sound_num);

	// Now, at last, launch missile
	for (t = 0; t < num_steps; t++)
	{
		for (i = 0; i < 30; i++)
		{
			if (store_missiles[i].missile_type >= 0)
			{
				// Where place?
				temp_rect = missile_origin_base;
				OffsetRect(&temp_rect, -8 + x2[i] + (x1[i] * t) / num_steps, -8 + y2[i] + (y1[i] * t) / num_steps);

				// now adjust for different paths
				if (store_missiles[i].path_type == 1)
				{
					OffsetRect(&temp_rect, 0, -1 * (t * (num_steps - t)) / 100);
				}

				IntersectRect(&missile_place_rect[i], &temp_rect, &active_area_rect);

				// Now put terrain in temporary;
				rect_draw_some_item_bmp(terrain_screen_gworld, missile_place_rect[i], temp_gworld, missile_place_rect[i], 0, 0);
				// Now put in missile
				from_rect = missile_origin_rect[i];
				if (store_missiles[i].missile_type >= 7)
				{
					OffsetRect(&from_rect, 18 * (t % 8), 0);
				}
				rect_draw_some_item_bmp(missiles_gworld, from_rect, temp_gworld, temp_rect, 1, 0);
			}
		}

		// Now draw all missiles to screen
		for (i = 0; i < 30; i++)
		{
			if (store_missiles[i].missile_type >= 0)
			{
				to_rect = store_erase_rect[i];
				OffsetRect(&to_rect, current_terrain_ul.x, current_terrain_ul.y);
				rect_draw_some_item_bmp(terrain_screen_gworld, store_erase_rect[i],terrain_screen_gworld, to_rect, 0, 1);

				to_rect = missile_place_rect[i];
				store_erase_rect[i] = to_rect;
				OffsetRect(&to_rect, current_terrain_ul.x, current_terrain_ul.y);
				rect_draw_some_item_bmp(temp_gworld, missile_place_rect[i],temp_gworld, to_rect, 0, 1);
			}
		}
		if ((PSD[306][6] == 3) || ((PSD[306][6] == 1) && (t % 4 == 0)) || ((PSD[306][6] == 2) && (t % 3 == 0)))
		{
			Delay(1);
		}
		if ((cartoon_happening == TRUE) && (t % 3 == 0))
		{
			Delay(1);
		}
	}

	// Exit gracefully, and clean up screen
	for (i = 0; i < 30; i++)
	{
		store_missiles[i].missile_type = -1;
	}
	DisposeGWorld(temp_gworld);

	to_rect = ter_scrn_rect;
	OffsetRect(&to_rect,current_terrain_ul.x,current_terrain_ul.y);
	rect_draw_some_item_bmp(terrain_screen_gworld,ter_scrn_rect, terrain_screen_gworld,to_rect,0,1);

	while (t2 - t1 < pause_len + 40)
	{
		t2 = GetCurrentTime();
	}
	play_sound(99);
}

short get_missile_direction(POINT origin_point, POINT the_point)
{
	location store_dir = { 0,0 };
	short dir = 0;
	// To reuse legacy code, will renormalize the_point, which is missile destination,
	// so that origin_point is moved to (149,185) and the_point is moved in proportion
	the_point.x += 149 - origin_point.x;
	the_point.y += 185 - origin_point.y;

	if ((the_point.x < 135) & (the_point.y >= ((the_point.x * 34) / 10) - 293)
		& (the_point.y <= (-1 * ((the_point.x * 34) / 10) + 663)))
	{
		store_dir.x--;
	}
	if ((the_point.x > 163) & (the_point.y <= ((the_point.x * 34) / 10) - 350)
		& (the_point.y >= (-1 * ((the_point.x * 34) / 10) + 721)))
	{
		store_dir.x++;
	}
					
	if ((the_point.y < 167) & (the_point.y <= (the_point.x / 2) + 102)
		& (the_point.y <= (-1 * (the_point.x / 2) + 249)))
	{
		store_dir.y--;
	}
	if ((the_point.y > 203) & (the_point.y >= (the_point.x / 2) + 123)
		& (the_point.y >= (-1 * (the_point.x / 2) + 268)))
	{
		store_dir.y++;
	}
			
	switch (store_dir.y)
	{
	case 0:
		dir = 4 - 2 * (store_dir.x); break;
	case -1:
		dir = (store_dir.x == -1) ? 7 : store_dir.x; break;
	case 1:
		dir = 4 - store_dir.x; break;
	}	

	return dir;
}

void do_explosion_anim(short sound_num,short special_draw)
// sound_num currently ignored
// special_draw - 0 normal 1 - first half 2 - second half
{
	RECT temp_rect,to_rect,from_rect;
	RECT base_rect = {0,0,28,36},text_rect;
	short i,temp_val,temp_val2;
	location screen_ul;
	
	short t,cur_boom_type = 0; 
	POINT current_terrain_ul; 
	HBITMAP temp_gworld;
	short boom_type_sound[3] = {5,10,53};
	RECT ter_scrn_rect = {0,0,279,351};
	HDC hdc;
	COLORREF colors[5] = {RGB(0,0,0),RGB(255,0,0),RGB(128,0,0),RGB(0,160,0),RGB(255,255,255)};
	UINT c[5];
	HGDIOBJ store_bmp;
	DWORD t1,t2;
	DWORD snd_len[3] = {1500,1410,1100};

	t2 = t1 = GetCurrentTime();

	if ((have_boom == FALSE) || (boom_anim_active == FALSE))
	{
		boom_anim_active = FALSE;
		return;
	}
	
	for (i = 0; i < 30; i++)
	{
		if (store_booms[i].boom_type >= 0)
		{
			i = 50;
		}
	}
	if (i == 30)
	{
		return;
	}

	// initialize general data
	if ((in_startup_mode == TRUE) && (store_anim_type == 0))
	{
		current_terrain_ul.x = 306;
		current_terrain_ul.y = 5;
	}
	else if ((cartoon_happening == TRUE) && (store_anim_type > 0))
	{
		current_terrain_ul = store_anim_ul;
	}
	else
	{
		current_terrain_ul.x = current_terrain_ul.y = 5;
	}

	// make terrain_template contain current terrain all nicely
	draw_terrain(1);
	if (special_draw != 2)
	{
		to_rect = ter_scrn_rect;
		OffsetRect(&to_rect,current_terrain_ul.x, current_terrain_ul.y);
		rect_draw_some_item_bmp(terrain_screen_gworld,ter_scrn_rect,terrain_screen_gworld,to_rect,0,1);
	}

	// create and clip temporary anim template 
	temp_rect = ter_scrn_rect;
	temp_gworld = CreateCompatibleBitmap(main_dc,ter_scrn_rect.right,ter_scrn_rect.bottom);
	hdc = CreateCompatibleDC(main_dc);
	SelectPalette(hdc,hpal,0);
	SetBkMode(hdc,TRANSPARENT);
	SelectObject(hdc,small_bold_font);
	for (i = 0; i < 5; i++)
	{
		c[i] = GetNearestPaletteIndex(hpal, colors[i]);
	}
	SetTextColor(hdc,PALETTEINDEX(c[4]));
		
	// init missile paths
	screen_ul.x = center.x - 4; screen_ul.y = center.y - 4;
	for (i = 0; i < 30; i++)
	{
		if ((store_booms[i].boom_type >= 0) && (special_draw < 2))
		{
			cur_boom_type = store_booms[i].boom_type;
			explode_place_rect[i] = base_rect;
			OffsetRect(&explode_place_rect[i], 13 + 28 * (store_booms[i].dest.x - screen_ul.x) + store_booms[i].x_adj,
				13 + 36 * (store_booms[i].dest.y - screen_ul.y) + store_booms[i].y_adj);

			if ((store_booms[i].place_type == 1) && (special_draw < 2))
			{
				temp_val = rand_short(0, 50) - 25;
				temp_val2 = rand_short(0, 50) - 25;
				OffsetRect(&explode_place_rect[i], temp_val, temp_val2);
			}

			// eliminate stuff that's too gone. 
			IntersectRect(&temp_rect, &explode_place_rect[i], &ter_scrn_rect);

			if (EqualRect(&temp_rect, &explode_place_rect[i]) == FALSE)
			{
				store_booms[i].boom_type = -1;
			}
		}
		else if (special_draw < 2)
		{
			explode_place_rect[i].top = explode_place_rect[i].left = explode_place_rect[i].bottom = explode_place_rect[i].right = 0;
		}
	}

	//play_sound(-1 * sound_num);
	if (special_draw < 2)
	{
		play_sound(boom_type_sound[cur_boom_type]);
	}
	
	// Now, at last, do explosion
	for (t = (special_draw == 2) ? 6 : 0; t < ((special_draw == 1) ? 6 : 11); t++) // t goes up to 10 to make sure screen gets cleaned up
	{
		// First, lay terrain in temporary graphic area;
		for (i = 0; i < 30; i++)
		{
			if (store_booms[i].boom_type >= 0)
			{
				rect_draw_some_item_bmp(terrain_screen_gworld, explode_place_rect[i], temp_gworld, explode_place_rect[i], 0, 0);
			}
		}

		// Now put in explosions
		for (i = 0; i < 30; i++)
		{
			if (store_booms[i].boom_type >= 0)
			{
				if ((t + store_booms[i].offset >= 0) && (t + store_booms[i].offset <= 7))
				{
					from_rect = base_rect;
					OffsetRect(&from_rect, 28 * (t + store_booms[i].offset), 144 + 36 * (store_booms[i].boom_type));
					rect_draw_some_item_bmp(fields_gworld, from_rect, temp_gworld, explode_place_rect[i], 1, 0);

					if (store_booms[i].val_to_place > 0)
					{
						text_rect = explode_place_rect[i];
						text_rect.top += 2;
						text_rect.left -= 2;
						if (store_booms[i].val_to_place < 10)
						{
							text_rect.left += 4;
						}
						store_bmp = SelectObject(hdc, temp_gworld);
						win_draw_string(hdc, text_rect, std::format("{:d}", store_booms[i].val_to_place), 1, 12);
						SelectObject(hdc, store_bmp);
					}
				}
			}
		}

		// Now draw all missiles to screen
		for (i = 0; i < 30; i++)
		{
			if (store_booms[i].boom_type >= 0)
			{
				to_rect = explode_place_rect[i];
				OffsetRect(&to_rect, current_terrain_ul.x, current_terrain_ul.y);
				rect_draw_some_item_bmp(temp_gworld, explode_place_rect[i],
					temp_gworld, to_rect, 0, 1);
			}
		}

		//if (((PSD[306][6] == 1) && (t % 3 == 0)) || ((PSD[306][6] == 2) && (t % 2 == 0)))
		Delay(2 * (1 + PSD[306][6]));
		if (cartoon_happening == TRUE)
		{
			Delay(1);
		}
	}
		
	// Exit gracefully, and clean up screen
	for (i = 0; i < 30; i++)
	{
		if (special_draw != 1)
		{
			store_booms[i].boom_type = -1;
		}
	}
	SelectObject(hdc,store_bmp);
	DeleteObject(hdc);
	DisposeGWorld(temp_gworld);

	if (special_draw != 1)
	{
		while (t2 - t1 < snd_len[cur_boom_type] + 100)
		{
			t2 = GetCurrentTime();
		}
		play_sound(99);
	}
	//to_rect = terrain_screen_gworld->portRect;
	//OffsetRect(&to_rect,current_terrain_ul.h,current_terrain_ul.v);
	//rect_draw_some_item_bmp(terrain_screen_gworld,terrain_screen_gworld->portRect,
	//	terrain_screen_gworld,to_rect,0,1);
}

/*
shop_type:
0 - weapon shop
1 - armor shop
2 - misc shop
3 - healer
4 - food
5-9 - magic shop
10 - mage spells
11 - priest spells
12 alchemy
*/
void click_shop_rect(RECT area_rect)
{
	draw_shop_graphics(1,area_rect);
	if (play_sounds == TRUE)
	{
		play_sound(37);
	}
	else
	{
		Delay(5);
	}
	draw_shop_graphics(0,area_rect);

}

static const std::array cost_strs{ "Extremely Cheap","Very Reasonable","Pretty Average","Somewhat Pricey",
	"Expensive","Exorbitant","Utterly Ridiculous" };

const COLORREF c_colors[7]{ RGB(0,0,0),RGB(255,0,130),RGB(128,0,70),RGB(0,0,100),RGB(0,0,220), RGB(0,220,0),RGB(255,110,255) };
const RECT c_item_info_from{ 42,11,56,24 };
const RECT c_face_rect{ 6,6,38,38 };
const RECT c_title_rect{ 48,15,260,42 };
const RECT c_help_from{ 126,23,142,36 };
const RECT c_done_from{ 0,23,63,46 };
const short c_faces[13]{ 1,1,1,42,43, 1,1,1,1,1, 44,44,44 };
const RECT c_shopper_name{ 6,44,260,56 };

void draw_shop_graphics(short draw_mode,RECT clip_area_rect)
// mode 1 - drawing dark for button press
{
	RECT area_rect, from_rect;
	RECT dest_rect;
	short i,what_chosen;
	short current_pos;
	short cur_cost,what_magic_shop,what_magic_shop_item;
	item_record_type base_item;
	HDC hdc;
	UINT c[7];
	HGDIOBJ store_bmp;
	HGDIOBJ old_brush;
	
	hdc = CreateCompatibleDC(main_dc);
	SelectPalette(hdc,hpal,0);
	SetBkMode(hdc,TRANSPARENT);
	SelectObject(hdc,small_bold_font);
	for (i = 0; i < 7; i++)
	{
		c[i] = GetNearestPaletteIndex(hpal, c_colors[i]);
	}
	store_bmp = SelectObject(hdc,talk_gworld);

	if (draw_mode > 0)
	{
		IntersectClipRect(hdc,clip_area_rect.left,clip_area_rect.top,clip_area_rect.right,clip_area_rect.bottom);
	}

	area_rect = talk_area_rect;
	OffsetRect(&area_rect,-5,-5);

	old_brush = SelectObject(hdc,GetStockObject(NULL_BRUSH));
	Rectangle(hdc,area_rect.left,area_rect.top,area_rect.right,area_rect.bottom);  
	SelectObject(hdc,old_brush);
	InflateRect(&area_rect,-1,-1);
	SelectObject(hdc,store_bmp);
	if (draw_mode > 0)
	{
		paint_pattern_bmp(talk_gworld,clip_area_rect,3);
	}
	else
	{
		paint_pattern_bmp(talk_gworld, area_rect, 3);
	}

	SelectObject(hdc,talk_gworld);
	//old_brush = SelectObject(hdc,bg[1]);
	//Rectangle(hdc,area_rect.left,area_rect.top,area_rect.right,area_rect.bottom);
	//SelectObject(hdc,old_brush);
	
	old_brush = SelectObject(hdc,GetStockObject(NULL_BRUSH));
	Rectangle(hdc,shop_frame.left,shop_frame.top,shop_frame.right,shop_frame.bottom);
	SelectObject(hdc,old_brush);

	SelectObject(hdc,store_bmp);
	// Place store icon
	if (draw_mode == 0)
	{
		i = c_faces[store_shop_type];
		draw_dialog_graphic_bmp(talk_gworld, c_face_rect, 1000 + i, FALSE);
	}
	SelectObject(hdc,talk_gworld);

	// Place name of store and shopper name
	SelectObject(hdc,fantasy_font);
	SetTextColor(hdc,PALETTEINDEX(c[3]));
	dest_rect = c_title_rect;
	OffsetRect(&dest_rect,1,-5);
	win_draw_string(hdc,dest_rect,store_store_name,2,18);
	OffsetRect(&dest_rect,-1,-1);
	SetTextColor(hdc,PALETTEINDEX(c[4]));
	win_draw_string(hdc,dest_rect,store_store_name,2,18);
	SelectObject(hdc,small_bold_font);

	//TextFont(geneva_font_num);

	SetTextColor(hdc,PALETTEINDEX(c[3]));

	std::string cur_name;
	switch (store_shop_type)
	{
	case 3:		cur_name = std::format("Healing for {}.",adven[current_pc].name); break;
	case 10:	cur_name = std::format("Mage Spells for {}.",adven[current_pc].name); break;
	case 11:	cur_name = std::format("Priest Spells for {}.",adven[current_pc].name); break;
	case 12:	cur_name = "Buying Alchemy."; break;
	case 4:		cur_name = "Buying Food."; break;
	default:	cur_name = std::format("Shopping for {}.",adven[current_pc].name); break;
	}

	win_draw_string(hdc,c_shopper_name,cur_name, 2, 18);

	// Place help and done buttons
	SetTextColor(hdc,PALETTEINDEX(c[0]));
	SelectObject(hdc,store_bmp);
	talk_help_rect.right = talk_help_rect.left + c_help_from.right - c_help_from.left;
	talk_help_rect.bottom = talk_help_rect.top + c_help_from.bottom - c_help_from.top;
	rect_draw_some_item_bmp(dlg_buttons_gworld, c_help_from,talk_gworld,talk_help_rect,0,0);
	//talk_help_rect.right = talk_help_rect.left + c_help_from.right - c_help_from.left;
	//talk_help_rect.bottom = talk_help_rect.top + c_help_from.bottom - c_help_from.top;
	rect_draw_some_item_bmp(dlg_buttons_gworld,c_done_from,talk_gworld,shop_done_rect,0,0);
	SelectObject(hdc,talk_gworld);
	if (draw_mode == 0)
	{
		SetTextColor(hdc, PALETTEINDEX(c[0]));
	}
	else
	{
		SetTextColor(hdc, PALETTEINDEX(c[4]));
	}

	SelectObject(hdc,small_bold_font);

	// Place all the items
	for (i = 0; i < 8; i++)
	{
		current_pos = i + GetScrollPos(shop_sbar,SB_CTL);
		if (store_shop_items[current_pos] < 0)
		{
			break; // theoretically, this shouldn't happen
		}
		cur_cost = store_shop_costs[current_pos];
		what_chosen = store_shop_items[current_pos];
		SelectObject(hdc,store_bmp);
		std::string cur_info_str;
		switch (what_chosen / 100)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
			base_item = item_source.stored_item(what_chosen);
			base_item.item_properties = base_item.item_properties | 1;
			draw_dialog_graphic_bmp(talk_gworld, shopping_rects[i][2],1800 + base_item.graphic_num, FALSE);
			cur_name = base_item.full_name;
			cur_info_str = get_item_interesting_string(base_item);
			break;
		case 5:
			base_item = store_alchemy(what_chosen - 500);
			draw_dialog_graphic_bmp(talk_gworld, shopping_rects[i][2],1853, FALSE);//// all graphic nums
			cur_name = base_item.full_name;
			break;
		case 6:
			//base_item = food_types[what_chosen - 600];
			//draw_dialog_graphic_bmp(talk_gworld, shopping_rects[i][2],633, FALSE);
			//cur_name = base_item.full_name;
			//cur_info_str = get_item_interesting_string(base_item);
			break;
		case 7:
			what_chosen -= 700;
			draw_dialog_graphic_bmp(talk_gworld, shopping_rects[i][2],1879, FALSE);
			cur_name = heal_types[what_chosen];
			break;
		case 8:
			base_item = store_mage_spells(what_chosen - 800 - 30);
			draw_dialog_graphic_bmp(talk_gworld, shopping_rects[i][2],1800 + base_item.graphic_num, FALSE);
			cur_name = base_item.full_name;
			break;
		case 9:
			base_item = store_priest_spells(what_chosen - 900 - 30);
			draw_dialog_graphic_bmp(talk_gworld, shopping_rects[i][2],1853, FALSE);
			cur_name = base_item.full_name;
			break;
		default:
			what_magic_shop = (what_chosen / 1000) - 1;
			what_magic_shop_item = what_chosen % 1000;
			base_item = party.magic_store_items[what_magic_shop][what_magic_shop_item];
			base_item.item_properties = base_item.item_properties | 1;
			draw_dialog_graphic_bmp(talk_gworld, shopping_rects[i][2],1800 + base_item.graphic_num, FALSE);
			cur_name = base_item.full_name;
			cur_info_str = get_item_interesting_string(base_item);
			break;
		}

		from_rect = c_item_info_from;
		OffsetRect(&from_rect,0,1);
		shopping_rects[i][6].bottom = shopping_rects[i][6].top + (from_rect.bottom - from_rect.top);
		if ((store_shop_type != 3) && (store_shop_type != 4))
		{
			rect_draw_some_item_bmp(mixed_gworld, c_item_info_from, talk_gworld, shopping_rects[i][6], 1, 0);
		}
		SelectObject(hdc,talk_gworld);
		// Now draw item shopping_rects[i][7]
		// 0 - whole area, 1 - active area 2 - graphic 3 - item name
		// 4 - item cost 5 - item extra str  6 - item help button
		//TextSize(12);
		win_draw_string(hdc,shopping_rects[i][3],cur_name, 0, 12);
		win_draw_string(hdc,shopping_rects[i][4], std::format("Cost: {:d}", cur_cost), 0, 12);
		//TextSize(10);
		win_draw_string(hdc,shopping_rects[i][5],cur_info_str,0,12);
	}

	// Finally, cost info and help strs
	//TextSize(12);
	//TextSize(10);
	win_draw_string(hdc,bottom_help_rects[0], std::format("Prices here are {}.", cost_strs[store_cost_mult]), 0, 12);
	win_draw_string(hdc,bottom_help_rects[1],"Click on item name (or type 'a'-'h') to buy.",0,12);
	win_draw_string(hdc,bottom_help_rects[2],"Hit done button (or Esc.) to quit.",0,12);
	if ((store_shop_type != 3) && (store_shop_type != 4))
	{
		win_draw_string(hdc, bottom_help_rects[3], "'I' button brings up description.", 0, 12);
	}

	SelectObject(hdc,store_bmp);
	DeleteObject(hdc);

	refresh_shopping();
	ShowScrollBar(shop_sbar,SB_CTL,TRUE);
}

const RECT c_from_rects[4] = { {0,0,279,62},{0,62,253,352},{269,62,279,352},{0,352,279,415} };

void refresh_shopping()
{
	for (const auto& rect: c_from_rects)
	{
		rect_draw_some_item_bmp(talk_gworld,rect,talk_gworld, offset_rect(rect, 5, 5),0,1);
	}
}

void click_talk_rect(char *str_to_place,char *str_to_place2,RECT c_rect)
{
	place_talk_str(str_to_place,str_to_place2,1,c_rect);
	if (play_sounds == TRUE)
	{
		play_sound(37);
	}
	else
	{
		Delay(5);
	}
	place_talk_str(str_to_place,str_to_place2,0,c_rect);
}

const short c_mage_spell_cost[32]{
	150,200,150,1000,1200,400,300,200,
	200,250,500,1500,300,  250,125,150,
	400,450, 800,600,700,600,7500, 500,
	5000,3000,3500,4000,4000,4500,7000,5000
};

item_record_type store_mage_spells(short which_s) 
{
	if (which_s != boe_clamp(which_s, 0, 31))
	{
		which_s = 0;
	}
	item_record_type spell = { item_variety::GemStoneEtc,0, 0,0,0,0,0,0, 53,0,0,0,0,0, 0, 0,0, {0,0},"", "",0,0,0,0 };
	spell.item_level = which_s + 30;
	spell.value = c_mage_spell_cost[which_s];
	get_str(spell.full_name,38,which_s + 1);
	return spell;
}

const short c_priest_spell_cost[32]{
	100,150,75,400,200, 100,80,250,
	400,400,1200,600,300, 600,350,250,
	500,500,600,800, 1000,900,400,600,
	2500,2000,4500,4500,3000,3000,2000,2000
};

// which_s = 0 means that it returns first 4th level spell
item_record_type store_priest_spells(short which_s)
{
	if (which_s != boe_clamp(which_s, 0, 31))
	{
		which_s = 0;
	}
	item_record_type spell = { item_variety::GemStoneEtc,0, 0,0,0,0,0,0, 53,0,0,0,0,0, 0, 0,0, {0,0},"", "",0,0,0,0 };
	spell.item_level = which_s + 30;
	spell.value = c_priest_spell_cost[which_s];
	get_str(spell.full_name,38,which_s + 50);
	return spell;
}

const short c_alchemy_spell_value[20]{
	50,75,30,130,100,150, 200,200,300,250,300, 500,600,750,700,1000,10000,5000,7000,12000
};

item_record_type store_alchemy(short which_s)
{
	if (which_s != boe_clamp(which_s, 0, 19))
	{
		which_s = 0;
	}
	item_record_type spell = { item_variety::GemStoneEtc,0, 0,0,0,0,0,0, 53,0,0,0,0,0, 0, 0,0, {0,0},"", "",0,0,0,0 };
	spell.item_level = which_s;
	spell.value = c_alchemy_spell_value[which_s];
	get_str(spell.full_name,38,which_s + 100);
	return spell; 
}

std::string get_item_interesting_string(const item_record_type& item)
{
	if (is_property(item))
	{
		return "Not yours.";
	}

	if (!is_ident(item))
	{
		return {};
	}

	if (is_cursed(item))
	{
		return "Cursed item.";
	}

	std::string message;
	switch (item.variety)
	{
	case item_variety::OneHandedWeapon:
	case item_variety::TwoHandedWeapon:
	case item_variety::Arrows:
	case item_variety::ThrownMissile:
	case item_variety::Bolts:
	case item_variety::MissileWeapon:
		if (item.bonus != 0)
		{
			message = std::format("Damage: 1-{:d} + {:d}.", item.item_level, item.bonus);
		}
		else
		{
			message = std::format("Damage: 1-{:d}.", item.item_level);
		}
		break;
	case item_variety::Shield:
	case item_variety::Armor:
	case item_variety::Helm:
	case item_variety::Gloves:
	case item_variety::Shield2:
	case item_variety::Boots:
		message = std::format("Blocks {:d}-{:d} damage.",item.item_level + (item.protection > 0) ? 1 : 0, item.item_level + item.protection);
		break;
	case item_variety::Bow:
	case item_variety::Crossbow:
		message = std::format("Bonus : +{:d} to hit.",item.bonus);
		break;
	case item_variety::Gold:
		message = std::format("{:d} gold pieces.",item.item_level);
		break;
	case item_variety::Food:
		message = std::format("{:d} food.",item.item_level);
		break;
	case item_variety::WeaponPoison:
		message = std::format("Poison: Does {:d}-{:d} damage.",item.item_level,item.item_level * 6);
		break;
	default:
		if (item.charges > 0)
		{
			message = std::format("Uses: {:d}", item.charges);
		}
		break;
	}
	if (item.charges > 0)
	{
		message = std::format("Uses: {:d}", item.charges);
	}
	return message;
}

const RECT c_title_rect2{ 48,19,260,42 };
const COLORREF c_colors2[7]{ RGB(0,0,0),RGB(0,0,204),RGB(0,0,102),RGB(0,0,100),RGB(0,0,220),RGB(51,153,51),RGB(0,204,0) };

void place_talk_str(char *str_to_place,const char *str_to_place2,short color,RECT c_rect)
// color 0 - regular  1 - darker
{
	RECT area_rect;
	RECT dest_rect;
	short i,j,str_len,line_height = 18;
	char p_str[256],str[256],str_to_draw[256],str_to_draw2[256];
	short text_len[257],current_rect,store_last_word_break = 0,start_of_last_kept_word = -1;
	short last_line_break = 0,last_word_break = 0,on_what_line = 0,last_stored_word_break = 0;
	Boolean force_skip = FALSE;
	short face_to_draw;
	
	HDC hdc;
	UINT c[7];
	HGDIOBJ store_bmp;
	HGDIOBJ old_brush;
	HGDIOBJ store_font;

	hdc = CreateCompatibleDC(main_dc);
	SelectPalette(hdc,hpal,0);
	SetBkMode(hdc,TRANSPARENT);
	store_font = SelectObject(hdc,fantasy_font);
	for (i = 0; i < 7; i++)
	{
		c[i] = GetNearestPaletteIndex(hpal, c_colors2[i]);
	}
	store_bmp = SelectObject(hdc,talk_gworld);

	if (c_rect.right > 0)
	{
		IntersectClipRect(hdc,c_rect.left,c_rect.top,c_rect.right,c_rect.bottom);
	}

	area_rect = talk_area_rect;
	OffsetRect(&area_rect,-5,-5);

	old_brush = SelectObject(hdc,GetStockObject(NULL_BRUSH));
	Rectangle(hdc,area_rect.left,area_rect.top,area_rect.right,area_rect.bottom);
	SelectObject(hdc,old_brush);
	InflateRect(&area_rect,-1,-1);
	//old_brush = SelectObject(hdc,bg[1]);
	//Rectangle(hdc,area_rect.left,area_rect.top,area_rect.right,area_rect.bottom);
	//SelectObject(hdc,old_brush);
	SelectObject(hdc,store_bmp);
	if (c_rect.right > 0)
	{
		paint_pattern_bmp(talk_gworld,c_rect,3);
	}
	else
	{
		paint_pattern_bmp(talk_gworld, area_rect, 3);
	}

	// Put help button
	talk_help_rect.right = talk_help_rect.left + c_help_from.right - c_help_from.left;
	talk_help_rect.bottom = talk_help_rect.top + c_help_from.bottom - c_help_from.top;
	rect_draw_some_item_bmp(dlg_buttons_gworld, c_help_from,talk_gworld,talk_help_rect,0,0);

	// Place face of talkee
	if ((color == 0) && (c_rect.right == 0))
	{
		face_to_draw = scenario.monster(store_monst_type).default_facial_pic;
		if (store_talk_face_pic >= 0)
		{
			face_to_draw = store_talk_face_pic;
		}
		if (store_talk_face_pic >= 1000)
		{
			draw_dialog_graphic_bmp(talk_gworld, c_face_rect, 2400 + store_talk_face_pic - 1000, FALSE);
		}
		else
		{
				i = get_monst_picnum(store_monst_type);

				if (face_to_draw <= 0)
				{
					draw_dialog_graphic_bmp(talk_gworld, c_face_rect, 400 + i, FALSE);
				}
				else
				{
					draw_dialog_graphic_bmp(talk_gworld, c_face_rect, 1000 + face_to_draw, FALSE);
				}
		}
	}
	SelectObject(hdc,talk_gworld);

	// Place name oftalkee
	SetTextColor(hdc,PALETTEINDEX(c[3]));
	dest_rect = c_title_rect2;
	OffsetRect(&dest_rect,1,-5);
	win_draw_string(hdc,dest_rect,title_string,2,18);
	OffsetRect(&dest_rect,-1,-1);
	SetTextColor(hdc,PALETTEINDEX(c[4]));
	win_draw_string(hdc,dest_rect,title_string,2,18);

	// Place buttons at bottom.
	if (color == 0)
	{
		SetTextColor(hdc, PALETTEINDEX(c[5]));
	}
	else
	{
		SetTextColor(hdc, PALETTEINDEX(c[6]));
	}
	for (i = 0; i < 9; i++)
	{
		if ((talk_end_forced == FALSE) || (i == 6) || (i == 5))
		{
			win_draw_string(hdc, offset_rect(preset_words[i].word_rect, 0, -2), preset_words[i].word, 2, 18);
		}
	}
	// Place bulk of what said. Save words.
	//TextSize(14);
	if (color == 0)
	{
		for (i = 0; i < 50; i++)
		{
			store_words[i].word_rect.left = store_words[i].word_rect.right = 0;
		}
	}

	str_len = (short) strlen(str_to_place);
	if (str_len == 0)
	{
		format_to_buf(str_to_place,".");
	}
	strcpy(str,str_to_place);
	strcpy(p_str,str_to_place);
	c2p(p_str);
	for (i = 0; i < 257; i++)
	{
		text_len[i] = 0;
	}
	MeasureText(256,p_str,text_len,hdc);

	dest_rect = word_place_rect;

	current_rect = 0;

	if (color == 0)
	{
		SetTextColor(hdc, PALETTEINDEX(c[2]));
	}
	else
	{
		SetTextColor(hdc, PALETTEINDEX(c[1]));
	}
	MoveTo(dest_rect.left + 1 , dest_rect.top + 1 + line_height * on_what_line + 9);
	//for (i = 0;text_len[i] != text_len[i + 1], i < str_len;i++) {
	for (i = 0;i < str_len;i++)
	{
		if (((str[i] != 39) && ((str[i] < 65) || (str[i] > 122)) && ((str[i] < 48) || (str[i] > 57))) && (color == 0)) // New word, so set up a rect
		{
			if (((i - store_last_word_break >= 4) || (i >= str_len - 1)) 
				&& (i - last_stored_word_break >= 4) && (talk_end_forced == FALSE))
			{
				store_words[current_rect].word_rect.left = dest_rect.left + (text_len[store_last_word_break] - text_len[last_line_break]) - 2;
				store_words[current_rect].word_rect.right = dest_rect.left + (text_len[i + 1] - text_len[last_line_break]) - 1;
				store_words[current_rect].word_rect.top = dest_rect.top + 1 + line_height * on_what_line - 5;
				store_words[current_rect].word_rect.bottom = dest_rect.top + 1 + line_height * on_what_line + 13;

				if ((str[store_last_word_break] < 48) || (str[store_last_word_break] == 96)
					|| (str[store_last_word_break] > 122)
					|| ((str[store_last_word_break] >= 58) && (str[store_last_word_break] <= 64)))
				{
					store_last_word_break++;
				}

				// adjust for if this word will be scrolled down
				//if (((text_len[i] - text_len[last_line_break] > (dest_rect.right - dest_rect.left - 6)) 
		  		//	&& (last_word_break > last_line_break)) || (str[i] == '|')) {
		  		//	OffsetRect(&store_words[current_rect].word_rect,5 + -1 * store_words[current_rect].word_rect.left,line_height);
		  		//	}
				
				store_words[current_rect].word[0] = str[store_last_word_break];
				store_words[current_rect].word[1] = str[store_last_word_break + 1];
				store_words[current_rect].word[2] = str[store_last_word_break + 2];
				store_words[current_rect].word[3] = str[store_last_word_break + 3];
				store_words[current_rect].word[4] = 0;
				for (j = 0; j < 4; j++)
				{
					if ((store_words[current_rect].word[j] >= 65) && (store_words[current_rect].word[j] <= 90))
					{
						store_words[current_rect].word[j] += 32;
					}
				}
				if (scan_for_response(store_words[current_rect].word) < 0)
				{
					store_words[current_rect].word_rect.left = store_words[current_rect].word_rect.right = 0;
				}
				else
				{
					start_of_last_kept_word = store_last_word_break;
					if (current_rect < 49)
					{
						current_rect++;
					}
					//FrameRect(&store_words[current_rect].word_rect);
				}
				last_stored_word_break = i + 1;
			}
		}
		if (((text_len[i] - text_len[last_line_break] > (dest_rect.right - dest_rect.left - 6)) 
		  && (last_word_break > last_line_break)) || (str[i] == '|') || (i == str_len - 1))
		{
			if (str[i] == '|')
			{
				str[i] = ' ';
		 		force_skip = TRUE;
			}
	 		store_last_word_break = last_word_break;
			if (i == str_len - 1)
			{
				last_word_break = i + 2;
			}
			format_to_buf(str_to_draw,"                                                         ");
			strncpy(str_to_draw, str + last_line_break,(size_t) (last_word_break - last_line_break - 1));
			format_to_buf(str_to_draw2," {}",str_to_draw);
			str_to_draw2[0] = (char) strlen(str_to_draw);
			MoveToDrawString((str_to_draw2 + 1),hdc);
			on_what_line++;
			MoveTo(dest_rect.left + 1 , dest_rect.top + 1 + line_height * on_what_line + 9);
			last_line_break = last_word_break;
			if (force_skip == TRUE)
			{
				force_skip = FALSE;
				i++;
				last_line_break++;
				last_word_break++;
			}
			if ((start_of_last_kept_word >= last_line_break) && (current_rect > 0))
			{
				//SysBeep(2);
				OffsetRect(&store_words[current_rect - 1].word_rect,5 + -1 * store_words[current_rect - 1].word_rect.left,line_height);
			}
		}
		if (str[i] == ' ') // New word
		{
			store_last_word_break = last_word_break = i + 1;
		}
		if (on_what_line == 17)
		{
			i = 10000;
		}
	}

	// Now for string 2
	str_len = (short) strlen(str_to_place2);
	start_of_last_kept_word = -1;

	if (str_len > 0)
	{
		strcpy(str,str_to_place2);
		strcpy(p_str,str_to_place2);
		c2p(p_str);
		for (i = 0; i < 257; i++)
			text_len[i]= 0;
		MeasureText(256,p_str,text_len,hdc);

		last_line_break = store_last_word_break = last_word_break = last_stored_word_break = 0;
		MoveTo(dest_rect.left + 1 , dest_rect.top + 1 + line_height * on_what_line + 9);
		//for (i = 0;text_len[i] != text_len[i + 1], i < str_len;i++)
		for (i = 0;i < str_len;i++)
		{
			if (((str[i] != 39) && ((str[i] < 65) || (str[i] > 122)) && ((str[i] < 48) || (str[i] > 57))) && (color == 0))  // New word, so set up a rect
			{
				if (((i - store_last_word_break >= 4) || (i >= str_len - 1)) && (i - last_stored_word_break >= 4) && (talk_end_forced == FALSE))
				{
					store_words[current_rect].word_rect.left = dest_rect.left + (text_len[store_last_word_break] - text_len[last_line_break]) - 2;
					store_words[current_rect].word_rect.right = dest_rect.left + (text_len[i + 1] - text_len[last_line_break]) - 1;
					store_words[current_rect].word_rect.top = dest_rect.top + 1 + line_height * on_what_line - 5;
					store_words[current_rect].word_rect.bottom = dest_rect.top + 1 + line_height * on_what_line + 13;

					if ((str[store_last_word_break] < 48) || (str[store_last_word_break] == 96)
						|| (str[store_last_word_break] > 122)
						|| ((str[store_last_word_break] >= 58) && (str[store_last_word_break] <= 64))
						)
					{
						store_last_word_break++;
					}

					// adjust for if this word will be scrolled down
					//if (((text_len[i] - text_len[last_line_break] > (dest_rect.right - dest_rect.left - 6))
					//	&& (last_word_break > last_line_break)) || (str[i] == '|')) {
					//	OffsetRect(&store_words[current_rect].word_rect,5 + -1 * store_words[current_rect].word_rect.left,line_height);
					//	}
					store_words[current_rect].word[0] = str[store_last_word_break];
					store_words[current_rect].word[1] = str[store_last_word_break + 1];
					store_words[current_rect].word[2] = str[store_last_word_break + 2];
					store_words[current_rect].word[3] = str[store_last_word_break + 3];
					store_words[current_rect].word[4] = 0;
					for (j = 0; j < 4; j++)
					{
						if ((store_words[current_rect].word[j] >= 65) && (store_words[current_rect].word[j] <= 90))
						{
							store_words[current_rect].word[j] += 32;
						}
					}
					if (scan_for_response(store_words[current_rect].word) < 0)
					{
						store_words[current_rect].word_rect.left = store_words[current_rect].word_rect.right = 0;
					}
					else
					{
						start_of_last_kept_word = store_last_word_break;
						if (current_rect < 49)
							current_rect++;

						//FrameRect(&store_words[current_rect].word_rect);
					}
					last_stored_word_break = i + 1;
				}
			}
			if (((text_len[i] - text_len[last_line_break] > (dest_rect.right - dest_rect.left - 6))
				&& (last_word_break > last_line_break)) || (str[i] == '|') || (i == str_len - 1)
				)
			{
				if (str[i] == '|')
				{
					str[i] = ' ';
					force_skip = TRUE;
				}
				store_last_word_break = last_word_break;
				if (i == str_len - 1)
				{
					last_word_break = i + 2;
				}
				format_to_buf(str_to_draw,"                                                         ");
				strncpy(str_to_draw, str + last_line_break,(size_t) (last_word_break - last_line_break - 1));
				format_to_buf(str_to_draw2," {}",str_to_draw);
				str_to_draw2[0] = (char) strlen(str_to_draw);
				MoveToDrawString((str_to_draw2 + 1),hdc);
				on_what_line++;
				MoveTo(dest_rect.left + 1 , dest_rect.top + 1 + line_height * on_what_line + 9);
				last_line_break = last_word_break;
				if (force_skip == TRUE)
				{
					force_skip = FALSE;
					i++;
					last_line_break++;
					last_word_break++;
				}
				if ((start_of_last_kept_word >= last_line_break) && (current_rect > 0))
				{
	 				::OffsetRect(&store_words[current_rect - 1].word_rect,5 + -1 * store_words[current_rect - 1].word_rect.left,line_height);				
				}
			}
			if (str[i] == ' ') // New word
			{
				store_last_word_break = last_word_break = i + 1;
			}
			if (on_what_line == 17)
			{
				i = 10000;
			}
		}
	}

	::SelectObject(hdc,store_bmp);
	::SelectObject(hdc,store_font);
	::DeleteObject(hdc);
	
	refresh_talking();
	
	// Clean up strings
	for (i = 0; i < 50; i++)
	{
		for (j = 0; j < 4; j++)
		{
			if ((store_words[current_rect].word[j] >= 65) && (store_words[current_rect].word[j] <= 90))
			{
				store_words[current_rect].word[j] += 32;
			}
		}
	}
}

void refresh_talking()
{
	rect_draw_some_item_bmp(talk_gworld, { 0,0,279,415 },talk_gworld,talk_area_rect,0,1);
}

short scan_for_response(char *str)
// returns -1 if no go
{
	for (short i = 0; i < 60; i++) // 60 response in each bunch
	{
		if (
				(talking.talk_nodes[i].personality != -1)
				&&
				(
					(talking.talk_nodes[i].personality == store_personality)
					||
					(talking.talk_nodes[i].personality == -2)
				)
				&&
				(
					(
						(str[0] == talking.talk_nodes[i].link1[0])
						&&
						(str[1] == talking.talk_nodes[i].link1[1])
						&&
						(str[2] == talking.talk_nodes[i].link1[2])
						&&
						(str[3] == talking.talk_nodes[i].link1[3])
					)
					||
					(
						(str[0] == talking.talk_nodes[i].link2[0])
						&&
						(str[1] == talking.talk_nodes[i].link2[1])
						&&
						(str[2] == talking.talk_nodes[i].link2[2])
						&&
						(str[3] == talking.talk_nodes[i].link2[3])
					)
				)
			)
		{
			return i;
		}
	}
	return -1;
}
