#include <Windows.h>
#include <array>
#include <cstdio>

#include "global.h"
#include "gutils.h"
#include "graphics.h"
#include "newgraph.h"
#include "fileio.h"
#include "items.h"
#include "monster.h"
#include "town.h"
#include "combat.h"
#include "party.h"
#include "text.h"
#include "exlsound.h"
#include "fields.h"
#include "locutils.h"
#include "dlogtool.h"
#include "dlogtool_helpers.hpp"
#include "infodlgs.h"
#include "graphutl.h"
#include "graphutl_helpers.hpp"
#include "boe/utility.hpp"
#include "game_globals.hpp"

extern HBITMAP mixed_gworld,spec_scen_g;
extern current_town_type c_town;
extern party_record_type party;
extern Adventurers adven;
extern town_item_list t_i;
extern short overall_mode,which_combat_type,current_pc,combat_active_pc;
extern location center;
extern HWND mainPtr;
extern short monst_target[T_M]; // 0-5 target that pc   6 - no target  100 + x - target monster x
extern unsigned char combat_terrain[64][64];
extern outdoor_record_type outdoors[2][2];
extern unsigned char misc_i[64][64];
extern short store_current_pc,current_ground;
extern short store_pre_shop_mode,store_pre_talk_mode;
extern location monster_targs[T_M];
extern Boolean modeless_exists[18],belt_present;
extern HWND modeless_dialogs[18];
extern unsigned char out[96][96];
extern unsigned char out_e[96][96];
extern unsigned char sfx[64][64];
extern stored_items_list_type stored_items[3];
extern big_tr_type  t_d;
extern short town_size[3];
extern short town_type;
extern setup_save_type setup_save;
extern Boolean web,crate,barrel,fire_barrier,force_barrier,quickfire,force_wall,fire_wall,antimagic,scloud,ice_wall,blade_wall;
extern location pc_pos[6];
extern short last_attacked[6], pc_dir[6], pc_parry[6];
extern const location hor_vert_place[14];
extern const location diag_place[14];
extern short terrain_pic[256];
extern std::array< short, 20 > special_queue;
extern HPALETTE hpal;
extern HDC main_dc;
extern HINSTANCE store_hInstance;
extern stored_town_maps_type town_maps,town_maps2;
extern piles_of_stuff_dumping_type data_store;
extern short ulx,uly;
extern HFONT small_bold_font;

// Globals
unsigned char map_graphic_placed[8][64]; // keeps track of what's been filled on map
HBITMAP map_gworld;
HBRUSH bg[14];
HBRUSH map_brush[25];
HBITMAP map_bitmap[25];

// statics
static DLGPROC map_proc = NULL;
// extra devices for maps
static HBRUSH hbrush[6] = { NULL, NULL, NULL, NULL, NULL, NULL };
static HPEN hpen[6];
static RECT store_map_window_rect{ 0,0,0,0 };
static Boolean need_map_full_refresh = TRUE;
static short town_force = 200;
static location town_force_loc;
static Boolean kludge_force_full_refresh = FALSE;

// const vars
static const COLORREF map_colors[6]{
	RGB(0,0,0),RGB(63,223,95),RGB(0,0,255),
	RGB(191,0,191),RGB(255,0,0),RGB(204,204,204)
};

static const unsigned char map_pats[256] = {1,1,2,2,2,7,7,7,7,7, ////
						7,7,7,7,7,7,7,7,3,3,
						3,3,3,3,3,3,3,3,3,3,
						3,3,5,5,5,5,5,5,5,5,
						5,5,5,5,5,5,4,4,4,4,
						4,4,4,4,4,4,4,4,4,0, // 50
						0,0,0,0,0,0,0,24,24,24,
						16,16,25,25,0,0,0,0,18,8,
						8,9,2,15,15,10,10,10,6,0,
						0,0,0,0,0,0,0,0,0,0,
						19,0,0,0,0,0,0,0,0,0, // 100
						23,0,0,0,0,0,0,0,0,0,
						0,0,0,11,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,12,0,0, // 150
						0,0,0,13,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,
						0,18,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0, // 200
						0,0,0,0,0,17,17,0,17,17,
						17,17,17,17,17,17,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0,
						0,0,0,0,0,0,0,0,0,0, 
						0,0,0,0,0,0};// 250
static const unsigned char anim_map_pats[18] = {14,0,0,0,22, 0,0,21,20,21, 20,0,0,0,0, 0,0,0};

// 0 grass 1 cave 2 mntn 3 bridge 4 cave bridge 5 rubble cave 6 cave tree 7 cave mush
// 8 cave swamp 9 surfac eorcks 10 surf swamp 11 surface woods 12 s. shrub 13 stalags
static const short general_types[260] = { 1,1,0,0,0,1,1,1,1,1,
	1,1,1,1,1,1,1,1,2,2,
	2,2,2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,2,2,2,2,
	2,2,2,2,2,2,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,// 50
	0,3,3,3,3,3,3,5,5,5,
	6,6,7,7,1,1,8,9,10,11,
	10,11,12,13,13,9,9,9,1,1,
	1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,// 100
	1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,// 150
	1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,
	1,0,1,1,1,1,1,1,1,0,
	0,0,0,0,1,0,0,0,0,0,
	0,0,1,0,2,0,0,1,1,1,// 200
	1,0,2,1,1,0,1,1,1,1,
	1,1,0,0,0,0,1,0,1,1,
	1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1 };// 250
static const short ter_base[14] = { 2,0,36,50,71, 0,0,0,0,2, 2,2,2,0 };
static const short ground_type[14] = { 2,0,36,50,71, 0,0,0,0,2,	2,2,2,0 };

static const location special_ter_locs[15] = { {11,10},{11,14},{10,20},{11,26},{9,30},
							{15,19},{23,19},{19,29},{20,11},{28,16},
							{28,24},{27,19},{27,29},{15,28},{19,19} };
static const unsigned char cave_pillar[4][4] = { {0,14,11,1},{14,19,20,11},{17,18,21,8},{1,17,8,0} };
static const unsigned char mntn_pillar[4][4] = { {37,29,27,36},{29,33,34,27},{31,32,35,25},{36,31,25,37} };
static const unsigned char surf_lake[4][4] = { {56,55,54,3},{57,50,61,54},{58,51,59,53},{3,4,58,52} };
static const unsigned char cave_lake[4][4] = { {93,96,71,71},{96,71,71,71},{71,71,71,96},{71,71,71,96} };
static const short terrain_odds[14][10] = { {3,80,4,40,115,20,114,10,112,1},
						{1,50,93,25,94,5,98,10,95,1},
						{37,20,0,0,0,0,0,0,0,0},
						{64,3,63,1,0,0,0,0,0,0},
						{74,1,0,0,0,0,0,0,0,0},
						{84,700,97,30,98,20,92,4,95,1},
						{93,280,91,300,92,270,95,7,98,10},
						{1,800,93,600,94,10,92,10,95,4},
						{1,700,96,200,95,100,92,10,112,5},
						{3,600,87,90,110,20,114,6,113,2},
						{3,200,4,400,111,250,0,0,0,0},
						{3,200,4,300,112,50,113,60,114,100},
						{3,100,4,250,115,120,114,30,112,2},
						{1,25,84,15,98,300,97,280,0,0} }; // ter then odds then ter then odds ...

void force_town_enter(short which_town,location where_start)
{
	town_force = which_town;
	town_force_loc = where_start;
}

void start_town_mode(short which_town, short entry_dir)
//short entry_dir; // if 9, go to forced
{
	short i,m,n;
	short j,k,town_number;
	short at_which_save_slot,former_town;
	Boolean monsters_loaded = FALSE,town_toast = FALSE;
	location loc;
	unsigned char temp;
	Boolean play_town_sound = FALSE;


		if (town_force < 200)
		which_town = town_force;
		else if (PSD[304][9] == 0) {
			play_town_sound = TRUE;

			}

	former_town = town_number = which_town;

	if ((town_number < 0) || (town_number >= scenario.num_towns())) {
		give_error("The scenario tried to put you into a town that doesn't exist.","",0);
		return;
		}	
		

	// Now adjust town number as necessary.
	for (i = 0; i < 10; i++)
		if ((scenario.town_to_add_to(i) >= 0) && (scenario.town_to_add_to(i) < 200) &&
			(town_number == scenario.town_to_add_to(i)) &&
			(sd_legit(scenario.flag_to_add_to_town(i,0), scenario.flag_to_add_to_town(i,1)) == TRUE)) {
			former_town = town_number;
			town_number += PSD[scenario.flag_to_add_to_town(i,0)][scenario.flag_to_add_to_town(i,1)];
			// Now update horses & boats
			for (i = 0; i < 30; i++) 	
				if ((party.horses[i].exists == TRUE) && (party.horses[i].which_town == former_town))
					party.horses[i].which_town = town_number;
			for (i = 0; i < 30; i++) 	
				if ((party.boats[i].exists == TRUE) && (party.boats[i].which_town == former_town))
					party.boats[i].which_town = town_number;
		}

		
		
	if ((town_number < 0) || (town_number >= scenario.num_towns())) {
		give_error("The scenario tried to put you into a town that doesn't exist.","",0);
		return;
		}
	overall_mode = 1;

	load_town(town_number,0,0,NULL);

	c_town.town_num = town_number;

	if (play_town_sound == TRUE) {
		if (c_town.town.lighting > 0)
			play_sound(95);
			else play_sound(16);
			
		}



	belt_present = FALSE;
	// Set up map, using stored map
	for (i = 0; i < town_size[town_type]; i++)
		for (j = 0; j < town_size[town_type]; j++) {
			c_town.explored[i][j] = 0;
			if (c_town.town_num < 100) {
				if (town_maps.town_maps[c_town.town_num][i / 8][j] & (char)(s_pow(2,i % 8)))
					make_explored(i,j);
				}
				else {
					if (town_maps2.town_maps[c_town.town_num - 100][i / 8][j] & (char)(s_pow(2,i % 8)))
						make_explored(i,j);
					}
			if (t_d.terrain[i][j] == 0)
				current_ground = 0;
				else if (t_d.terrain[i][j] == 2)
				current_ground = 2;
			if ((scenario.ter_type(t_d.terrain[i][j]).special >= terrain_special::ConveyorNorth) &&
				(scenario.ter_type(t_d.terrain[i][j]).special <= terrain_special::ConveyorWest))
					belt_present = TRUE;	
		}
	c_town.hostile = 0;
	c_town.monst.which_town = town_number;
	c_town.monst.friendly = 0;

	at_which_save_slot = party.at_which_save_slot;
	
	for (i = 0; i < 4; i++)
		if (town_number == party.creature_save[i].which_town) {
			c_town.monst = party.creature_save[i];
			monsters_loaded = TRUE;
						
			for (j = 0; j < T_M; j++) {
				if (loc_off_act_area(c_town.monst.dudes[j].m_loc) == TRUE)
					c_town.monst.dudes[j].active = 0;
				if (c_town.monst.dudes[j].active == 2)
					c_town.monst.dudes[j].active = 1;
				c_town.monst.dudes[j].m_loc = t_d.creatures[j].start_loc;
				c_town.monst.dudes[j].m_d.health = c_town.monst.dudes[j].m_d.m_health;
				c_town.monst.dudes[j].m_d.mp = c_town.monst.dudes[j].m_d.max_mp; 			
				c_town.monst.dudes[j].m_d.morale = c_town.monst.dudes[j].m_d.m_morale;
				for (k = 0; k < 15; k++)
					c_town.monst.dudes[j].m_d.mstatus[k] = 0;
				if (c_town.monst.dudes[j].summoned > 0)
					c_town.monst.dudes[j].active = 0;
				monst_target[j] = 6;
				}
			
			// Now, travelling NPCs might have arrived. Go through and put them in.
			// These should have protected status (i.e. spec1 >= 200, spec1 <= 204)
			for (j = 0; j < T_M; j++) 
				if ((c_town.monst.dudes[j].monst_start.time_flag >= 4) &&
					(c_town.monst.dudes[j].monst_start.time_flag <= 6)) {
						if ((((short) (party.age / 1000) % 3) + 4) != c_town.monst.dudes[j].monst_start.time_flag)
							c_town.monst.dudes[j].active = 0;
							else {
								c_town.monst.dudes[j].active = 1;
								c_town.monst.dudes[j].monst_start.spec_enc_code = 0;
								// Now remove time flag so it doesn't geet reappearing
								c_town.monst.dudes[j].monst_start.time_flag = 0;
								c_town.monst.dudes[j].m_loc = t_d.creatures[j].start_loc;
								c_town.monst.dudes[j].m_d.health = c_town.monst.dudes[j].m_d.m_health;
								}
						}
					
			
			
			for (j = 0; j < town_size[town_type]; j++)
				for (k = 0; k < town_size[town_type]; k++) { // now load in saved setup, 
						// except that barrels and crates resotre to orig locs
					temp = setup_save.setup[i][j][k] & 231;

					misc_i[j][k] = (misc_i[j][k] & 24) | temp;//setup_save.setup[i][j][k];
					}
			}

	if (monsters_loaded == FALSE) {
				for (i = 0; i < T_M; i++)
					if (t_d.creatures[i].number == 0) {
						c_town.monst.dudes[i].active = 0;
						c_town.monst.dudes[i].number = 0;
						c_town.monst.dudes[i].monst_start.time_flag = 0;
						c_town.monst.dudes[i].m_loc.x = 80;
						}
					else {
						// First set up the values.
						monst_target[i] = 6;
						c_town.monst.dudes[i].active = 1;
						c_town.monst.dudes[i].number = t_d.creatures[i].number;
						c_town.monst.dudes[i].attitude = t_d.creatures[i].start_attitude;
						c_town.monst.dudes[i].m_loc = t_d.creatures[i].start_loc;
						c_town.monst.dudes[i].mobile = t_d.creatures[i].mobile;
						c_town.monst.dudes[i].m_d = return_monster_template(c_town.monst.dudes[i].number);

						c_town.monst.dudes[i].summoned = 0;
						c_town.monst.dudes[i].monst_start = t_d.creatures[i];

						if (c_town.monst.dudes[i].monst_start.spec_enc_code > 0)
							c_town.monst.dudes[i].active = 0;

						// Now, if necessary, fry the monster.
						switch (c_town.monst.dudes[i].monst_start.time_flag) {
							case 1:
								if (day_reached(c_town.monst.dudes[i].monst_start.monster_time,
								  c_town.monst.dudes[i].monst_start.time_code) == FALSE)
									c_town.monst.dudes[i].active = 0;
								break;
							case 2:
								if (day_reached(c_town.monst.dudes[i].monst_start.monster_time,
								  c_town.monst.dudes[i].monst_start.time_code) == TRUE)
									c_town.monst.dudes[i].active = 0;
								break;
							case 3:
								// unused
								break;
							case 4: case 5: case 6:
								if ((((short) (party.age / 1000) % 3) + 4) != c_town.monst.dudes[i].monst_start.time_flag) {
									c_town.monst.dudes[i].active = 0;
									c_town.monst.dudes[i].monst_start.spec_enc_code = 50;
									}
									else {
										c_town.monst.dudes[i].active = 1;
										// Now remove time flag so it doesn't keep reappearing
										c_town.monst.dudes[i].monst_start.time_flag = 0;
										}
								break;
							case 7:
								if (calc_day() < party.key_times[c_town.monst.dudes[i].monst_start.time_code])
									c_town.monst.dudes[i].active = 0;
								break;

							case 8:
								if (calc_day() > party.key_times[c_town.monst.dudes[i].monst_start.time_code])
									c_town.monst.dudes[i].active = 0;
								break;
							case 9:
								if (c_town.town.town_chop_time > 0)
									if (day_reached(c_town.town.town_chop_time,c_town.town.town_chop_key) == TRUE) {
										c_town.monst.dudes[i].active += 10;
										break;
										}
								c_town.monst.dudes[i].active = 0;
								break;
							case 0:
								break;
							default:
								//ASB("ERROR! Odd character data.");
								//print_nums(0,i,c_town.monst.dudes[i].monst_start.time_flag);
								break;							}
					}
		}

	// Now munch all large monsters that are misplaced
	// only large monsters, as some smaller monsters are intentionally placed
	// where they cannot be
	for (i = 0; i < T_M; i++) {
		if (c_town.monst.dudes[i].active > 0)
			if (((c_town.monst.dudes[i].m_d.x_width > 1) || (c_town.monst.dudes[i].m_d.y_width > 1)) && 
				(monst_can_be_there(c_town.monst.dudes[i].m_loc,i) == FALSE))
				c_town.monst.dudes[i].active = 0;			
		}
	// Thrash town?
	if (party.m_killed[c_town.town_num] > c_town.town.max_num_monst) {
		town_toast = TRUE;
		add_string_to_buf("Area has been cleaned out.");
		}
	if (c_town.town.town_chop_time > 0) {
		if (day_reached(c_town.town.town_chop_time,c_town.town.town_chop_key) == TRUE){
			//add_string_to_buf("Area has been abandoned.");
			for (i = 0; i < T_M; i++)
				if ((c_town.monst.dudes[i].active > 0) && (c_town.monst.dudes[i].active < 10) && 
				(c_town.monst.dudes[i].attitude % 2 == 1))
					c_town.monst.dudes[i].active += 10;
			town_toast = TRUE;
			}
		}
	if (town_toast == TRUE) {
			for (i = 0; i < T_M; i++)
				if (c_town.monst.dudes[i].active >= 10)
					c_town.monst.dudes[i].active -= 10;
					else c_town.monst.dudes[i].active = 0;			
		}
	handle_town_specials(town_number, (short) town_toast,(entry_dir < 9) ? c_town.town.start_locs[entry_dir] : town_force_loc);	
	// Flush excess doomguards and viscous goos
	for (i = 0; i < T_M; i++)
		if ((c_town.monst.dudes[i].m_d.spec_skill == 12) &&
			(c_town.monst.dudes[i].number != t_d.creatures[i].number))
				c_town.monst.dudes[i].active = 0;
		
		
	quickfire = FALSE;
	crate = FALSE;
	barrel = FALSE;
	web = FALSE;
	fire_barrier = FALSE;
	force_barrier = FALSE;
	// Set up field booleans, correct for doors
			for (j = 0; j < town_size[town_type]; j++)
				for (k = 0; k < town_size[town_type]; k++) {
					loc.x = j; loc.y = k;
					if (is_door(loc)) 
						misc_i[j][k] = misc_i[j][k] & 3;

					// Set up field booleans
					if (is_web(j,k))
						web = TRUE;
					if (is_crate(j,k))
						crate = TRUE;
					if (is_barrel(j,k))
						barrel = TRUE;
					if (is_fire_barrier(j,k))
						fire_barrier = TRUE;
					if (is_force_barrier(j,k))
						force_barrier = TRUE;
					if (is_quickfire(j,k))
						quickfire = TRUE;
					}
		
	// Set up items, maybe place items already there
	for (i = 0; i < NUM_TOWN_ITEMS; i++)
		t_i.items[i] = item_record_type{};
	
							
	for (j = 0; j < 3; j++)
		if (scenario.store_item_town(j) == town_number) {
			for (i = 0; i < NUM_TOWN_ITEMS; i++)
				t_i.items[i] = stored_items[j].items[i];		
			}				

	//print_nums(party.item_taken[c_town.town_num][0],party.item_taken[c_town.town_num][1],party.item_taken[c_town.town_num][2]);
	//print_nums(party.item_taken[c_town.town_num][3],party.item_taken[c_town.town_num][4],party.item_taken[c_town.town_num][5]);
	//print_nums(party.item_taken[c_town.town_num][6],party.item_taken[c_town.town_num][7],999);
	for (i = 0; i < 64; i++)
		if ((c_town.town.preset_items[i].item_code >= 0) 
		 && (((party.item_taken[c_town.town_num][i / 8] & s_pow(2,i % 8)) == 0) ||
		 (c_town.town.preset_items[i].always_there == TRUE))) {
			for (j = 0; j < NUM_TOWN_ITEMS; j++) 
			
				// place the preset item, if party hasn't gotten it already
				if (t_i.items[j].variety == item_variety::None) {
					t_i.items[j] = item_source.stored_item(c_town.town.preset_items[i].item_code);
					t_i.items[j].item_loc = c_town.town.preset_items[i].item_loc;

					// Not use the items data flags, starting with forcing an ability
					if (c_town.town.preset_items[i].ability >= 0) {
		 				switch (t_i.items[j].variety) {
							case item_variety::Gold: case item_variety::Food: // If gold or food, this value is amount
								if (c_town.town.preset_items[i].ability > 0)
									t_i.items[j].item_level = c_town.town.preset_items[i].ability;
								break;
							default:
								// CC: This was not present in the original source
								break;
		 					}
		 				}
		 				
					if (c_town.town.preset_items[i].property == TRUE)
		 				t_i.items[j].item_properties = t_i.items[j].item_properties | 2;
		 			if (town_toast == TRUE) 
		 				t_i.items[j].item_properties = t_i.items[j].item_properties & 253;
		 			if (c_town.town.preset_items[i].contained == TRUE)
		 				t_i.items[j].item_properties = t_i.items[j].item_properties | 8;
		 			t_i.items[j].is_special = i + 1;
		 			
					j = NUM_TOWN_ITEMS;
					}
		 	}

							
	for (i = 0; i < T_M; i++) 
		if (loc_off_act_area(c_town.monst.dudes[i].m_loc) == TRUE)
			c_town.monst.dudes[i].active = 0;
	for (i = 0; i < NUM_TOWN_ITEMS; i++)
		if (loc_off_act_area(t_i.items[i].item_loc) == TRUE)
				t_i.items[i].variety = item_variety::None;
				
	// Clean out unwanted monsters
	for (i = 0; i < T_M; i++) 
		if (sd_legit(c_town.monst.dudes[i].monst_start.spec1,c_town.monst.dudes[i].monst_start.spec2) == TRUE) {
			if (party.stuff_done[c_town.monst.dudes[i].monst_start.spec1][c_town.monst.dudes[i].monst_start.spec2] > 0)
				c_town.monst.dudes[i].active = 0;
			}
		
	erase_specials();
	make_town_trim(0);

	load_area_graphics();
							
	
	c_town.p_loc = (entry_dir < 9) ? c_town.town.start_locs[entry_dir] : town_force_loc;
	center = c_town.p_loc;
	if (party.in_boat >= 0) {
		party.boats[party.in_boat].which_town = which_town;
		party.boats[party.in_boat].boat_loc = c_town.p_loc;
		}
	if (party.in_horse >= 0) {
		party.horses[party.in_horse].which_town = which_town;
		party.horses[party.in_horse].horse_loc = c_town.p_loc;
		}
		
	
	// End flying
	if (party.stuff_done[305][1] > 0) {
		party.stuff_done[305][1] = 0;	
		add_string_to_buf("You land, and enter.             ");
		}

	// No hostile monsters present.
	party.stuff_done[305][9] = 0;
		
	add_string_to_buf("Now entering:");
	add_string_to_buf("   {:<30.30s} ",data_store.town_strs[0]);

	// clear entry space, and check exploration
	misc_i[c_town.p_loc.x][c_town.p_loc.y] = misc_i[c_town.p_loc.x][c_town.p_loc.y] & 3;
	update_explored(c_town.p_loc);

	// If a PC dead, drop his items
	for (m = 0; m < 6; m++)
		for (n = 0; n < 24; n++)
			if ((adven[m].main_status != status_type::Normal) && (adven[m].items[n].variety != item_variety::None)) {
				place_item(adven[m].items[n],c_town.p_loc,TRUE);
				adven[m].items[n].variety = item_variety::None;
				}

	for (i = 0; i < T_M; i++)
		{monster_targs[i].x = 0;  monster_targs[i].y = 0;}


	//// check horses
	for (i = 0; i < 30; i++) {
		if ((scenario.boats(i).which_town >= 0) && (scenario.boats(i).boat_loc.x >= 0)) {
			if (party.boats[i].exists == FALSE) {
				party.boats[i] = scenario.boats(i);
				party.boats[i].exists = TRUE;
				}
			}
		if ((scenario.horses(i).which_town >= 0) && (scenario.horses(i).horse_loc.x >= 0)) {
			if (party.horses[i].exists == FALSE) {
				party.horses[i] = scenario.horses(i);
				party.horses[i].exists = TRUE;
				}
			}
		}

	// Place correct graphics
	redraw_screen(0);


	clear_map();
	reset_item_max();
	town_force = 200;
}


location end_town_mode(short switching_level,location destination)  // returns new party location
{
	location to_return;
	short i,j,k;
	Boolean data_saved = FALSE,combat_end = FALSE;

	if (is_combat())
		combat_end = TRUE;
	
	if (overall_mode == 1) {
			for (i = 0; i < 4; i++)
				if (party.creature_save[i].which_town == c_town.town_num) {
					party.creature_save[i] = c_town.monst;
					for (j = 0; j < town_size[town_type]; j++)
						for (k = 0; k < town_size[town_type]; k++)
							setup_save.setup[i][j][k] = misc_i[j][k];
					data_saved = TRUE;
					}
			if (data_saved == FALSE) {
				party.creature_save[party.at_which_save_slot] = c_town.monst;
				for (j = 0; j < town_size[town_type]; j++)
					for (k = 0; k < town_size[town_type]; k++)
						setup_save.setup[party.at_which_save_slot][j][k] = misc_i[j][k];
				party.at_which_save_slot = (party.at_which_save_slot == 3) ? 0 : party.at_which_save_slot + 1;
				} 

	// Store items, if necessary 
		for (j = 0; j < 3; j++)
			if (scenario.store_item_town(j) == c_town.town_num) {
			for (i = 0; i < NUM_TOWN_ITEMS; i++)
				if ((t_i.items[i].variety != item_variety::None) && (t_i.items[i].is_special == 0) &&
				((t_i.items[i].item_loc.x >= scenario.store_item_rects(j).left) &&
				 (t_i.items[i].item_loc.x <= scenario.store_item_rects(j).right) &&
				 (t_i.items[i].item_loc.y >= scenario.store_item_rects(j).top) &&
				 (t_i.items[i].item_loc.y <= scenario.store_item_rects(j).bottom)) ) {
				 	stored_items[j].items[i] = t_i.items[i];
				 	}
				 	else stored_items[j].items[i].variety = item_variety::None;
				}
				
			
		// Clean up special data, just in case
		for (i = 0; i < T_M; i++) {
			c_town.monst.dudes[i].monst_start.spec1 = 0;
			c_town.monst.dudes[i].monst_start.spec2 = 0;
			}

		// Now store map
			for (i = 0; i < town_size[town_type]; i++)
				for (j = 0; j < town_size[town_type]; j++)
					if (is_explored(i,j)) {
						if (c_town.town_num < 100)
							town_maps.town_maps[c_town.town_num][i / 8][j] = town_maps.town_maps[c_town.town_num][i / 8][j] |
								(char) (s_pow(2,i % 8));
							else town_maps2.town_maps[c_town.town_num - 100][i / 8][j] = town_maps.town_maps[c_town.town_num][i / 8][j] |
								(char) (s_pow(2,i % 8));
					}

		
		to_return = party.p_loc;

		for (i = 0; i < 30; i++)
	 		if ((party.party_event_timers[i] > 0) && (party.global_or_town[i] == 1))
				party.party_event_timers[i] = 0;
		}
		
			
				// Check for exit specials, if leaving town
	if (switching_level == 0) {
		to_return = party.p_loc;

		if (is_town()) {
			if (destination.x <= c_town.town.in_town_rect.left) {
				if (c_town.town.exit_locs[1].x > 0) 
					to_return = local_to_global(c_town.town.exit_locs[1]);
					else to_return.x--;
				party.p_loc = to_return; party.p_loc.x++;
				handle_leave_town_specials(c_town.town_num, c_town.town.exit_specs[1],destination) ;
				}
			else if (destination.x >= c_town.town.in_town_rect.right) {
				if (c_town.town.exit_locs[3].x > 0) 
					to_return = local_to_global(c_town.town.exit_locs[3]);
					else to_return.x++;
				party.p_loc = to_return; party.p_loc.x--;
				handle_leave_town_specials(c_town.town_num, c_town.town.exit_specs[3],destination) ;
				}
			else if (destination.y <= c_town.town.in_town_rect.top) {
				if (c_town.town.exit_locs[0].x > 0) 
					to_return = local_to_global(c_town.town.exit_locs[0]);
					else to_return.y--;
				party.p_loc = to_return; party.p_loc.y++;
				handle_leave_town_specials(c_town.town_num, c_town.town.exit_specs[0],destination) ;
				}
			else if (destination.y >= c_town.town.in_town_rect.bottom) {
				if (c_town.town.exit_locs[2].x > 0) 
					to_return = local_to_global(c_town.town.exit_locs[2]);
					else to_return.y++;			
				party.p_loc = to_return; party.p_loc.y--;
				handle_leave_town_specials(c_town.town_num, c_town.town.exit_specs[2],destination) ;
				}

			}
		}

	if (switching_level == 0) {
			fix_boats();
			overall_mode = 0;

			erase_out_specials();

			load_area_graphics();

		party.stuff_done[305][0] = 0;
			for (i = 0; i < 6; i++)
				for (j = 0; j < 15; j++)
					if ((j != 2) && (j != 7) && (j != 9))
						adven[i].status[j] = 0;
	

		update_explored(to_return);
			redraw_screen(0);

		}


		

	if (combat_end == FALSE)
		clear_map();
	
	return to_return;
}


void handle_town_specials(short town_number, short entry_dir,location start_loc) 
{
	if (entry_dir > 0)
		special_queue[0] = c_town.town.spec_on_entry_if_dead;
		else special_queue[0] = c_town.town.spec_on_entry;
}

void handle_leave_town_specials(short town_number, short which_spec,location start_loc) 
{
	//run_special(6,2,which_spec,start_loc,&s1,&s2,&s3);
	special_queue[1] = which_spec;
}

Boolean abil_exists(short abil) // use when outdoors
{
	for (short i = 0; i < 6; i++)
		for (short j = 0; j < 24; j++)
			if ((adven[i].items[j].variety != item_variety::None) && (adven[i].items[j].ability == abil))
				return TRUE;
	for (short i = 0; i < 3; i++)
		for (short j = 0; j < NUM_TOWN_ITEMS; j++)
			if ((stored_items[i].items[j].variety != item_variety::None) && (stored_items[i].items[j].ability == abil))
				return TRUE;

	return FALSE;
}

void start_town_combat(short direction)
{
	create_town_combat_terrain();

	place_party(direction);

	if (current_pc == 6)
	{
		for (short i = 0; i < 6; i++)
		{
			if (adven[i].main_status == status_type::Normal)
			{
				current_pc = i;
				i = 6;
			}
		}
	}
	center = pc_pos[current_pc];

	which_combat_type = 1;
	overall_mode = 10;

	combat_active_pc = 6;
	for (auto& tar: monst_target)
		tar = 6;

	for (short i = 0; i < 6; i++) {
		last_attacked[i] = T_M + 10;
		pc_parry[i] = 0;
		pc_dir[i] = direction;
		adven[current_pc].direction = direction;
		if (adven[i].main_status == status_type::Normal)
			update_explored(pc_pos[i]);
		}

	store_current_pc = current_pc;
	current_pc = 0;
	set_pc_moves();
	pick_next_pc();
	center = pc_pos[current_pc];
	draw_buttons(0);
	put_pc_screen();
	set_stat_window(current_pc);
	give_help(48,49,0);

}

short end_town_combat()
{
	short num_tries = 0;
	short r1 = rand_short(0,5);
	while ((adven[r1].main_status != status_type::Normal) && (num_tries++ < 1000))
	{
		r1 = rand_short(0, 5);
	}
	c_town.p_loc = pc_pos[r1];
	overall_mode = 1;
	current_pc = store_current_pc;
	if (adven[current_pc].main_status != status_type::Normal)
	{
		current_pc = first_active_pc();
	}
	for (auto& pcp: pc_parry)
	{
		pcp = 0;
	}
	return pc_dir[r1];
}

void place_party(short direction)
{
	Boolean spot_ok[14] = {TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE,TRUE};
	location pos_locs[14];
	location check_loc;
	short x_adj,y_adj,how_many_ok = 1,where_in_a = 0;
	
	for (short i = 0; i < 14; i++)
	{
		check_loc = c_town.p_loc;
		if (direction % 4 < 2)
			x_adj = ((direction % 2 == 0) ? hor_vert_place[i].x : diag_place[i].x);
		else
			x_adj = ((direction % 2 == 0) ? hor_vert_place[i].y : diag_place[i].y);
		if (direction % 2 == 0)
			x_adj = (direction < 4) ? x_adj : -1 * x_adj;
		else
			x_adj = ((direction == 1) || (direction == 7)) ? -1 * x_adj : x_adj;
		check_loc.x -= x_adj;
		if (direction % 4 < 2)
			y_adj = ((direction % 2 == 0) ? hor_vert_place[i].y : diag_place[i].y);
		else
			y_adj = ((direction % 2 == 0) ? hor_vert_place[i].x : diag_place[i].x);
		if (direction % 2 == 0)
			y_adj = ((direction > 1) && (direction < 6)) ? y_adj : -1 * y_adj;
		else
			y_adj = ((direction == 3) || (direction == 1)) ? -1 * y_adj : y_adj;
		
		check_loc.y -= y_adj;		
		pos_locs[i] = check_loc;
		if ((loc_off_act_area(check_loc) == FALSE) &&
			is_not_blocked(check_loc) && is_not_special(check_loc) && (get_obscurity(check_loc.x,check_loc.y) == 0)
			&& (can_see(c_town.p_loc,check_loc,1) < 1))
		{
			spot_ok[i] = TRUE;
			how_many_ok += (i > 1) ? 1 : 0;
		}
		else
		{
			spot_ok[i] = FALSE;
		}

		if (i == 0)
		{
			spot_ok[i] = TRUE;
		}
	}
	for(short i = 0; i < 6; ++i)
	{
		if (adven[i].main_status == status_type::Normal)
		{
			if (how_many_ok == 1)
			{
				pc_pos[i] = pos_locs[where_in_a];
			}
			else
			{
				pc_pos[i] = pos_locs[where_in_a];
				if (how_many_ok > 1)
				{
					++where_in_a;
				}
				how_many_ok--;
//				if (how_many_ok > 1) {
				while (spot_ok[where_in_a] == FALSE)
				{
					++where_in_a;
				}
//				}
			}
		}
	}
}

void create_town_combat_terrain()
{
	for (size_t where_x = 0; where_x < town_size[town_type]; ++where_x)
	{
		for (size_t where_y = 0; where_y < town_size[town_type]; ++where_y)
		{
			combat_terrain[where_x][where_y] = t_d.terrain[where_x][where_y];
		}
	}
}

void create_out_combat_terrain(short type,short num_walls,short spec_code)
// spec_code is encounter's spec_code
{
	short ter_type = scenario.ter_type(type).picture;
	if (ter_type > 260)
		ter_type = 1;
	else
		ter_type = general_types[ter_type];

	for (short i = 0; i < 48; ++i)
	{
		for (short j = 0; j < 48; ++j)
		{
			c_town.explored[i][j] = 0;
			misc_i[i][j] = 0;
			sfx[i][j] = 0;
			if ((j <= 8) || (j >= 35) || (i <= 8) || (i >= 35))
			{
				t_d.terrain[i][j] = 90;
			}
			else
			{
				t_d.terrain[i][j] = ter_base[ter_type];
			}
		}
	}

	for (short i = 0; i < 48; ++i)
		for (short j = 0; j < 48; ++j)
			for (short k = 0; k < 5; ++k)
				if ((t_d.terrain[i][j] != 90) && (rand_short(1,1000) < terrain_odds[ter_type][k * 2 + 1]))
					t_d.terrain[i][j] = terrain_odds[ter_type][k * 2];
	
	t_d.terrain[0][0] = ter_base[ter_type];
	
	if ((ter_type == 3) || (ter_type == 4) )
	{
		t_d.terrain[0][0] = 83;
		for (short i = 15; i < 26; ++i)
			for (short j = 9; j < 35; ++j)
				t_d.terrain[i][j] = 83;
	}

	if ((type >= 79) && (type <= 81))
	{
		t_d.terrain[0][0] = 82;
		for (short i = 19; i < 23; ++i)
			for (short j = 9; j < 35; ++j)
				t_d.terrain[i][j] = 82;
	}

	// Now place special lakes, etc.
	location stuff_ul;
	if (ter_type == 2)
		for (short i = 0; i < 15; ++i)
			if (rand_short(0,5) == 1)
			{
				stuff_ul = special_ter_locs[i];
				for (short j = 0; j < 4; ++j)
					for (short k = 0; k < 4; ++k)
						t_d.terrain[stuff_ul.x + j][stuff_ul.y + k] = mntn_pillar[k][j];
			}
	if (t_d.terrain[0][0] == 0)
		for (short i = 0; i < 15; ++i)
			if (rand_short(0,25) == 1)
			{
				stuff_ul = special_ter_locs[i];
				for (short j = 0; j < 4; ++j)
					for (short k = 0; k < 4; ++k)
						t_d.terrain[stuff_ul.x + j][stuff_ul.y + k] = cave_pillar[k][j];
			}
	if (t_d.terrain[0][0] == 0)
		for (short i = 0; i < 15; ++i)
			if (rand_short(0,40) == 1)
			{
				stuff_ul = special_ter_locs[i];
				for (short j = 0; j < 4; ++j)
					for (short k = 0; k < 4; ++k)
						t_d.terrain[stuff_ul.x + j][stuff_ul.y + k] = cave_lake[k][j];
			}
	if (t_d.terrain[0][0] == 2)
		for (short i = 0; i < 15; ++i)
			if (rand_short(0,40) == 1)
			{
				stuff_ul = special_ter_locs[i];
				for (short j = 0; j < 4; ++j)
					for (short k = 0; k < 4; ++k)
						t_d.terrain[stuff_ul.x + j][stuff_ul.y + k] = surf_lake[k][j];
			}

	if (ground_type[ter_type] == 0)
	{
		for (short i = 0; i < num_walls; ++i)
		{
			const short r1 = rand_short(0,3);
			for (short j = 9; j < 35; ++j)
			{
				switch (r1)
				{
				case 0:
					t_d.terrain[j][8] = 6;
					break;
				case 1:
					t_d.terrain[8][j] = 9;
					break;
				case 2:
					t_d.terrain[j][35] = 12;
					break;
				case 3:
					t_d.terrain[32][j] = 15;
					break;
				}
			}
		}
		if ((t_d.terrain[17][8] == 6) && (t_d.terrain[8][20] == 9))
			t_d.terrain[8][8] = 21;
		if ((t_d.terrain[32][20] == 15) && (t_d.terrain[17][35] == 12))
			t_d.terrain[32][35] = 19;	
		if ((t_d.terrain[17][8] == 6) && (t_d.terrain[32][20] == 15))
			t_d.terrain[32][8] = 32;
		if ((t_d.terrain[8][20] == 9) && (t_d.terrain[17][35] == 12))
			t_d.terrain[8][35] = 20;	
	}
	if (ground_type[ter_type] == 36)
	{
		for (short i = 0; i < num_walls; ++i)
		{
			const short r1 = rand_short(0,3);
			for (short j = 9; j < 35; ++j)
			{
				switch (r1)
				{
				case 0:
					t_d.terrain[j][8] = 24;
					break;
				case 1:
					t_d.terrain[8][j] = 26;
					break;
				case 2:
					t_d.terrain[j][35] = 28;
					break;
				case 3:
					t_d.terrain[32][j] = 30;
					break;
				}
			}
		}
		if ((t_d.terrain[17][8] == 6) && (t_d.terrain[8][20] == 9))
			t_d.terrain[8][8] = 35;
		if ((t_d.terrain[32][20] == 15) && (t_d.terrain[17][35] == 12))
			t_d.terrain[32][35] = 33;	
		if ((t_d.terrain[17][8] == 6) && (t_d.terrain[32][20] == 15))
			t_d.terrain[32][8] = 32;
		if ((t_d.terrain[8][20] == 9) && (t_d.terrain[17][35] == 12))
			t_d.terrain[8][35] = 34;	
	}

	for (short i = 0; i < 48; ++i)
		for (short j = 0; j < 48; ++j)
			combat_terrain[i][j] = t_d.terrain[i][j];
			
	make_town_trim(1);
}


void elim_monst(unsigned char which,short spec_a,short spec_b)
{
	if (party.stuff_done[spec_a][spec_b] > 0)
	{
		for (short i = 0; i < T_M; ++i)
		{
			if (c_town.monst.dudes[i].number == which)
			{
				c_town.monst.dudes[i].active = 0;
			}
		}
	}
}


void dump_gold(short print_mes)
//short print_mes; // 0 - no 1 - yes
{
	// Mildly kludgy gold check
	if (party.gold > 30000)
	{
		party.gold = 30000;
		if (print_mes == 1)
		{
			put_pc_screen();
			add_string_to_buf("Excess gold dropped.            ");
			print_buf();
		}
	}
	if (party.food > 25000)
	{
		party.food = 25000;
		if (print_mes == 1)
		{
			put_pc_screen();
			add_string_to_buf("Excess food dropped.            ");
			print_buf();
		}
	}
}


void pick_lock(location where,short pc_num)
{
	const auto which_item{ pc_has_abil_equip(adven[pc_num],161) };
	if (which_item == 24)
	{
		add_string_to_buf("  Need lockpick equipped.        ");
		return;
	}

	const auto& terrain{ scenario.ter_type(t_d.terrain[where.x][where.y]) };
	if ((terrain.special < terrain_special::UnlockableTerrain) || (terrain.special > terrain_special::UnlockableOrBashable))
	{
		add_string_to_buf("  Wrong terrain type.           ");
		return;
	}

	short r1 = rand_short(0,100) - 5 * pc_stat_adj(adven[pc_num], skill::Dexterity) + c_town.difficulty * 7
	 - 5 * adven[pc_num].skills[skill::Lockpicking] - adven[pc_num].items[which_item].ability_strength * 7;

	// Nimble?
	if (adven[pc_num].traits[trait::NimbleFingers] == FALSE)
	{
		r1 -= 8;
	}

	if (pc_has_abil_equip(adven[pc_num], 42) < 24)
	{
		r1 -= 12;
	}

	const short unlock_adjust = terrain.flag2;
	if ((unlock_adjust >= 5) || (r1 > (unlock_adjust * 15 + 30)))
	{
		add_string_to_buf("  Didn't work.                ");
		const bool will_break = (rand_short(0, 100) + adven[pc_num].items[which_item].ability_strength * 7) < 75;
		if (will_break)
		{
			add_string_to_buf("  Pick breaks.                ");
			remove_charge(pc_num,which_item);
		}
		play_sound(41);
	}
	else
	{
		add_string_to_buf("  Door unlocked.                ");
		play_sound(9);
		t_d.terrain[where.x][where.y] = terrain.flag1;
	}
}


void bash_door(location where,short pc_num)
{
	const auto& terrain{ scenario.ter_type(t_d.terrain[where.x][where.y]) };
	if ((terrain.special < terrain_special::UnlockableTerrain) || (terrain.special > terrain_special::UnlockableOrBashable))
	{
		add_string_to_buf("  Wrong terrain type.           ");
		return;
	}

	const short unlock_adjust = terrain.flag2;
	const short r1 = rand_short(0, 100) - 15 * pc_stat_adj(adven[pc_num], skill::Strength) + c_town.difficulty * 4;
	if ((unlock_adjust >= 5) || (r1 > (unlock_adjust * 15 + 40)) || (terrain.special != terrain_special::UnlockableOrBashable))
	{
		add_string_to_buf("  Didn't work.                ");
		damage_pc(pc_num,rand_short(1,4), damage_type::Unblockable,-1);
	}
	else
	{
		add_string_to_buf("  Lock breaks.                ");
		play_sound(9);
		t_d.terrain[where.x][where.y] = terrain.flag1;
	}
}


void erase_specials()
{
	if (is_combat() && (which_combat_type == 0))
	{
		return;
	}
	if (is_not_town() && (is_not_combat()))
	{
		return;
	}

	for (short k = 0; k < 50; ++k)
	{
		if (c_town.town.spec_id[k] >= 0)
		{
			const special_node_type& sn{ c_town.town.specials[c_town.town.spec_id[k]] };
			if ((sd_legit(sn.sd1, sn.sd2) == TRUE) && (PSD[sn.sd1][sn.sd2] == 250))
			{
				const location& where = c_town.town.special_locs[k];
				if ((where.x != 100) && ((where.x > town_size[town_type]) || (where.y > town_size[town_type])
					|| (where.x < 0) || (where.y < 0)))
				{
					SysBeep(2);
					add_string_to_buf("Town corrupt. Problem fixed.");
					print_nums(where.x,where.y,k);
					c_town.town.special_locs[k].x = 0;
				}				
				if (where.x != 100)
				{
					switch (scenario.ter_type(t_d.terrain[where.x][where.y]).picture)
					{
					case 207: t_d.terrain[where.x][where.y] = 0; break;
					case 208: t_d.terrain[where.x][where.y] = 170; break;
					case 209: t_d.terrain[where.x][where.y] = 210; break;
					case 210: t_d.terrain[where.x][where.y] = 217; break;
					case 211: t_d.terrain[where.x][where.y] = 2; break;
					case 212: t_d.terrain[where.x][where.y] = 36; break;
					}
					take_special(where.x,where.y);
				}
			}
		}
	}
}

void erase_out_specials()
{
	for (short k = 0; k < 2; k++)
	{
		for (short l = 0; l < 2; l++)
		{
			if (quadrant_legal(k, l) == TRUE)
			{
				for (short m = 0; m < 8; m++)
				{
					if ((outdoors[k][l].exit_dests[m] >= 0) &&
						(outdoors[k][l].exit_locs[m].x == boe_clamp(outdoors[k][l].exit_locs[m].x, 0, 47)) &&
						(outdoors[k][l].exit_locs[m].y == boe_clamp(outdoors[k][l].exit_locs[m].y, 0, 47)))
					{
						if (party.can_find_town[outdoors[k][l].exit_dests[m]] == 0)
						{
							out[48 * k + outdoors[k][l].exit_locs[m].x][48 * l + outdoors[k][l].exit_locs[m].y] =
								scenario.ter_type(outdoors[k][l].terrain[outdoors[k][l].exit_locs[m].x][outdoors[k][l].exit_locs[m].y]).flag1;
							//exit_g_type[out[48 * k + outdoors[k][l].exit_locs[m].x][48 * l + outdoors[k][l].exit_locs[m].y] - 217];
						}
						else if (party.can_find_town[outdoors[k][l].exit_dests[m]] > 0)
						{
							out[48 * k + outdoors[k][l].exit_locs[m].x][48 * l + outdoors[k][l].exit_locs[m].y] =
								outdoors[k][l].terrain[outdoors[k][l].exit_locs[m].x][outdoors[k][l].exit_locs[m].y];
						}
					}
				}
			}
		}
	}

	for (short i = 0; i < 2; i++)
	{
		for (short j = 0; j < 2; j++)
		{
			if (quadrant_legal(i, j) == TRUE)
			{
				for (short k = 0; k < 18; k++)
				{
					if (outdoors[i][j].special_id[k] >= 0)
					{
						const special_node_type& sn = outdoors[i][j].specials[outdoors[i][j].special_id[k]];
						if ((sd_legit(sn.sd1, sn.sd2) == TRUE) && (PSD[sn.sd1][sn.sd2] == 250))
						{
							const location& where = outdoors[i][j].special_locs[k];
							if (where.x != 100)
							{
								if ((where.x > 48) || (where.y > 48)
									|| (where.x < 0) || (where.y < 0))
								{
									SysBeep(2);
									add_string_to_buf("Outdoor section corrupt. Problem fixed.");
									outdoors[i][j].special_id[k] = -1;
								}

								switch (scenario.ter_type(outdoors[i][j].terrain[where.x][where.y]).picture)
								{
								case 207: out[48 * i + where.x][48 * j + where.y] = 0; break;
								case 208: out[48 * i + where.x][48 * j + where.y] = 170; break;
								case 209: out[48 * i + where.x][48 * j + where.y] = 210; break;
								case 210: out[48 * i + where.x][48 * j + where.y] = 217; break;
								case 211: out[48 * i + where.x][48 * j + where.y] = 2; break;
								case 212: out[48 * i + where.x][48 * j + where.y] = 36; break;
								}
							}
						}
					}
				}
			}
		}
	}
}

// returns id # of special at where, or 40 if there is none.
short get_town_spec_id(location where)
{
	short i = 0;
	while ((not_same_point(c_town.town.special_locs[i], where)) && (i < 40))
	{
		++i;
	}
	return i;
}


void clear_map()
{
	const RECT map_world_rect = {0,0,384,384};
	HDC hdc = ::CreateCompatibleDC(main_dc);
	::SelectPalette(hdc,hpal,0);
	HGDIOBJ old_bmp = ::SelectObject(hdc, map_gworld);
	HGDIOBJ old_pen = ::SelectObject(hdc, GetStockObject(BLACK_PEN));
	HGDIOBJ old_brush = ::SelectObject(hdc, GetStockObject(BLACK_BRUSH));
	::Rectangle(hdc, map_world_rect.left, map_world_rect.top, map_world_rect.right, map_world_rect.bottom);
	(void)::SelectObject(hdc, old_pen);
	(void)::SelectObject(hdc, old_brush);
	(void)::SelectObject(hdc, old_bmp);
	::DeleteDC(hdc);
	draw_map(modeless_dialogs[5],10);
}


const RECT c_map_world_rect{ 0,0,321,321 };
const RECT c_whole_map_win_rect{ 0,0,400,400 };
const RECT c_dlogpicrect{ 6,6,42,42 };
const RECT c_orig_draw_rect{ 0,0,6,6 };
const RECT c_tiny_rect{ 0,0,32,32 };
const RECT c_big_rect{ 0,0,64,64 };

void draw_map (HWND the_dialog, short the_item)
//short the_item; // Being sneaky - if this gets value of 5, this is not a full restore -
				// just update near party, if it gets 11, blank out middle and leave
				// No redrawing in gworld
				// If a 10, do a regular full restore
				// Also, can get a 5 even when the window is not up, so have to make
				// sure dialog exists before accessing it.
{
	Boolean draw_surroundings = FALSE;
	if (the_item == 4)
	{
		draw_surroundings = TRUE;
		the_item = 5;
	}

	if (kludge_force_full_refresh == TRUE)
	{
		draw_surroundings = TRUE;
	}

	if ((modeless_exists[5] == FALSE) && (the_item == 5) && (need_map_full_refresh == TRUE))
	{
		return;
	}

	if ((modeless_exists[5] == FALSE) && (the_item == 10))
	{
		need_map_full_refresh = TRUE;
		return;
	}

	if ((modeless_exists[5] == TRUE) && (the_item != 11) && (need_map_full_refresh == TRUE))
	{
		need_map_full_refresh = FALSE;
		the_item = 10;
	}

	if (the_item == 10)
	{
		for (short i = 0; i < 8; i++)
			for (short j = 0; j < 64; j++)
				map_graphic_placed[i][j] = 0;
	}

	// view rect is rect that is visible, redraw rect is area to redraw now
	// area_to_draw_from is final draw from rect
	// area_to_draw_on is final draw to rect
	// extern short store_pre_shop_mode,store_pre_talk_mode;
	short total_size = 48; // if full redraw, use this to figure out everything
	RECT view_rect{ 0,0,48,48 }; // RECTangle visible in view screen
	RECT redraw_rect{ 0,0,48,48 };
	if (is_out() || (is_combat() && (which_combat_type == 0)) ||
		((overall_mode == 20) && (store_pre_talk_mode == 0)) ||
		((overall_mode == 21) && (store_pre_shop_mode == 0)))
	{
		view_rect.left = boe_clamp(party.loc_in_sec.x - 20,0,8);
		view_rect.right = view_rect.left + 40;
		view_rect.top = boe_clamp(party.loc_in_sec.y - 20,0,8);
		view_rect.bottom = view_rect.top + 40;
		redraw_rect = view_rect;
	}
	else
	{
		switch (town_type)
		{
		case 0:
			view_rect.left = boe_clamp(c_town.p_loc.x - 20,0,24);
			view_rect.right = view_rect.left + 40;
			view_rect.top = boe_clamp(c_town.p_loc.y - 20,0,24);
			view_rect.bottom = view_rect.top + 40;
			if (the_item == 5)
				redraw_rect = view_rect;
				else redraw_rect = c_big_rect;
			total_size = 64;
			break;
		case 1:
			view_rect.left = boe_clamp(c_town.p_loc.x - 20,0,8);
			view_rect.right = view_rect.left + 40;
			view_rect.top = boe_clamp(c_town.p_loc.y - 20,0,8);
			view_rect.bottom = view_rect.top + 40;
			redraw_rect = view_rect;
			break;
		case 2:
			view_rect = c_tiny_rect;
			redraw_rect = view_rect;
			//InsetRect(&area_to_draw_to,48,48);
			total_size = 32;
			break;
		}
	}

	const RECT area_to_draw_on{ offset_rect({47,29,287,269}, 0,-23) };
	RECT area_to_draw_from;
	short small_adj = 0;
	if (is_out() || (is_combat() && (which_combat_type == 0)) ||
		((overall_mode == 20) && (store_pre_talk_mode == 0)) ||
		((overall_mode == 21) && (store_pre_shop_mode == 0)) ||
		((is_town() || is_combat()) && (town_type != 2))
		)
	{
		area_to_draw_from = view_rect;	
		area_to_draw_from.left *= 6;
		area_to_draw_from.right *= 6;
		area_to_draw_from.top *= 6;
		area_to_draw_from.bottom *= 6;
	}
	else
	{
		area_to_draw_from = area_to_draw_on;
		OffsetRect(&area_to_draw_from,-1 * area_to_draw_from.left,-1 * area_to_draw_from.top);
		small_adj = 0;
	}

	Boolean draw_pcs = TRUE;
	if (is_combat())
	{
		draw_pcs = FALSE;
	}

	// make map pens
	if (hbrush[0] == NULL)
	{
		for (short i = 0; i < 6; i++)
		{
			const UINT c = GetNearestPaletteIndex(hpal,map_colors[i]);
			hbrush[i] = CreateSolidBrush(PALETTEINDEX(c));
			hpen[i] = CreatePen(PS_SOLID,1,PALETTEINDEX(c));
		}
	}

	HDC hdc = CreateCompatibleDC(main_dc);
	SelectPalette(hdc,hpal,0);
	HGDIOBJ old_bmp = SelectObject(hdc, map_gworld);
	HGDIOBJ old_brush = SelectObject(hdc,map_brush[0]);
	HGDIOBJ old_pen = SelectObject(hdc,GetStockObject(NULL_PEN));

	RECT draw_rect;
	location map_adj = { 0,0 };
	if (the_item == 11)
	{
			SelectObject(hdc,GetStockObject(WHITE_BRUSH));
			Rectangle(hdc, c_map_world_rect.left, c_map_world_rect.top, c_map_world_rect.right, c_map_world_rect.bottom);
			draw_pcs = FALSE;
	}
	else
	{
		if (modeless_exists[5] == TRUE)
		{
			SetDlgItemText(the_dialog, 3, "");
		}

		if (is_out()) // for outside map, adjust for where in outdoors is being mapped	
		{
			if (party.i_w_c.x == 1)
				map_adj.x += 48;
			if (party.i_w_c.y == 1)
				map_adj.y += 48;
		}

		// Now, if doing just partial restore, crop redraw_rect to save time.
		if (the_item == 5)
		{
			location kludge;
			if (is_out() || (is_combat() && (which_combat_type == 0)) ||
				((overall_mode == 20) && (store_pre_talk_mode == 0)) ||
				((overall_mode == 21) && (store_pre_shop_mode == 0))
				)
				kludge = global_to_local(party.p_loc);
			else
				if (is_combat())
					kludge = pc_pos[current_pc];
				else
					kludge = c_town.p_loc;
			redraw_rect.left = max(0, kludge.x - 4);
			redraw_rect.right = min(view_rect.right, kludge.x + 5);
			redraw_rect.top = max(0, kludge.y - 4);
			redraw_rect.bottom = min(view_rect.bottom, kludge.y + 5);
		}

		// Now, if shopping or talking, just don't touch anything.
		if ((overall_mode == 21) || (overall_mode == 20))
			redraw_rect.right = -1;

		Boolean out_mode;
		if (is_out() ||
			(is_combat() && (which_combat_type == 0)) ||
			((overall_mode == 20) && (store_pre_talk_mode == 0)) ||
			((overall_mode == 21) && (store_pre_shop_mode == 0)))
		{
			out_mode = TRUE;
		}
		else
		{
			out_mode = FALSE;
		}

		RECT area_to_put_on_map_rect = redraw_rect;
		if (the_item == 10)
		{
			area_to_put_on_map_rect.top = area_to_put_on_map_rect.left = 0;
			area_to_put_on_map_rect.right = area_to_put_on_map_rect.bottom = total_size;
		}

		location where;
		for (where.x = area_to_put_on_map_rect.left; where.x < area_to_put_on_map_rect.right; where.x++)
		{
			for (where.y = area_to_put_on_map_rect.top; where.y < area_to_put_on_map_rect.bottom; where.y++)
			{
				if ((map_graphic_placed[where.x / 8][where.y] & (unsigned char)(s_pow(2, where.x % 8))) == 0)
				{
					draw_rect = c_orig_draw_rect;
					OffsetRect(&draw_rect, 6 * where.x + small_adj, 6 * where.y + small_adj);

					unsigned char what_ter;
					if (out_mode == TRUE)
						what_ter = out[where.x + 48 * party.i_w_c.x][where.y + 48 * party.i_w_c.y];
					else
						what_ter = t_d.terrain[where.x][where.y];

					RECT ter_temp_from = c_orig_draw_rect;

					Boolean expl;
					if (out_mode == TRUE)
						expl = out_e[where.x + 48 * party.i_w_c.x][where.y + 48 * party.i_w_c.y];
					else
						expl = is_explored(where.x, where.y) ? TRUE : FALSE;

					if (expl != 0)
					{
						map_graphic_placed[where.x / 8][where.y] =
							map_graphic_placed[where.x / 8][where.y] | (unsigned char)(s_pow(2, where.x % 8));
						short pic = scenario.ter_type(what_ter).picture;
						if (pic >= 1000)
						{
							if (spec_scen_g != NULL)
							{
								pic = pic % 1000;
								RECT custom_from = coord_to_rect(pic % 10, pic / 10);
								OffsetRect(&custom_from, -13, -13);
								SelectObject(hdc, old_bmp);
								rect_draw_some_item_bmp(spec_scen_g, custom_from, map_gworld, draw_rect, 0, 0);
								SelectObject(hdc, map_gworld);
							}
						}
						else
						{
							switch ((pic >= 400) ? anim_map_pats[pic - 400] : map_pats[pic])
							{
							case 0:
							case 10:
							case 11:
								if (terrain_pic[what_ter] < 400)
									OffsetRect(&ter_temp_from, 6 * (terrain_pic[what_ter] % 10) + 312, 6 * (terrain_pic[what_ter] / 10));
								else
									OffsetRect(&ter_temp_from, 24 * ((terrain_pic[what_ter] - 400) / 5) + 312, 6 * ((terrain_pic[what_ter] - 400) % 5) + 163);
								SelectObject(hdc, old_bmp);
								rect_draw_some_item_bmp(mixed_gworld, ter_temp_from, map_gworld, draw_rect, 0, 0);
								SelectObject(hdc, map_gworld);
								break;

							default:
								if (((pic >= 400) ? anim_map_pats[pic - 400] : map_pats[pic]) < 30)
								{
									// Try a little optimization
									if ((pic < 400) && (where.x < area_to_put_on_map_rect.right - 1))
									{
										unsigned char what_ter2;
										if (out_mode == TRUE)
											what_ter2 = out[(where.x + 1) + 48 * party.i_w_c.x][where.y + 48 * party.i_w_c.y];
										else
											what_ter2 = t_d.terrain[where.x + 1][where.y];
										Boolean expl2;
										if (out_mode == TRUE)
											expl2 = out_e[(where.x + 1) + 48 * party.i_w_c.x][where.y + 48 * party.i_w_c.y];
										else
											expl2 = is_explored(where.x + 1, where.y) ? TRUE : FALSE;
										const short pic2 = scenario.ter_type(what_ter2).picture;
										if ((map_pats[pic] == map_pats[pic2]) && (expl2 != 0))
										{
											draw_rect.right += 6;
											map_graphic_placed[(where.x + 1) / 8][where.y] =
												map_graphic_placed[(where.x + 1) / 8][where.y] | (unsigned char)(s_pow(2, (where.x + 1) % 8));
										}
									}
									SelectObject(hdc, map_brush[((pic >= 400) ? anim_map_pats[pic - 400] : map_pats[pic]) - 1]);
									Rectangle(hdc, draw_rect.left, draw_rect.top, draw_rect.right + 1, draw_rect.bottom + 1);
									break;
								}
								OffsetRect(&ter_temp_from, 312 + 24 * ((map_pats[what_ter] - 30) / 5), 138 + 6 * ((map_pats[what_ter] - 30) % 5));
								//OffsetRect(&ter_temp_from,0,115);
								SelectObject(hdc, old_bmp);
								rect_draw_some_item_bmp(mixed_gworld, ter_temp_from, map_gworld, draw_rect, 0, 0);
								SelectObject(hdc, map_gworld);
								break;
							}
						}
					}
				}
			}
		}
	}
	
	SelectObject(hdc,old_brush);
	SelectObject(hdc,old_pen);
	SelectObject(hdc, old_bmp);
	DeleteDC(hdc);

	HDC hdc2 = nullptr;

  	// Now place terrain map gworld
	if (modeless_exists[5] == TRUE)
	{
		hdc2 = GetDC(the_dialog);
		SelectPalette(hdc2, hpal, 0);

		// graphics goes here
		if ((draw_surroundings == TRUE) || (the_item != 5)) // redraw much stuff
		{
			//old_brush = SelectObject(hdc2,bg[0]);
			//old_pen = SelectObject(hdc2,GetStockObject(NULL_PEN));
			//Rectangle(hdc2,whole_map_win_rect.left, whole_map_win_rect.top,whole_map_win_rect.right,whole_map_win_rect.bottom);
			paint_pattern_dc(hdc2, c_whole_map_win_rect,0);
			//SelectObject(hdc2,old_brush);
			//SelectObject(hdc2,old_pen);
			SetBkMode(hdc2,TRANSPARENT);
			SelectObject(hdc2,small_bold_font);
			//win_draw_string(hdc2,
			//	map_title_rect,"Your map:      (Hit Escape to close.)",0,12);
			//ShowOwnedPopups(the_dialog,TRUE);
			GetClientRect(GetDlgItem(the_dialog,1),&draw_rect);
			InvalidateRect(GetDlgItem(the_dialog,1),&draw_rect,FALSE);
		}

		rect_draw_some_item_dc(map_gworld,area_to_draw_from,hdc2,area_to_draw_on,0,2);
	}

	// Now place PCs and monsters
	if ((draw_pcs == TRUE) && (modeless_exists[5] == TRUE))
	{
		if (is_town() && (party.stuff_done[305][2] > 0))
		{
			for (short i = 0; i < T_M; i++)
			{
				if (c_town.monst.dudes[i].active > 0)
				{
					location where = c_town.monst.dudes[i].m_loc;
					if (is_explored(where.x, where.y) &&
						((where.x >= view_rect.left) && (where.x <= view_rect.right)
							&& (where.y >= view_rect.top) && (where.x <= view_rect.bottom))) {

						draw_rect.left = area_to_draw_on.left + 6 * (where.x - view_rect.left);
						draw_rect.top = area_to_draw_on.top + 6 * (where.y - view_rect.top);
						//if ((is_not_out()) && (town_type == 2)) {
						//	draw_rect.left += 48;
						//	draw_rect.top += 48;
						//	}
						draw_rect.right = draw_rect.left + 6;
						draw_rect.bottom = draw_rect.top + 6;

						map_graphic_placed[where.x / 8][where.y] =
							map_graphic_placed[where.x / 8][where.y] & ~((unsigned char)(s_pow(2, where.x % 8)));
						SelectObject(hdc2, hpen[0]);
						SelectObject(hdc2, hbrush[0]);
						Rectangle(hdc2, draw_rect.left, draw_rect.top, draw_rect.right, draw_rect.bottom);
						SelectObject(hdc2, hpen[4]);
						SelectObject(hdc2, hbrush[5]);
						Ellipse(hdc2, draw_rect.left, draw_rect.top, draw_rect.right, draw_rect.bottom);
					}
				}
			}
		}

		if ((overall_mode != 21) && (overall_mode != 20))
		{
			location where = is_town() ? c_town.p_loc : global_to_local(party.p_loc);

			draw_rect.left = area_to_draw_on.left + 6 * (where.x - view_rect.left);
			draw_rect.top = area_to_draw_on.top + 6 * (where.y - view_rect.top);
			//if ((is_not_out()) && (town_type == 2)) {
			//	draw_rect.left += 48;
			//	draw_rect.top += 48;
			//	}
			draw_rect.right = draw_rect.left + 6;
			draw_rect.bottom = draw_rect.top + 6;
			if ((where.x >= 0) && (where.x < 64) &&
				(where.y >= 0) && (where.y < 64)
				)
			{
				map_graphic_placed[where.x / 8][where.y] =  /* Crash! vvv */
				map_graphic_placed[where.x / 8][where.y] & ~((unsigned char)(s_pow(2,where.x % 8)));
				SelectObject(hdc2,hpen[0]);
				SelectObject(hdc2,hbrush[0]);
				Rectangle(hdc2, draw_rect.left,draw_rect.top,draw_rect.right,draw_rect.bottom);
				SelectObject(hdc2,hpen[3]);
				SelectObject(hdc2,hbrush[3]);
				Ellipse(hdc2, draw_rect.left,draw_rect.top,draw_rect.right,draw_rect.bottom);
			}
		}	
	}
	
	// Now exit gracefully
	if (modeless_exists[5] == TRUE)
	{
		fry_dc(the_dialog, hdc2);
	}

	if (modeless_exists[5] == TRUE)
	{
		// graphics goes here
		if ((draw_surroundings == TRUE) || (the_item != 5)) // redraw much stuff
		{
			draw_dialog_graphic_wnd(the_dialog, c_dlogpicrect, 721, FALSE); // draw the map graphic
		}
	}
}


static INT_PTR CALLBACK map_dialog_proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		if (store_map_window_rect.right > 0)
		{
			MoveWindow(hDlg, store_map_window_rect.left, store_map_window_rect.top,
				store_map_window_rect.right - store_map_window_rect.left,
				store_map_window_rect.bottom - store_map_window_rect.top, FALSE);
		}
		else
		{
			GetWindowRect(hDlg,&store_map_window_rect);
			MoveWindow(hDlg,294 + ulx, 47 + uly,
				store_map_window_rect.right - store_map_window_rect.left,
				store_map_window_rect.bottom - store_map_window_rect.top,FALSE);

		}
		kludge_force_full_refresh = TRUE;
		draw_map(hDlg,10);
		kludge_force_full_refresh = FALSE;
		SetFocus(mainPtr);
		break;

	case WM_ERASEBKGND:
		return 1;

	case WM_PAINT:
		kludge_force_full_refresh = TRUE;
		draw_map(hDlg,5);
		kludge_force_full_refresh = FALSE;
		// ShowOwnedPopups(hDlg,TRUE);
		return FALSE;

	case WM_KEYDOWN:
		//	force_play_sound(0);
		if (wParam != VK_RETURN)
			return 0;

	case WM_COMMAND:
		modeless_exists[5] = FALSE;
		GetWindowRect(hDlg,&store_map_window_rect);
		DestroyWindow(hDlg);
		return TRUE;

	case WM_DESTROY:
		modeless_exists[5] = FALSE;
		return 0;
	}
	return FALSE;
}


void display_map()
{
	if ( modeless_exists[5] == TRUE)
		return;
	modeless_exists[5] = TRUE;
	if (map_proc == NULL)
	{
		map_proc = map_dialog_proc;
	}
	modeless_dialogs[5] = CreateDialog(store_hInstance, MAKEINTRESOURCE(1046),mainPtr,map_proc);
	//	 draw_map(modeless_dialogs[5],10);
}


bool is_door(location destination)
{
	if ((scenario.ter_type(t_d.terrain[destination.x][destination.y]).special == terrain_special::UnlockableTerrain) ||
		(scenario.ter_type(t_d.terrain[destination.x][destination.y]).special == terrain_special::ChangeWhenStepOn) ||
		(scenario.ter_type(t_d.terrain[destination.x][destination.y]).special == terrain_special::UnlockableOrBashable))
	{
		return true;
	}
	return false;
}


Boolean quadrant_legal(short i, short j) 
{
	if (party.outdoor_corner.x + i >= scenario.out_width())
		return FALSE;
	if (party.outdoor_corner.y + j >= scenario.out_height())
		return FALSE;
	if (party.outdoor_corner.x + i < 0)
		return FALSE;
	if (party.outdoor_corner.y + j < 0)
		return FALSE;
	return TRUE;
}
