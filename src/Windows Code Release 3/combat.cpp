#include <Windows.h>
#include <array>
#include <cstdio>
#include "global.h"
#include "monster.h"
#include "graphics.h"
#include "locutils.h"
#include "newgraph.h"
#include "infodlgs.h"
#include "fields.h"
#include "text.h"
#include "items.h"
#include "party.h"
#include "combat.h"
#include "exlsound.h"
#include "town.h"
#include "specials.h"
#include "gutils.h"
#include "boe/utility.hpp"
#include "boe/item.hpp"

extern party_record_type party;
extern Adventurers adven;
extern current_town_type c_town;
extern town_item_list t_i;
extern short overall_mode,store_item_spell_level;
extern short which_combat_type;
extern short stat_window;
extern location center;
extern unsigned char combat_terrain[64][64];
extern location pc_pos[6];
extern short current_pc;
extern short pc_last_cast[2][6];
extern short combat_active_pc;
extern Boolean monsters_going,spell_forced,in_scen_debug;
extern short store_spell_target,current_spell_range;
extern std::reference_wrapper<const effect_pat_type> current_pat;
extern short town_size[3];
extern short town_type;
extern short monst_target[T_M]; // 0-5 target that pc   6 - no target  100 + x - target monster x
extern short num_targets_left;
extern location spell_targets[8];
extern Boolean quickfire,force_wall,fire_wall,antimagic,scloud,ice_wall,blade_wall;
extern Boolean sleep_field;
extern Boolean fast_bang;
extern unsigned char misc_i[64][64],sfx[64][64];
extern short store_current_pc;
extern const std::array<short, 62> refer_mage,refer_priest;
extern short combat_posing_monster, current_working_monster; // 0-5 PC 100 + x - monster x
extern short spell_caster, missile_firer,current_monst_tactic;
char create_line[60];
short spell_being_cast;
short missile_inv_slot, ammo_inv_slot;
short force_wall_position = 10; //  10 -> no force wall
Boolean processing_fields = TRUE;
short futzing;
unsigned char store_sum_monst;
short store_sum_monst_cost;

static const location out_start_loc = {20,23};

static const short abil_range[40] = { 0,6,8,8,10, 10,10,8,6,8, 6,0,0,0,6, 0,0,0,0,4, 10,0,0,6,0,
						0,0,0,0,0, 0,0,8,6,9, 0,0,0,0,0 };

//short s_cost[2][62] = {{1,1,1,1,1,2,15,2,1,3, 2,2,2,2,2,2,4,6,2,2, 3,3,4,3,3,5,5,4,6,4,/
//							4,4,4,4,30,-1,8,5, 5,8,7,5,8,10,5,7, 6,6,7,7,12,10,-1,20, 12,8,20,8,14,10,50,10},
//							{1,1,1,2,1,2,2,5,50,1, 2,2,2,2,2,6,8,7,4,3, 3,4,3,3,3,10,6,3,3,7,
//							 4,5,5,15,8,6,4,4, 5,5,25,8,12,12,10,5, 7,6,6,8,14,20,7,6, 8,7,20,12,8,12,30,8}};
static const short s_cost[2][62] = {{1,1,1,1,1,2,50,2,1,3, 2,3,2,2,2,2,4,4,2,6, 3,3,5,3,3,5,6,4,6,4,
							4,5,4,8,30,-1,8,6, 5,8,8,6,9,10,6,6, 7,6,8,7,12,10,12,20, 12,8,20,10,14,10,50,10},
							{1,1,1,2,1,1,3,5,50,1, 2,2,2,2,3,5,8,6,4,2, 3,4,3,3,3,10,5,3,4,6,
							 5,5,5,15,6,5,5,8, 6,7,25,8,10,12,12,6, 8,7,8,8,14,17,8,7, 10,10,35,10,12,12,30,10}};
							 
extern const short mage_range[80] = {0,6,0,0,7,7,0,14,8,0, 6,8,7,10,0,8,3,8,10,6, 0,0,12,0,10,12,4,10,8,0,
						 8,12,12,0,10,4,8,8, 0,0,14,0,2,4,10,12, 8,12,6,8,5,8,4,0, 0,0,8,0,4,2,4,6 
						 ,10,8,8,12,8,10,10,10, 10,10,10,10,10,10,10,10,10,10};
extern const short priest_range[62] = {0,0,0,8,0,0,0,0,0,10, 0,0,10,0,6,4,0,6,6,8, 0,0,8,0,10,0,8,0,0,8,
						 0,10,8,0,6,0,0,0, 0,0,0,9,0,4,0,8, 0,0,10,0,4,8,0,8, 0,4,0,12,0,10,0,0};
static const short monst_mage_cost[27] = {1,1,1,1,2, 2,2,2,2,4, 2,4,4,3,4, 4,4,5,5,5, 5,6,6,6,7, 7,7};
static const short monst_mage_area_effect[27] = {0,0,0,0,0, 0,0,0,1,0, 1,1,0,1,0, 0,0,0,1,0, 1,0,0,0,0, 0,0};
static const short monst_priest_cost[26] = {1,1,1,1,2, 2,2,4,2,3, 3,3,4,4,4, 5,5,5,10,6, 6,10,8,8,8, 8};
static const short monst_priest_area_effect[26] = {0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,1,0,0, 0,0,0,0,0, 1};

static const std::array d_string{"North", "NorthEast", "East", "SouthEast", "South", "SouthWest", "West", "NorthWest"};

short pc_marked_damage[6];
short monst_marked_damage[T_M];

extern const location hor_vert_place[14] = {{0,0},{-1,1},{1,1},{-2,2},{0,2},
								{2,2},{0,1},{-1,2},{1,2},{-1,3},
								{1,3},{0,3},{0,4},{0,5}};
extern const location diag_place[14] = {{0,0},{-1,0},{0,1},{-1,1},{-2,0},
							{0,2},{-2,1},{-1,2},{-2,2},{-3,2},
							{-2,3},{-3,3},{-4,3},{-3,4}};

extern const effect_pat_type single = {{{0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0},
						{0,0,0,0,1,0,0,0,0},
						{0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0}}};
static const effect_pat_type t = {{{0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0},
						{0,0,0,0,1,0,0,0,0},
						{0,0,0,1,1,1,0,0,0},
						{0,0,0,0,1,0,0,0,0},
						{0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0}}};
static const effect_pat_type small_square = {{{0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0},
						{0,0,0,0,1,1,0,0,0},
						{0,0,0,0,1,1,0,0,0},
						{0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0}}};
extern const effect_pat_type square = {{{0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0},
						{0,0,0,1,1,1,0,0,0},
						{0,0,0,1,1,1,0,0,0},
						{0,0,0,1,1,1,0,0,0},
						{0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0}}};
extern const effect_pat_type rad_pat2 = {{{0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0},
						{0,0,0,1,1,1,0,0,0},
						{0,0,1,1,1,1,1,0,0},
						{0,0,1,1,1,1,1,0,0},
						{0,0,1,1,1,1,1,0,0},
						{0,0,0,1,1,1,0,0,0},
						{0,0,0,0,0,0,0,0,0},
						{0,0,0,0,0,0,0,0,0}}};
static const effect_pat_type rad_pat3 = {{{0,0,0,0,0,0,0,0,0},
						{0,0,0,1,1,1,0,0,0},
						{0,0,1,1,1,1,1,0,0},
						{0,1,1,1,1,1,1,1,0},
						{0,1,1,1,1,1,1,1,0},
						{0,1,1,1,1,1,1,1,0},
						{0,0,1,1,1,1,1,0,0},
						{0,0,0,1,1,1,0,0,0},
						{0,0,0,0,0,0,0,0,0}}};
static const effect_pat_type protect_pat = { {{0,4,4,4,4,4,4,4,0},
						{4,8,8,8,8,8,8,8,4},
						{4,8,9,9,9,9,9,8,4},
						{4,8,9,6,6,6,9,8,4},
						{4,8,9,6,6,6,9,8,4},
						{4,8,9,6,6,6,9,8,4},
						{4,8,9,9,9,9,9,8,4},
						{4,8,8,8,8,8,8,8,4},
						{0,4,4,4,4,4,4,4,0}} };

static const effect_pat_type field[8] = {{{{0,0,0,0,1,1,0,0,0},
							{0,0,0,0,1,1,0,0,0},
							{0,0,0,0,1,1,0,0,0},
							{0,0,0,0,1,1,0,0,0},
							{0,0,0,0,1,1,0,0,0},
							{0,0,0,0,1,1,0,0,0},
							{0,0,0,0,1,1,0,0,0},
							{0,0,0,0,1,1,0,0,0},
							{0,0,0,0,1,1,0,0,0}}},

							{{{0,0,0,0,0,0,0,0,1},
							{0,0,0,0,0,0,0,1,1},
							{0,0,0,0,0,0,1,1,0},
							{0,0,0,0,0,1,1,0,0},
							{0,0,0,0,1,1,0,0,0},
							{0,0,0,1,1,0,0,0,0},
							{0,0,1,1,0,0,0,0,0},
							{0,1,1,0,0,0,0,0,0},
							{1,1,0,0,0,0,0,0,0}}},

							{{{0,0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0,0},
							{1,1,1,1,1,1,1,1,1},
							{1,1,1,1,1,1,1,1,1},
							{0,0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0,0}}},

							{{{1,0,0,0,0,0,0,0,0},
							{1,1,0,0,0,0,0,0,0},
							{0,1,1,0,0,0,0,0,0},
							{0,0,1,1,0,0,0,0,0},
							{0,0,0,1,1,0,0,0,0},
							{0,0,0,0,1,1,0,0,0},
							{0,0,0,0,0,1,1,0,0},
							{0,0,0,0,0,0,1,1,0},
							{0,0,0,0,0,0,0,1,1}}},

							{{{0,0,0,1,1,0,0,0,0},
							{0,0,0,1,1,0,0,0,0},
							{0,0,0,1,1,0,0,0,0},
							{0,0,0,1,1,0,0,0,0},
							{0,0,0,1,1,0,0,0,0},
							{0,0,0,1,1,0,0,0,0},
							{0,0,0,1,1,0,0,0,0},
							{0,0,0,1,1,0,0,0,0},
							{0,0,0,1,1,0,0,0,0}}},

							{{{0,0,0,0,0,0,0,1,1},
							{0,0,0,0,0,0,1,1,0},
							{0,0,0,0,0,1,1,0,0},
							{0,0,0,0,1,1,0,0,0},
							{0,0,0,1,1,0,0,0,0},
							{0,0,1,1,0,0,0,0,0},
							{0,1,1,0,0,0,0,0,0},
							{1,1,0,0,0,0,0,0,0},
							{1,0,0,0,0,0,0,0,0}}},

							{{{0,0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0,0},
							{1,1,1,1,1,1,1,1,1},
							{1,1,1,1,1,1,1,1,1},
							{0,0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0,0},
							{0,0,0,0,0,0,0,0,0}}},

							{{{1,1,0,0,0,0,0,0,0},
							{0,1,1,0,0,0,0,0,0},
							{0,0,1,1,0,0,0,0,0},
							{0,0,0,1,1,0,0,0,0},
							{0,0,0,0,1,1,0,0,0},
							{0,0,0,0,0,1,1,0,0},
							{0,0,0,0,0,0,1,1,0},
							{0,0,0,0,0,0,0,1,1},
							{0,0,0,0,0,0,0,0,1}}}};

short last_attacked[6], pc_dir[6], pc_parry[6];
std::array<short, 6> pc_moves;
Boolean center_on_monst;



static const short low[10]{ 15,7,3,3,1,1,1,7,2,1 };
static const short high[10]{ 30,10,5,5,3,2,1,10,4,1 };
static const BoeRect town_rect{ 0,0,47,47 };


void start_outdoor_combat(outdoor_creature_type encounter,unsigned char in_which_terrain,short num_walls)
{
	short i,j,how_many,num_tries = 0;
	short nums[10];

	for (i = 0; i < 7; i++) 
		nums[i] = rand_short(low[i],high[i]);
	for (i = 0; i < 3; i++) 
		nums[i + 7] = rand_short(low[i + 7],high[i + 7]);
	notify_out_combat_began(encounter.what_monst,nums);
  	print_buf();
	play_sound(23);

	which_combat_type = 0;	
	town_type = 1;
	overall_mode = 10;

	// Basically, in outdoor combat, we create kind of a 48x48 town for 
	// the combat to take place in
	for (i = 0; i < 48; i++)
		for (j = 0; j < 48; j++) {
			c_town.explored[i][j] = 0;
			misc_i[i][j] = 0;
			sfx[i][j] = 0;
			}
	c_town.town.in_town_rect = town_rect;

	create_out_combat_terrain((short) in_which_terrain,num_walls,0);////
	
	for (i = 0; i < T_M; i++) {
		c_town.monst.dudes[i].number = 0;
		c_town.monst.dudes[i].active = 0;
		}
	for (i = 0; i < 7; i++) {
		how_many = nums[i];
		if (encounter.what_monst.monst[i] != 0) 
			for (j = 0; j < how_many; j++) 
				set_up_monst(0,encounter.what_monst.monst[i]);
		}
	for (i = 0; i < 3; i++) {
		how_many = nums[i + 7];
		if (encounter.what_monst.friendly[i] != 0) 
			for (j = 0; j < how_many; j++) 
				set_up_monst(1,encounter.what_monst.friendly[i]);
		}

	// place PCs
	pc_pos[0] = out_start_loc;
	update_explored(pc_pos[0]);
	if (get_blockage(combat_terrain[pc_pos[0].x][pc_pos[0].y]) > 0)
		combat_terrain[pc_pos[0].x][pc_pos[0].y] = combat_terrain[0][0];		
	for (i = 1; i < 6; i++) {
		pc_pos[i] = pc_pos[0];
		pc_pos[i].x = pc_pos[i].x + hor_vert_place[i].x;
		pc_pos[i].y = pc_pos[i].y + hor_vert_place[i].y;
		if (get_blockage(combat_terrain[pc_pos[i].x][pc_pos[i].y]) > 0)
			combat_terrain[pc_pos[i].x][pc_pos[i].y] = combat_terrain[0][0];	
		update_explored(pc_pos[i]);
		
		for (j = 0; j < 6; j++) 
			if (j != 2)
				adven[i].status[j] = 0;
		}
		
	// place monsters, w. friendly monsts landing near PCs
	for (i = 0; i < T_M; i++) 
		if (c_town.monst.dudes[i].active > 0) {
			monst_target[i] = 6;
			
			c_town.monst.dudes[i].m_loc.x  = rand_short(15,25);
			c_town.monst.dudes[i].m_loc.y  = rand_short(14,18);
			if (c_town.monst.dudes[i].attitude == 2)
				c_town.monst.dudes[i].m_loc.y += 9;
				else if ((c_town.monst.dudes[i].m_d.mu > 0) || (c_town.monst.dudes[i].m_d.cl > 0))
					c_town.monst.dudes[i].m_loc.y = c_town.monst.dudes[i].m_loc.y - 4;//max(12,c_town.monst.dudes[i].m_loc.y - 4);
			num_tries = 0;
			while (((monst_can_be_there(c_town.monst.dudes[i].m_loc,i) == FALSE) ||
			 (combat_terrain[c_town.monst.dudes[i].m_loc.x][c_town.monst.dudes[i].m_loc.y] == 180) ||
			 (pc_there(c_town.monst.dudes[i].m_loc) < 6)) &&
			 (num_tries++ < 50)) {
				c_town.monst.dudes[i].m_loc.x = rand_short(15,25);
				c_town.monst.dudes[i].m_loc.y = rand_short(14,18);		
				if (c_town.monst.dudes[i].attitude == 2)
					c_town.monst.dudes[i].m_loc.y += 9;
					else if ((c_town.monst.dudes[i].m_d.mu > 0) || (c_town.monst.dudes[i].m_d.cl > 0))
						c_town.monst.dudes[i].m_loc.y = c_town.monst.dudes[i].m_loc.y - 4;//max(12,c_town.monst.dudes[i].m_loc.y - 4);
				}
			if (get_blockage(combat_terrain[c_town.monst.dudes[i].m_loc.x][c_town.monst.dudes[i].m_loc.y]) > 0)
				combat_terrain[c_town.monst.dudes[i].m_loc.x][c_town.monst.dudes[i].m_loc.y] = combat_terrain[0][0];	
		}


	combat_active_pc = 6;
	spell_caster = 6; missile_firer = 6;
	for (i = 0; i < T_M; i++)
		monst_target[i] = 6;

	for (i = 0; i < 6; i++) {
		pc_parry[i] = 0;
		last_attacked[i] = T_M + 10;
		}
		
	for (i = 0; i < NUM_TOWN_ITEMS; i++)
		t_i.items[i].variety = item_variety::None;
	store_current_pc = current_pc;
	current_pc = 0;
	set_pc_moves();
	pick_next_pc();	
	center = pc_pos[current_pc];
	load_area_graphics();
	draw_buttons(0);
	put_pc_screen();
	set_stat_window(current_pc);

	adjust_spell_menus();
	
	//clear_map();
	give_help(48,49,0);

}

Boolean pc_combat_move(location destination)
{
	short dir,monst_hit,s1,s2,i,monst_exist,switch_pc;
	Boolean keep_going = TRUE,forced = FALSE,check_f;
	location monst_loc,store_loc;
	short spec_num;
	
	if (monst_there(destination) > T_M)
		keep_going = check_special_terrain(destination,2,current_pc,&spec_num,&check_f);
	if (check_f == TRUE)
		forced = TRUE;

	if (spec_num == 50)
		forced = TRUE;
	
	if (keep_going == TRUE) {

	dir = set_direction(pc_pos[current_pc], destination);

	if ((loc_off_act_area(destination) == TRUE) && (which_combat_type == 1)) {
		add_string_to_buf("Move: Can't leave town during combat.");
		print_buf();
		return TRUE;
		}
	else if ((combat_terrain[destination.x][destination.y] == 90) && (which_combat_type == 0)) {
			if (rand_short(1,10) < 3)
			{
				adven[current_pc].main_status = status::Fled;
				if (combat_active_pc == current_pc)
					combat_active_pc = 6;
				add_string_to_buf("Moved: Fled.                    ");
				pc_moves[current_pc] = 0;
			}
			else
			{
				take_ap(1);
				add_string_to_buf("Moved: Couldn't flee.                  ");
			}
			return TRUE;		
		}
		else if ((monst_hit = monst_there(destination)) <= T_M) {
			s1 = c_town.monst.dudes[monst_hit].attitude;
			s2 = (s1 % 2 == 1) ? 2 : fancy_choice_dialog(1045,0);
			if ((s2 == 2) && (s1 % 2 != 1)) 
				make_town_hostile();
			if (s2 == 2) {
					last_attacked[current_pc] = monst_hit;
					pc_attack(current_pc,monst_hit);
					return TRUE;
				}
			}
			else if ((switch_pc = pc_there(destination)) < 6) {
				if (pc_moves[switch_pc] == 0) {
					add_string_to_buf("Move: Can't switch places.");
					add_string_to_buf("  (other PC has no APs)	");
					return FALSE;
					}
					else pc_moves[switch_pc]--;
				add_string_to_buf("Move: Switch places.");
				store_loc = pc_pos[current_pc];
				pc_pos[current_pc] = destination;
				pc_pos[switch_pc] = store_loc;
				adven[current_pc].direction = dir;
				take_ap(1);
				check_special_terrain(store_loc,2,switch_pc,&spec_num,&check_f);
				move_sound(combat_terrain[destination.x][destination.y],pc_moves[current_pc]);
				return TRUE;
				}
			else if ((forced == TRUE)
				|| ( !impassable(combat_terrain[destination.x][destination.y]) && (pc_there(destination) == 6))) {
			
				// monsters get back-shots
				for (i = 0; i < T_M; i++) {
					monst_loc = c_town.monst.dudes[i].m_loc;
					monst_exist = c_town.monst.dudes[i].active;
					
					s1 = current_pc;
					if ((monst_exist > 0) && (monst_adjacent(pc_pos[current_pc],i) == TRUE)
						&& (monst_adjacent(destination,i) == FALSE) &&
							(c_town.monst.dudes[i].attitude % 2 == 1) &&
							(c_town.monst.dudes[i].gaffect(affect::Asleep) <= 0) &&
							(c_town.monst.dudes[i].gaffect(affect::Paralyzed) <= 0)) {
							combat_posing_monster = current_working_monster = 100 + i;
							monster_attack_pc(i,current_pc);
							combat_posing_monster = current_working_monster = -1;
							draw_terrain(0);
							}
					if (s1 != current_pc)
						return FALSE;
					}
				
				// move if still alive
				if (adven[current_pc].main_status == status::Normal) {
						pc_dir[current_pc] = set_direction(pc_pos[current_pc],destination);
						pc_pos[current_pc] = destination;
						adven[current_pc].direction = dir;
						take_ap(1);
						//add_string_to_buf("Moved: {}               ",d_string[dir]);
						move_sound(combat_terrain[destination.x][destination.y],pc_moves[current_pc]);

					}
					else return FALSE;
				return TRUE;
				}
				else {
					add_string_to_buf("Blocked: {}               ", d_string[dir]);
					return FALSE;
				}
	}
	return FALSE;
}

void char_parry()
{
	pc_parry[current_pc] = (pc_moves[current_pc] / 4) * 
		(2 + pc_stat_adj(adven[current_pc], skill::Dexterity) + adven[current_pc].skills[skill::Defense]);
	pc_moves[current_pc] = 0;
}

void char_stand_ready()
{
	pc_parry[current_pc] = 100;
	pc_moves[current_pc] = 0;
}

void pc_attack(short who_att,short target)
{
	short r1,r2,weap1 = 24, weap2 = 24,i,store_hp,skill_item;
	skill what_skill1 = skill::Dexterity, what_skill2 = skill::Dexterity;
	creature_data_type *which_m;	
	short hit_adj, dam_adj, spec_dam = 0,poison_amt;

	// slice out bad attacks
	if (adven[who_att].main_status != status::Normal)
		return;
	if ((adven[who_att].gaffect(affect::Asleep) > 0) || (adven[who_att].gaffect(affect::Paralyzed) > 0))
		return;

	last_attacked[who_att] = target;	
	which_m = &c_town.monst.dudes[target];
	
	for (i = 0; i < 24; i++)
		if (((adven[who_att].items[i].variety == item_variety::OneHandedWeapon) || (adven[who_att].items[i].variety == item_variety::TwoHandedWeapon)) &&
			(adven[who_att].equip[i] == TRUE))
				if (weap1 == 24)
					weap1 = i;
					else weap2 = i;

	hit_adj = (-5 * boe_clamp(adven[who_att].gaffect(affect::CursedBlessed),-8,8)) + 5 * boe_clamp(which_m->gaffect(affect::CursedBlessed),-8,8)
			- pc_stat_adj(adven[who_att], skill::Dexterity) * 5 + (get_encumberance(adven[who_att])) * 5;

	dam_adj = boe_clamp(adven[who_att].gaffect(affect::CursedBlessed),-8,8) - boe_clamp(which_m->gaffect(affect::CursedBlessed),-8,8)
			+ pc_stat_adj(adven[who_att], skill::Strength);

	if ((which_m->gaffect(affect::Asleep) > 0) || (which_m->gaffect(affect::Paralyzed) > 0)) {
		hit_adj -= 80;
		dam_adj += 10;
		}
		

	if ((skill_item = text_pc_has_abil_equip(who_att,37)) < 24) {
		hit_adj += 5 * (adven[who_att].items[skill_item].item_level / 2 + 1);
		dam_adj += adven[who_att].items[skill_item].item_level / 2;
		}
	if ((skill_item = text_pc_has_abil_equip(who_att,43)) < 24) {
		dam_adj += adven[who_att].items[skill_item].item_level;
		hit_adj += adven[who_att].items[skill_item].item_level * 2;
		}

	void_sanctuary(who_att);
	
	store_hp = c_town.monst.dudes[target].m_d.health;

	combat_posing_monster = current_working_monster = who_att;
	
	if (weap1 == 24) {	
		add_string_to_buf("{} punches.  ", adven[who_att].name);//,hit_adj, dam_adj);

		r1 = rand_short(0,100) + hit_adj - 20;
		r1 += 5 * (adven[current_pc].gaffect(affect::Webbed) / 3);
		r2 = rand_short(1,4) + dam_adj;
		
		if (r1 <= skill_hit_chance(adven[who_att].skills[what_skill1])) {
			damage_monst(target, who_att, r2, 0, damage_type::Weapon, 4, TRUE);
			}
			else {
				draw_terrain(2);
				add_string_to_buf("{} misses. ", adven[who_att].name);
				play_sound(2);		
			}
		}
		
	// Don't forget awkward and stat adj.
	if (weap1 < 24) {
		what_skill1 = static_cast<skill>(skill::Intelligence + adven[who_att].items[weap1].type);

		// safety valve
		if (what_skill1 == skill::Intelligence)
			what_skill1 = skill::EdgedWeapon;
			
		add_string_to_buf("{} swings. ", adven[who_att].name);//,hit_adj, dam_adj);
		
		r1 = rand_short(0,100) - 5 + hit_adj
		 - 5 * adven[who_att].items[weap1].bonus;
		r1 += 5 * (adven[current_pc].gaffect(affect::Webbed) / 3);
		
		if ((weap2 < 24) && (adven[who_att].traits[trait::Ambidextrous] == FALSE))
			r1 += 25;

		// race adj.
		if ((adven[who_att].race == 2) && (adven[who_att].items[weap1].type == 3))
			r1 -= 10;

		r2 = rand_short(1,adven[who_att].items[weap1].item_level) + dam_adj + 2 + adven[who_att].items[weap1].bonus;			
		if (adven[who_att].items[weap1].ability == 12) 
			r2 = (r2 * (10 - adven[who_att].items[weap1].ability_strength)) / 10;

		if (r1 <= skill_hit_chance(adven[who_att].skills[what_skill1])) {
			spec_dam = calc_spec_dam(adven[who_att].items[weap1].ability,
				adven[who_att].items[weap1].ability_strength,which_m);

			// assassinate
			r1 = rand_short(0,100);
			if ((adven[who_att].level >= which_m->m_d.level - 1) 
				&& (adven[who_att].skills[skill::Assassination] >= which_m->m_d.level / 2)
				&& (which_m->m_d.spec_skill != 12)) // Can't assassinate splitters
				if (r1 < skill_hit_chance(max(adven[who_att].skills[skill::Assassination] - which_m->m_d.level,0))) {
					add_string_to_buf("  You assassinate.           ");
					spec_dam += r2;
					}
					
			switch (what_skill1) {
			 	case skill::EdgedWeapon:
					if (adven[who_att].items[weap1].item_level < 8)
						damage_monst(target, who_att, r2, spec_dam, damage_type::Weapon, 1, TRUE);
						else damage_monst(target, who_att, r2, spec_dam, damage_type::Weapon, 2, TRUE);
					break;	
			 	case skill::BashingWeapon:
					damage_monst(target, who_att, r2, spec_dam, damage_type::Weapon, 4, TRUE);
					break;	
			 	case skill::PoleWeapon:
					damage_monst(target, who_att, r2, spec_dam, damage_type::Weapon, 3, TRUE);
					break;
				default:
					// CC: Not present in original source
					break;
				}
			// poison			
			if ((adven[who_att].gaffect(affect::PoisonedWeapon) > 0) && (adven[who_att].weap_poisoned == weap1)) {
					poison_amt = adven[who_att].gaffect(affect::PoisonedWeapon);
					if (pc_has_abil_equip(adven[who_att],51) < 24)
						poison_amt += 2;
					poison_monst(which_m,poison_amt);
					adven[who_att].reduce_affect(affect::PoisonedWeapon);
				}
			if ((adven[who_att].items[weap1].ability == 14) && (rand_short(0,1) == 1)) {
				add_string_to_buf("  Blade drips venom.             ");
				poison_monst(which_m,adven[who_att].items[weap1].ability_strength / 2);
				}
			if ((adven[who_att].items[weap1].ability == 9) && (rand_short(0,1) == 1)) {
				add_string_to_buf("  Blade drips acid.             ");
				acid_monst(which_m,adven[who_att].items[weap1].ability_strength / 2);
				}
			if ((adven[who_att].items[weap1].ability == 10) && (rand_short(0,1) == 1)) {
				add_string_to_buf("  Blade drains life.             ");
				pc_heal(adven[who_att],adven[who_att].items[weap1].ability_strength / 2);
				}
			}
			else {
				draw_terrain(2);
				add_string_to_buf("  {} misses.              ", adven[who_att].name);
				if (what_skill1 == skill::PoleWeapon)
					play_sound(19);
					else play_sound(2);
			}	
		}
	if ((weap2 < 24) && (which_m->active > 0)) {

		what_skill2 = static_cast<skill>(skill::Intelligence + adven[who_att].items[weap2].type);

		// safety valve
		if (what_skill2 == skill::Intelligence)
			what_skill2 = skill::EdgedWeapon;

		add_string_to_buf("{} swings.                    ", adven[who_att].name);//,hit_adj, dam_adj);
		r1 = rand_short(0,100) + hit_adj - 5 * adven[who_att].items[weap2].bonus;
		
		// Ambidextrous?
		if (adven[who_att].traits[trait::Ambidextrous] == FALSE)
			r1 += 25;

		r1 += 5 * (adven[current_pc].gaffect(affect::Webbed) / 3);
		r2 = rand_short(1,adven[who_att].items[weap2].item_level) + dam_adj - 1 + adven[who_att].items[weap2].bonus;
		if (adven[who_att].items[weap2].ability == 12) 
			r2 = (r2 * (10 - adven[who_att].items[weap2].ability_strength)) / 10;
			
		if (r1 <= skill_hit_chance(adven[who_att].skills[what_skill2])) {
			spec_dam = calc_spec_dam(adven[who_att].items[weap2].ability,
				adven[who_att].items[weap2].ability_strength,which_m);
			switch (what_skill2) {
			 	case skill::EdgedWeapon:
					if (adven[who_att].items[weap1].item_level < 8)
						damage_monst(target, who_att, r2, spec_dam, damage_type::Weapon, 1, TRUE);
						else damage_monst(target, who_att, r2, spec_dam, damage_type::Weapon, 2, TRUE);
					break;	
			 	case skill::BashingWeapon:
					damage_monst(target, who_att, r2, spec_dam, damage_type::Weapon, 4, TRUE);
					break;	
			 	case skill::PoleWeapon:
					damage_monst(target, who_att, r2, spec_dam, damage_type::Weapon, 3, TRUE);
					break;
				default:
					// CC: Not present in original source
					break;
			}

			if ((adven[who_att].items[weap2].ability == 14) && (rand_short(0,1) == 1)) {
				add_string_to_buf("  Blade drips venom.             ");
				poison_monst(which_m,adven[who_att].items[weap2].ability_strength / 2);
				}
			if ((adven[who_att].items[weap2].ability == 9) && (rand_short(0,1) == 1)) {
				add_string_to_buf("  Blade drips acid.             ");
				acid_monst(which_m,adven[who_att].items[weap2].ability_strength / 2);
				}
			if ((adven[who_att].items[weap2].ability == 10) && (rand_short(0,1) == 1)) {
				add_string_to_buf("  Blade drains life.             ");
				pc_heal(adven[who_att],adven[who_att].items[weap2].ability_strength / 2);
				}

			}
			else {
				draw_terrain(2);
				add_string_to_buf("{} misses.             ", adven[who_att].name);
				if (what_skill2 == skill::PoleWeapon)
					play_sound(19);
					else play_sound(2);
			}
		}
	adven[who_att].reduce_affect(affect::PoisonedWeapon);
	take_ap(4);

	if (((c_town.monst.dudes[target].gaffect(affect::MartyrsShield) > 0) || (c_town.monst.dudes[target].m_d.spec_skill == 22))
	 && (store_hp - c_town.monst.dudes[target].m_d.health > 0)) {
		add_string_to_buf("  Shares damage!   ");
		damage_pc(who_att, store_hp - c_town.monst.dudes[target].m_d.health, damage_type::GeneralMagic,-1);
		}
	combat_posing_monster = current_working_monster = -1;
 }


short calc_spec_dam(short abil,short abil_str,creature_data_type *monst) ////
{
	short store = 0;
	
	switch (abil) {
		case 1: case 171:
			store += get_ran(abil_str,1,6);
			break;
		case 2:
			if (monst->m_d.m_type == 7)
				store += 8 * abil_str;
			break;
		case 175:
			if (monst->m_d.m_type == 7)
				store += 25 + 8 * abil_str;
			break;
		case 174:
			if (monst->m_d.m_type == 8)
				store += 20 + 6 * abil_str;
			break;
		case 3:
			if (monst->m_d.m_type == 8)
				store += 6 * abil_str;
			break;
		case 4:
			if (monst->m_d.m_type == 1)
				store += 5 * abil_str;
			break;
		case 5:
			if (monst->m_d.m_type == 9)
				store += 8 * abil_str;
			break;
		case 6:
			if (monst->m_d.m_type == 4)
				store += 4 * abil_str;
			break;
		case 7:
			if (monst->m_d.m_type == 5)
				store += 4 * abil_str;
			break;
		case 8:
			if (monst->m_d.m_type == 12)
				store += 7 * abil_str;
			break;
		case 13:
			scare_monst(monst,abil_str * 10);
			break;
		case 173:
			acid_monst(monst,abil_str);
			break;
		}
	return store;
}

void place_target(location target)
{
	short i;
 
 	if (num_targets_left > 0) {
		if (loc_off_act_area(target) == TRUE) {
				add_string_to_buf("  Space not in town.           ");
				return;
				}
		if (can_see(pc_pos[current_pc],target,0) > 4) {
				add_string_to_buf("  Can't see target.           ");
				return;
				}
		if (dist(pc_pos[current_pc],target) > ((spell_being_cast >= 100) ? priest_range[spell_being_cast - 100] : mage_range[spell_being_cast]))  {
			add_string_to_buf("  Target out of range.");
			return;
			}
		if ((get_obscurity(target.x,target.y) == 5) && (spell_being_cast != 41)) {
				 add_string_to_buf("  Target space obstructed.           ");
				 return;
				 }
		if (is_antimagic(target.x,target.y)) {
				 add_string_to_buf("  Target in antimagic field.");
				 return;
				 }
		for (i = 0; i < 8; i++) {
			if (same_point(spell_targets[i],target) == TRUE) {
				add_string_to_buf("  Target removed.");
				num_targets_left++;
				spell_targets[i].x = 120;
				play_sound(-1);
				return;
				}
			}
		for (i = 0; i < 8; i++)
			if (spell_targets[i].x == 120) {
				add_string_to_buf("  Target added.");
				spell_targets[i] = target;
				num_targets_left--;
				play_sound(0);
				i = 8;
			}
	}

	if (num_targets_left == 0) {
		do_combat_cast(spell_targets[0]);
		overall_mode = 10;
		}
}

// Special spells:
//   62 - Carrunos
//	 63 - Summon Rat
//	 64 - Ice Wall Balls
//	 65 - Goo Bomb
//   66 - Foul vapors
//	 67 - SLeep cloud
//	 68 - Acid spray
//   69 - Paralyze beam
//   70 - mass sleep
void do_combat_cast(location target)
{
	short adjust,r1,r2,targ_num,s_num,level,bonus = 1,i,item,store_sound = 0;
	creature_data_type *cur_monst;
	Boolean freebie = FALSE,ap_taken = FALSE,cost_taken = FALSE;
	short num_targets = 1,store_m_type = 2;
	short spray_type_array[15] = {1,1,1,4,4,5,5,5,6,6,7,7,8,8,9};
	unsigned char summon;

	location ashes_loc = {0,0};

	// to wedge in animations, have to kludge like crazy
	short boom_dam[8] = {0,0,0,0,0,0,0,0};
	damage_type boom_type[8]{};
	location boom_targ[8];
	
	if (spell_being_cast >= 1000) {
		spell_being_cast -= 1000;
		freebie = TRUE;
		level = store_item_spell_level;
		level = boe_clamp(level,2,20);
		}
		else {
			level = 1 + adven[current_pc].level / 2;
			bonus = pc_stat_adj(adven[current_pc], skill::Intelligence);
			}
	force_wall_position = 10;
	s_num = spell_being_cast % 100;
	
	void_sanctuary(current_pc);
	if (overall_mode == 11) {
		spell_targets[0] = target;
		}
		else {
			num_targets = 8;
			}
	
		spell_caster = current_pc;

	// assign monster summoned, if summoning
	if (spell_being_cast == 16) {
			summon = get_summon_monster(1);

			}
	if (spell_being_cast == 26) {
			summon = get_summon_monster(1);
			}
	if (spell_being_cast == 43) {
			summon = get_summon_monster(2);
			}
	if (spell_being_cast == 58) {
			summon = get_summon_monster(3);
			}
	combat_posing_monster = current_working_monster = current_pc;

	for (i = 0; i < num_targets; i++) 
		if (spell_targets[i].x != 120) {
		target = spell_targets[i];
		spell_targets[i].x = 120; // nullify target as it is used
	
		if ((cost_taken == FALSE) && (freebie == FALSE) && (s_num != 52) && (s_num != 35)) {
			adven[current_pc].cur_sp -= s_cost[spell_being_cast / 100][s_num];
			cost_taken = TRUE;
			}
		if ((cost_taken == FALSE) && (freebie == FALSE) && (s_num == 35)) {
			adven[current_pc].cur_sp -=	store_sum_monst_cost;
			cost_taken = TRUE;
			}
			
		if ((adjust = can_see(pc_pos[current_pc],target,0)) > 4) {
			add_string_to_buf("  Can't see target.           ");
			}
		else if (loc_off_act_area(target) == TRUE) {
			add_string_to_buf("  Space not in town.           ");
			}
		else if (dist(pc_pos[current_pc],target) > ((spell_being_cast >= 100) ? priest_range[spell_being_cast - 100] : mage_range[spell_being_cast]))  	
		add_string_to_buf("  Target out of range.");
			else if ((get_obscurity(target.x,target.y) == 5) && (spell_being_cast != 41))
			 add_string_to_buf("  Target space obstructed.           ");
			else if (is_antimagic(target.x,target.y))
			 add_string_to_buf("  Target in antimagic field.");
			else {
				if (ap_taken == FALSE) {
					if (freebie == FALSE)
						take_ap(5);
					ap_taken = TRUE;
					draw_terrain(2);
					}
				boom_targ[i] = target;
				switch (spell_being_cast) {

				case 8: case 28: case 65: // web spells
					place_spell_pattern(current_pat,target,1,FALSE,current_pc);
					break;
				case 5: case 17: // fire wall spells
					place_spell_pattern(current_pat,target,5,FALSE,current_pc);
					break;
				case 15: case 66: // stink cloud
					place_spell_pattern(current_pat,target,7,FALSE,current_pc);
					break;
				case 25: case 44: case 126: // force walls
					place_spell_pattern(current_pat,target,4,FALSE,current_pc);
					break;				
				case 37: case 64: // ice walls
					place_spell_pattern(current_pat,target,8,FALSE,current_pc);				
					break;
				case 51: // antimagic
					place_spell_pattern(current_pat,target,6,FALSE,current_pc);									
					break;
				case 19: case 67: // sleep clouds
					place_spell_pattern(current_pat,target,12,FALSE,current_pc);									
					break;
				case 60:
					make_quickfire(target.x,target.y);
					break;
				case 45: // spray fields
					r1 = rand_short(0,14);
					place_spell_pattern(current_pat,target,spray_type_array[r1],FALSE,current_pc);									
					break;
				case 159:  // wall of blades
					place_spell_pattern(current_pat,target,9,FALSE,current_pc);													
					break;
				case 145: case 119: case 18: // wall dispelling
					place_spell_pattern(current_pat,target,11,FALSE,current_pc);													
					break;
			case 42: // Fire barrier
				play_sound(68);
				r1 = get_ran(3,2,7);
				hit_space(target,r1, damage_type::Fire,TRUE,TRUE);
				make_fire_barrier(target.x,target.y);
				if (is_fire_barrier(target.x,target.y))
					add_string_to_buf("  You create the barrier.              ");
					else add_string_to_buf("  Failed.");
				break;		
			case 59: // Force barrier
				play_sound(68);
				r1 = get_ran(7,2,7);
				hit_space(target,r1, damage_type::Fire,TRUE,TRUE);
				make_force_barrier(target.x,target.y);
				if (is_force_barrier(target.x,target.y))
					add_string_to_buf("  You create the barrier.              ");
					else add_string_to_buf("  Failed.");				
				break;		
			
			default: // spells which involve animations 
				start_missile_anim();
				switch (spell_being_cast) {

				case 157:	
					add_missile(target,9,1,0,0);
					store_sound = 11;
					r1 = min(18,(level * 7) / 10 + 2 * bonus);
					place_spell_pattern(rad_pat2,target,130 + r1,TRUE,current_pc);
					ashes_loc = target;
					break;

				case 1: case 31:
					r1 = (spell_being_cast == 1) ? get_ran(2,1,4) : get_ran(min(20,level + bonus),1,4);
					add_missile(target,6,1,0,0);
					do_missile_anim(100,pc_pos[current_pc],11); 
					hit_space(target,r1,(spell_being_cast == 1) ? damage_type::GeneralMagic : damage_type::Cold,1,0);
					break;
				case 27: // flame arrows
					add_missile(target,4,1,0,0);
					r1 = get_ran(2,1,4);
					boom_type[i] = damage_type::Fire;
					boom_dam[i] = r1;
					//hit_space(target,r1,damage_type::Fire,1,0);
					break;
				case 129: // smite
					add_missile(target,6,1,0,0);
					r1 = get_ran(2,1,5);
					boom_type[i] = damage_type::Cold;
					boom_dam[i] = r1;
					//hit_space(target,r1,damage_type::Cold,1,0);				
					break;
				case 114:
					r1 = get_ran(min(7,2 + bonus + level / 2),1,4);
					add_missile(target,14,1,0,0);
					do_missile_anim(100,pc_pos[current_pc],24); 
					hit_space(target,r1, damage_type::Unblockable,1,0);
					break;
				case 11:
					r1 = get_ran(min(10,1 + level / 3 + bonus),1,6);
					add_missile(target,2,1,0,0);
					do_missile_anim(100,pc_pos[current_pc],11); 
					hit_space(target,r1, damage_type::Fire,1,0);
					break;	
				case 22: case 141:
					r1 = min(9,1 + (level * 2) / 3 + bonus) + 1;
					add_missile(target,2,1,0,0);
					store_sound = 11;
					//do_missile_anim(100,pc_pos[current_pc],11); 
					if (spell_being_cast == 141)
						r1 = (r1 * 14) / 10;
						else if (r1 > 10) r1 = (r1 * 8) / 10;
					if (r1 <= 0) r1 = 1;
					place_spell_pattern(square,target,50 + r1,TRUE,current_pc);
					ashes_loc = target;
					break;
				case 40:
					add_missile(target,2,1,0,0);
					store_sound = 11;
					//do_missile_anim(100,pc_pos[current_pc],11); 
					r1 = min(12,1 + (level * 2) / 3 + bonus) + 2;
					if (r1 > 20)
						r1 = (r1 * 8) / 10; 
					place_spell_pattern(rad_pat2,target,50 + r1,TRUE,current_pc);
					ashes_loc = target;
					break;
				case 48:  // kill
					add_missile(target,9,1,0,0);
					do_missile_anim(100,pc_pos[current_pc],11); 
					r1 = get_ran(3,0,10) + adven[current_pc].level * 2;
					hit_space(target,40 + r1, damage_type::GeneralMagic,1,0);
					break;
				case 61:	// death arrows
					add_missile(target,9,1,0,0);
					store_sound = 11;
					r1 = get_ran(3,0,10) + adven[current_pc].level + 3 * bonus;
					boom_type[i] = damage_type::GeneralMagic;
					boom_dam[i] = r1;
					//hit_space(target,40 + r1, damage_type::GeneralMagic,1,0);
					break;
				// summoning spells
		case 35: case 16: case 26: case 43: case 58: case 50:  
		case 63: case 115: case 134: case 143: case 150:
			add_missile(target,8,1,0,0);
			do_missile_anim(50,pc_pos[current_pc],61); 
			switch (spell_being_cast) {
			case 35: // Simulacrum
				r2 = get_ran(3,1,4) + pc_stat_adj(adven[current_pc], skill::Intelligence);
				if (summon_monster(store_sum_monst,target,r2,2) == FALSE)
					add_string_to_buf("  Summon failed.");
				break;
			case 16: // summon beast
				r2 = get_ran(3,1,4) + pc_stat_adj(adven[current_pc], skill::Intelligence);
				if ((summon < 0) || (summon_monster(summon,target,r2,2) == FALSE))
					add_string_to_buf("  Summon failed.");
				break;
			case 26: // summon 1
				r2 = get_ran(4,1,4) + pc_stat_adj(adven[current_pc], skill::Intelligence);
				if ((summon < 0) || (summon_monster(summon,target,r2,2) == FALSE))
					add_string_to_buf("  Summon failed.");
				break;
			case 43: // summon 2
				r2 = get_ran(5,1,4) + pc_stat_adj(adven[current_pc], skill::Intelligence);
				if ((summon < 0) || (summon_monster(summon,target,r2,2) == FALSE))
					add_string_to_buf("  Summon failed.");
				break;
			case 58: // summon 3
				r2 = get_ran(7,1,4) + pc_stat_adj(adven[current_pc], skill::Intelligence);
				if ((summon < 0) || (summon_monster(summon,target,r2,2) == FALSE))
					add_string_to_buf("  Summon failed.");
				break;
			case 50: // Daemon
				r2 = get_ran(5,1,4) + pc_stat_adj(adven[current_pc], skill::Intelligence);
				if (summon_monster(85,target,r2,2) == FALSE)
					add_string_to_buf("  Summon failed.");
				break;
			case 63: // Rat!
				r1 = get_ran(3,1,4);
				if (summon_monster(80,target,r1,2) == FALSE)
					add_string_to_buf("  Summon failed.");
				break;
			
			case 115: // summon spirit
				r2 = get_ran(2,1,5) + pc_stat_adj(adven[current_pc], skill::Intelligence);
				if (summon_monster(125,target,r2,2) == FALSE)
					add_string_to_buf("  Summon failed.");		
				break;
			case 134: // s to s
				r1 = rand_short(0,7);
				r2 = get_ran(2,1,5) + pc_stat_adj(adven[current_pc], skill::Intelligence);
				if (summon_monster((r1 == 1) ? 100 : 99,target,r2,2) == FALSE)
					add_string_to_buf("  Summon failed.");
				break;
			case 143: // host
				r2 = get_ran(2,1,4) + pc_stat_adj(adven[current_pc], skill::Intelligence);
				if (summon_monster((i == 0) ? 126 : 125,target,r2,2) == FALSE)
					add_string_to_buf("  Summon failed.");		
				break;
			case 150: // guardian
				r2 = get_ran(6,1,4) + pc_stat_adj(adven[current_pc], skill::Intelligence);
				if (summon_monster(122,target,r2,2) == FALSE)
					add_string_to_buf("  Summon failed.");		
				break;
				}
			break;

							
		default:
					targ_num = monst_there(target);
					if (targ_num > T_M)
						add_string_to_buf("  Nobody there                 ");
						else {
							cur_monst = &c_town.monst.dudes[targ_num];	
							if ((cur_monst->attitude % 2 != 1) && (spell_being_cast != 7)
							 && (spell_being_cast != 34))
								make_town_hostile();
							store_sound = (spell_being_cast >= 50) ? 24 : 25;
							switch (spell_being_cast) {
								case 68: // spray acid
									store_m_type = 0;
									acid_monst(cur_monst,level);
									break;
								case 69: // paralyze
									store_m_type = 9;
									charm_monst(cur_monst,0,12,500);
									break;
									
								case 7: // monster info
									store_m_type = -1;
									play_sound(52);
									party.m_seen[cur_monst->number] = TRUE;
									adjust_monst_menu();
									display_monst(0,cur_monst,0);
									break;
								case 34: // Capture soul
									store_m_type = 15;
									record_monst(cur_monst);
									break;
									
								case 52: // Mindduel!
									store_m_type = -1;
									if ((cur_monst->m_d.mu == 0) && (cur_monst->m_d.cl == 0))
										add_string_to_buf("  Can't duel: no magic.");
										else {
											item = pc_has_abil(adven[current_pc],159);
											if (item >= 24)
												add_string_to_buf("  You need a smoky crystal.   ");
												else {
													remove_charge(current_pc,item);
													do_mindduel(current_pc,cur_monst);
													}
											}
									break;
									
								case 117: // charm
									store_m_type = 14;
									charm_monst(cur_monst,-1 * (bonus + adven[current_pc].level / 8),0,0);
									break;
									
								case 118: // disease
									store_m_type = 0;
									r1 = rand_short(0,1);
									disease_monst(cur_monst,2 + r1 + bonus);
									break;
																		
								case 62:
									store_m_type = 14;
									cur_monst->m_d.health += 20;
									store_sound = 55;
									break;	
						
								case 13:
									store_m_type = 14;
									dumbfound_monst(cur_monst,1 + bonus / 3);
									store_sound = 53;
									break;
									
								case 4:
									store_m_type = 11;
									scare_monst(cur_monst,get_ran(2 + bonus,1,6));
									store_sound = 54;
									break;
								case 24:
									store_m_type = 11;
									scare_monst(cur_monst,get_ran(min(20,adven[current_pc].level / 2 + bonus),1,
										((spell_being_cast == 24) ? 8 : 6)));
									store_sound = 54;
									break;
									
								case 12:
									store_m_type = 11;
									r1 = rand_short(0,1);
									slow_monst(cur_monst,2 + r1 + bonus);
									break;
									
								case 10: case 36:
									store_m_type = (spell_being_cast == 36) ? 4 : 11;
									poison_monst(cur_monst,2 + bonus / 2);
									store_sound = 55;
									break;
								case 49: // Paralysis
									store_m_type = 9;
									charm_monst(cur_monst,-10,12,1000);
									break;
								case 30:
									store_m_type = 11;
									poison_monst(cur_monst,4 + bonus / 2);
									store_sound = 55;
									break;
								case 46:
									store_m_type = 11;
									poison_monst(cur_monst,8 + bonus / 2);
									store_sound = 55;
									break;
									
								case 109: // stumble
									store_m_type = 8;
									curse_monst(cur_monst,4 + bonus);
									break;

								case 112:
									store_m_type = 8;
									curse_monst(cur_monst,2 + bonus);
									break;
									
								case 122:
									store_m_type = 8;
									curse_monst(cur_monst,2 + adven[current_pc].level / 2);
									break;	
																	
								case 103: case 132:
									if (cur_monst->m_d.m_type != 8) {
										add_string_to_buf("  Not undead.                    ");
										store_m_type = -1;
										break;
										}
									store_m_type = 8;
									r1 = rand_short(0,90);
									if (r1 > skill_hit_chance(boe_clamp(bonus * 2 + level * 4 - (cur_monst->m_d.level / 2) + 3,0,19)))
										add_string_to_buf("  Monster resisted.                  ");
										else {
										r1 = get_ran((spell_being_cast == 103) ? 2 : 6, 1, 14);	

										hit_space(cur_monst->m_loc,r1, damage_type::Unblockable,0,current_pc);
										}
									break;
									
								case 155:
									if (cur_monst->m_d.m_type != 7) {
										add_string_to_buf("  Not a demon.                    ");
										store_m_type = -1;
										break;
										}
									r1 = rand_short(0,100);
									if (r1 > skill_hit_chance(boe_clamp(level * 4 - cur_monst->m_d.level + 10,0,19)))
										add_string_to_buf("  Demon resisted.                  ");
										else {
										r1 = get_ran(8 + bonus * 2, 1, 11);	
										if (PSD[4][0] == 3) // anama
											r1 += 25;
										//play_sound(53);
										hit_space(cur_monst->m_loc,r1, damage_type::Unblockable,0,current_pc);
										}
									break;
								}
							if (store_m_type >= 0)
								add_missile(target,store_m_type,1,
									14 * (cur_monst->m_d.x_width - 1),18 * (cur_monst->m_d.y_width - 1));

							}
					break;			 
					}
				}
			}
		}
		
	do_missile_anim((num_targets > 1) ? 35 : 60,pc_pos[current_pc],store_sound); 

	// process mass damage
	for (i = 0; i < 8; i++)
		if (boom_dam[i] > 0)
			hit_space(boom_targ[i],boom_dam[i],boom_type[i],1,0);				
	
	if (ashes_loc.x > 0)
		make_sfx(ashes_loc.x,ashes_loc.y,6);
	
	do_explosion_anim(5,0);
	
	end_missile_anim();
	
	handle_marked_damage();
	combat_posing_monster = current_working_monster = -1;
	
	print_buf();
}

void handle_marked_damage()
{
	short i;
	
	for (i = 0; i < 6; i++)
		if (pc_marked_damage[i] > 0)
			{
			damage_pc(i,pc_marked_damage[i], damage_type::MarkedDamage,-1);
			pc_marked_damage[i] = 0;
			}
	for (i = 0; i < T_M; i++)
		if (monst_marked_damage[i] > 0)
			{
			damage_monst(i, current_pc, monst_marked_damage[i], 0, damage_type::Unknown9);

			monst_marked_damage[i] = 0;
			}
}

void load_missile()
{
	short i,bow = 24,arrow = 24,thrown = 24,crossbow = 24,bolts = 24,no_ammo = 24;
		
	for (i = 0; i < 24; i++) {
		if ((adven[current_pc].equip[i] == TRUE) &&
			(adven[current_pc].items[i].variety == item_variety::ThrownMissile))
				thrown = i;
		if ((adven[current_pc].equip[i] == TRUE) &&
			(adven[current_pc].items[i].variety == item_variety::Bow))
				bow = i;
		if ((adven[current_pc].equip[i] == TRUE) &&
			(adven[current_pc].items[i].variety == item_variety::Arrows))
				arrow = i;
		if ((adven[current_pc].equip[i] == TRUE) &&
			(adven[current_pc].items[i].variety == item_variety::Crossbow))
				crossbow = i;
		if ((adven[current_pc].equip[i] == TRUE) &&
			(adven[current_pc].items[i].variety == item_variety::Bolts))
				bolts = i;
		if ((adven[current_pc].equip[i] == TRUE) &&
			(adven[current_pc].items[i].variety == item_variety::MissileWeapon))
				no_ammo = i;
		}

	if (thrown < 24) {
		ammo_inv_slot = thrown;
		add_string_to_buf("Throw: Select a target.        ");
		add_string_to_buf("  (Hit 's' to cancel.)");
		overall_mode = 13;
		current_spell_range = 8;
		current_pat = single;
		}
	else if (((bolts < 24) && (bow < 24)) || ((arrow < 24) && (crossbow < 24)))  {
			add_string_to_buf("Fire: Wrong ammunition.       ");
			}
	else if ((arrow == 24) && (bow < 24)) {
			add_string_to_buf("Fire: Equip some arrows.       ");
			}
	else if ((arrow < 24) && (bow < 24)) {
			missile_inv_slot = bow;
			ammo_inv_slot = arrow;
			overall_mode = 12;
			add_string_to_buf("Fire: Select a target.        ");
		add_string_to_buf("  (Hit 's' to cancel.)");
			current_spell_range = 12;
			current_pat = single; 
			}
	else if ((bolts < 24) && (crossbow < 24)) {
			missile_inv_slot = crossbow;
			ammo_inv_slot = bolts;
			overall_mode = 12;
			add_string_to_buf("Fire: Select a target.        ");
		add_string_to_buf("  (Hit 's' to cancel.)");
			current_spell_range = 12;
			current_pat = single; 
			}
	else if (no_ammo < 24) {
			missile_inv_slot = no_ammo;
			ammo_inv_slot = no_ammo;
			overall_mode = 12;
			add_string_to_buf("Fire: Select a target.        ");
			add_string_to_buf("  (Hit 's' to cancel.)");
			current_spell_range = 12;
			current_pat = single; 
		}
	else add_string_to_buf("Fire: Equip a missile.       ");
}

void fire_missile(location target)
{
	short r1, r2, skill, dam, dam_bonus, hit_bonus, range, targ_monst, spec_dam = 0,poison_amt = 0;
	short skill_item,m_type = 0;
	creature_data_type *cur_monst;
	Boolean exploding = FALSE;
	
	skill = (overall_mode == 12) ? adven[current_pc].skills[skill::Archery] : adven[current_pc].skills[skill::Defense];
	range = (overall_mode == 12) ? 12 : 8;
	dam = adven[current_pc].items[ammo_inv_slot].item_level;
	dam_bonus = adven[current_pc].items[ammo_inv_slot].bonus + boe_clamp(adven[current_pc].gaffect(affect::CursedBlessed),-8,8);
	hit_bonus = (overall_mode == 12) ? adven[current_pc].items[missile_inv_slot].bonus : 0;
	hit_bonus += pc_stat_adj(adven[current_pc], skill::Dexterity) - can_see(pc_pos[current_pc],target,0)
		+ boe_clamp(adven[current_pc].gaffect(affect::CursedBlessed),-8,8);
	if ((skill_item = pc_has_abil_equip(adven[current_pc],41)) < 24) {
		hit_bonus += adven[current_pc].items[skill_item].ability_strength / 2;
		dam_bonus += adven[current_pc].items[skill_item].ability_strength / 2;
		}
	
	// race adj.
	if (adven[current_pc].race == 1)
		hit_bonus += 2;
	
	if (adven[current_pc].items[ammo_inv_slot].ability == 172) 
		exploding = TRUE;
	if (adven[current_pc].items[ammo_inv_slot].variety != item_variety::MissileWeapon) {
		if (adven[current_pc].items[ammo_inv_slot].ability != 170)
			adven[current_pc].items[ammo_inv_slot].charges--;
			else adven[current_pc].items[ammo_inv_slot].charges = 1;
		if ((pc_has_abil_equip(adven[current_pc],11) < 24) && (adven[current_pc].items[ammo_inv_slot].ability != 170))
			adven[current_pc].items[ammo_inv_slot].charges--;
		if (adven[current_pc].items[ammo_inv_slot].charges <= 0)
			take_item(current_pc,ammo_inv_slot);
		}

	// First, some missiles do special things
	if (exploding == TRUE) {
		take_ap((overall_mode == 12) ? 3 : 2);
		add_string_to_buf("  The arrow explodes!             ");
		run_a_missile(pc_pos[current_pc],target,2,1,5,
			0,0,100);
		start_missile_anim();
		add_missile(target,2,1, 0, 0);
		do_missile_anim(100,pc_pos[current_pc], 5);
		place_spell_pattern(rad_pat2,target,
			50 + adven[current_pc].items[ammo_inv_slot].ability_strength * 2,TRUE,current_pc);
		do_explosion_anim(5,0);
		end_missile_anim();
		handle_marked_damage();
		return;
		}
	
	if (dist(pc_pos[current_pc],target) > range)
		add_string_to_buf("  Out of range.");
		else if (can_see(pc_pos[current_pc],target,0) == 5)
			add_string_to_buf("  Can't see target.             ");
			else {
				combat_posing_monster = current_working_monster = current_pc;
				draw_terrain(2);
				void_sanctuary(current_pc);
				//play_sound((overall_mode == 12) ? 12 : 14);
				take_ap((overall_mode == 12) ? 3 : 2);
				missile_firer = current_pc;			
				r1 = rand_short(0,100) - 5 * hit_bonus - 10;
				r1 += 5 * (adven[current_pc].gaffect(affect::Webbed) / 3);
				r2 = rand_short(1,dam) + dam_bonus;
				add_string_to_buf("{} fires.", adven[current_pc].name); // debug

				switch (overall_mode) {
					case 13:
						switch (adven[current_pc].items[ammo_inv_slot].item_level) {
							case 7:m_type = 10;break;
							case 4:m_type = 1;break;
							case 8:m_type = 5;break;
							case 9:m_type = 7;break;
							default:m_type = 10;break;
							}
						break;
					case 12: case 14:
						m_type = is_magic(adven[current_pc].items[ammo_inv_slot]) ? 4 : 3;
						break; 
					}
				run_a_missile(pc_pos[current_pc],target,m_type,1,(overall_mode == 12) ? 12 : 14,
					0,0,100);

				if (r1 > skill_hit_chance(skill))
					add_string_to_buf("  Missed.");
					else if ((targ_monst = monst_there(target)) < T_M) {
						cur_monst = &c_town.monst.dudes[targ_monst];
						spec_dam = calc_spec_dam(adven[current_pc].items[ammo_inv_slot].ability,
							adven[current_pc].items[ammo_inv_slot].ability_strength,cur_monst);
						if (adven[current_pc].items[ammo_inv_slot].ability == 176) {
							ASB("  There is a flash of light.");
							cur_monst->m_d.health += r2;
							}
							else damage_monst(targ_monst, current_pc, r2, spec_dam, damage_type::Weapon, 13, TRUE);
						
						//if (adven[current_pc].items[ammo_inv_slot].ability == 33)
						//	hit_space(cur_monst->m_loc,get_ran(3,1,6),damage_type::Fire,1,1);

						// poison			
						if ((adven[current_pc].gaffect(affect::PoisonedWeapon) > 0) && (adven[current_pc].weap_poisoned == ammo_inv_slot)) {
								poison_amt = adven[current_pc].gaffect(affect::PoisonedWeapon);
								if (pc_has_abil_equip(adven[current_pc],51) < 24)
									poison_amt++;
								poison_monst(cur_monst,poison_amt);
							}
					
						}
						else hit_space(target,r2, damage_type::Weapon,1,0);
						
				}	

	combat_posing_monster = current_working_monster = -1;
	adven[current_pc].reduce_affect(affect::PoisonedWeapon);
	print_buf();////
}

// Select next active PC and, if necessary, run monsters
// if monsters go or PC switches (i.e. if need redraw above), return TRUE
Boolean combat_next_step()
{
	Boolean to_return = FALSE;
	short store_pc; // will print current pc name is active pc changes

	store_pc = current_pc;
	while (pick_next_pc() == TRUE) {
		combat_run_monst();
		set_pc_moves();	
		to_return = TRUE;
		// Safety valve
		if (adventurers_dead(adven))
			return TRUE;
		}
	pick_next_pc();
	if (current_pc != store_pc)
		to_return = TRUE;
	center = pc_pos[current_pc];		
	//if (ensure_redraw == TRUE)
	//	draw_terrain(0);
		
	adjust_spell_menus();
	
	// In case running monsters affected active PC...
/*	if (adven[current_pc].gaffect(affect::Speed) < 0)
		this_pc_hasted = FALSE;
	if ((adven[current_pc].main_status != status::Normal) || 
		((adven[current_pc].gaffect(affect::Speed) < 0) && (party.age % 2 == 0)))
		pick_next_pc();
	center = pc_pos[current_pc];		*/

	if ((combat_active_pc == 6) && (current_pc != store_pc)) {
			add_string_to_buf("Active:  {} (#{:d}, {:d} ap.)                     ",
				adven[current_pc].name,current_pc + 1,pc_moves[current_pc]);         
			print_buf();
		}
	if ((current_pc != store_pc) || (to_return == TRUE)) {
		put_pc_screen();
		set_stat_window(current_pc);
		}
	return to_return;
}

// Find next active PC, return TRUE is monsters need running, and run monsters is slow spells
// active
Boolean pick_next_pc()
{
	Boolean store = FALSE;
		
	if (current_pc == 6)
		current_pc = 0;
		
	// If current pc isn't active, fry its moves
	if ((combat_active_pc < 6) && (combat_active_pc != current_pc))
		pc_moves[current_pc] = 0;
					
	// Find next PC with moves
	while ((pc_moves[current_pc] <= 0) && (current_pc < 6)) {
		current_pc++;
		if ((combat_active_pc < 6) && (combat_active_pc != current_pc))
			pc_moves[current_pc] = 0;
		}

	// If run out of PC's, return to start and try again
	if (current_pc == 6) {
		current_pc = 0;
		while ((pc_moves[current_pc] <= 0) && (current_pc < 6)) {
			current_pc++;
			if ((combat_active_pc < 6) && (combat_active_pc != current_pc))
				pc_moves[current_pc] = 0;			
			}
		if (current_pc == 6) {
			store = TRUE;
			current_pc = 0;
			}
		}

	return store;
}


void combat_run_monst()
{
	short i,item,item_level;
	Boolean update_stat = FALSE;


		monsters_going = TRUE;
		do_monster_turn();
		monsters_going = FALSE;

		process_fields();
		party.light_level = move_to_zero(party.light_level);
		if ((which_combat_type == 1) && (c_town.town.lighting == 2))
			party.light_level = max (0,party.light_level - 9);
		if (c_town.town.lighting == 3)
			party.light_level = 0;

		party.stuff_done[305][2] = move_to_zero(party.stuff_done[305][2]);
		party.stuff_done[305][3] = move_to_zero(party.stuff_done[305][3]);

		// decrease monster present counter
		party.stuff_done[305][9] = move_to_zero(party.stuff_done[305][9]);

		dump_gold(1);

		party.age++;		
		if (party.age % 4 == 0)
			for (i = 0; i < 6; i++) {
			if ((adven[i].gaffect(affect::CursedBlessed) != 0) || (adven[i].gaffect(affect::Speed) != 0))
				update_stat = TRUE;
				adven[i].reduce_affect(affect::CursedBlessed);
				adven[i].reduce_affect(affect::Speed);
				party.stuff_done[305][0] = move_to_zero(party.stuff_done[305][0]);
				if ((item = pc_has_abil_equip(adven[i],50)) < 24) {
					update_stat = TRUE;
					pc_heal(adven[i],rand_short(0,adven[i].items[item].item_level + 1));
					}	
				}
		for (i = 0; i < 6; i++) 
			if (adven[i].main_status == status::Normal) {
			if ((adven[i].gaffect(affect::Invulnerable) != 0) || (adven[i].gaffect(affect::MagicResistant) != 0)
			 || (adven[i].gaffect(affect::Sanctuary) != 0)|| (adven[i].gaffect(affect::MartyrsShield) != 0)
			 || (adven[i].gaffect(affect::Asleep) != 0)|| (adven[i].gaffect(affect::Paralyzed) != 0))
				update_stat = TRUE;

				adven[i].reduce_affect(affect::Invulnerable);
				adven[i].reduce_affect(affect::MagicResistant);
				adven[i].reduce_affect(affect::Sanctuary);
				adven[i].reduce_affect(affect::MartyrsShield);
				adven[i].reduce_affect(affect::Asleep);
				adven[i].reduce_affect(affect::Paralyzed);

				// Do special items
				if (((item_level = pc_prot_level(adven[i],47)) > 0)
					&& (rand_short(0,10) == 5)) {
						update_stat = TRUE;
						adven[i].gaffect(affect::Speed) += item_level / 2;
						add_string_to_buf("An item hastes you!");
						}
				if ((item_level = pc_prot_level(adven[i],46)) > 0) {
					if (rand_short(0,10) == 5) {
						update_stat = TRUE;
						adven[i].gaffect(affect::CursedBlessed) += item_level / 2;
						add_string_to_buf("An item blesses you!");
						}
					}


			}

		special_increase_age();
		push_things();
					
		if (party.age % 2 == 0)
			do_poison();
		if (party.age % 3 == 0)
			handle_disease();
		handle_acid();

		if (update_stat == TRUE)
		put_pc_screen();
			
}


void do_monster_turn()
{
	Boolean acted_yet, had_monst = FALSE,printed_poison = FALSE,printed_disease = FALSE,printed_acid = FALSE;
	Boolean redraw_not_yet_done = TRUE;
	location targ_space = {0,0},move_targ,l;
	short i,j,k,num_monst,target,r1,move_target;
	creature_data_type *cur_monst;
	Boolean pc_adj[6];
	short abil_odds[40] = {0,5,7,6,6, 5,5,6,6,6, 6,0,0,0,4, 0,0,0,0,4, 8,0,0,7,0,
							0,0,0,0,0, 0,0,7,5,6, 0,0,0,0,0};
	 
	monsters_going = TRUE; // This affects how graphics are drawn.

	num_monst = T_M;
	if (overall_mode < 10) 
		which_combat_type = 1;
				
	for (i = 0; i < num_monst; i++) {  // Give monsters ap's, check activity
	
		cur_monst = &c_town.monst.dudes[i];
	
		// See if hostile monster notices party, during combat
		if ((cur_monst->active == 1) && (cur_monst->attitude % 2 == 1) && (overall_mode == 10)) {
			r1 = rand_short(1,100); // Check if see PCs first
			r1 += (party.stuff_done[305][0] > 0) ? 45 : 0;
			r1 += can_see(cur_monst->m_loc,closest_pc_loc(cur_monst->m_loc),0) * 10;
			if (r1 < 50)
				cur_monst->active = 2;
			
			for (j = 0; j < T_M; j++)
				if (monst_near(j,cur_monst->m_loc,5,1) == TRUE) {
					cur_monst->active = 2;		
					}
			}
		if ((cur_monst->active == 1) && (cur_monst->attitude % 2 == 1)) {
			// Now it looks for PC-friendly monsters
			// dist check is for efficiency
			for (j = 0; j < T_M; j++) 
				if ((c_town.monst.dudes[j].active > 0) && 
					(c_town.monst.dudes[j].attitude % 2 != 1) &&
					(dist(cur_monst->m_loc,c_town.monst.dudes[j].m_loc) <= 6) &&
				  (can_see(cur_monst->m_loc,c_town.monst.dudes[j].m_loc,0) < 5))
					cur_monst->active = 2;			
			}
						
		// See if friendly, fighting monster see hostile monster. If so, make mobile
		// dist check is for efficiency
		if ((cur_monst->active == 1) && (cur_monst->attitude == 2)) {
			for (j = 0; j < T_M; j++)
				if ((c_town.monst.dudes[j].active > 0) && (c_town.monst.dudes[j].attitude % 2 == 1) &&
		 		 (dist(cur_monst->m_loc,c_town.monst.dudes[j].m_loc) <= 6)	
		 		 && (can_see(cur_monst->m_loc,c_town.monst.dudes[j].m_loc,0) < 5)) {
					cur_monst->active = 2;	
					cur_monst->mobile = TRUE;	
					}
			}
		// End of seeing if monsters see others

		cur_monst->m_d.ap = 0;
		if (cur_monst->active == 2) { // Begin action loop for angry, active monsters
			// First note that hostile monsters are around.
			if (cur_monst->attitude % 2 == 1)
				party.stuff_done[305][9] = 30;
			
			// Give monster its action points
			cur_monst->m_d.ap = cur_monst->m_d.speed;
			if (is_town())
				cur_monst->m_d.ap = max(1,cur_monst->m_d.ap / 3);
			if (party.age % 2 == 0)
				if (cur_monst->gaffect(affect::Speed) < 0)
					cur_monst->m_d.ap = 0;
			if (cur_monst->m_d.ap > 0) { // adjust for webs
				cur_monst->m_d.ap = max(0,cur_monst->m_d.ap - cur_monst->gaffect(affect::Webbed) / 2);
				if (cur_monst->m_d.ap == 0) 
					cur_monst->gaffect(affect::Webbed) = max(0,cur_monst->gaffect(affect::Webbed) - 2);
				}
			if (cur_monst->gaffect(affect::Speed) > 0)
				cur_monst->m_d.ap *= 2;
			}
			if ((cur_monst->gaffect(affect::Asleep) > 0) || (cur_monst->gaffect(affect::Paralyzed) > 0))
				cur_monst->m_d.ap = 0;
			if (in_scen_debug == TRUE)
				cur_monst->m_d.ap = 0;
			center_on_monst = FALSE;

			// Now take care of summoned monsters
			if (cur_monst->active > 0) { 
				if ((cur_monst->summoned % 100) == 1) {
					cur_monst->active = 0;
					cur_monst->m_d.ap = 0;
					monst_spell_note(cur_monst->number,17);		
					}
				cur_monst->summoned = move_to_zero(cur_monst->summoned);
				}
	
		}
		
	for (i = 0; i < num_monst; i++) {  // Begin main monster loop, do monster actions
		// If party dead, no point
		if (adventurers_dead(adven))
			return;
			
		futzing = 0; // assume monster is fresh

		cur_monst = &c_town.monst.dudes[i];
		
		
		for (j = 0; j < 6; j++)
			if ((adven[j].main_status == status::Normal) && (monst_adjacent(pc_pos[j],i) == TRUE))
				pc_adj[j] = TRUE;
				else pc_adj[j] = FALSE; 


			
			while ((cur_monst->m_d.ap > 0) && (cur_monst->active > 0)) {  // Spend each action point

					if (is_combat()) { // Pick target. If in town, target already picked
					// in do_monsters
							target = monst_pick_target(i);
							target = switch_target_to_adjacent(i,target);
							if (target < 6)
								targ_space = pc_pos[target];
								else if (target != 6)
									targ_space = c_town.monst.dudes[target - 100].m_loc;	
						monst_target[i] = target;
						}
						else {
							if (monst_target[i] < 6)
								targ_space = c_town.p_loc;
								else if (monst_target[i] != 6)
									targ_space = c_town.monst.dudes[monst_target[i] - 100].m_loc;	
						}
						
		//			add_string_to_buf("  {:d} targets {:d}.",i,target);

					if ((monst_target[i] < 0) || ((monst_target[i] > 5) && (monst_target[i] < 100)))
						monst_target[i] = 6;
					target = monst_target[i];

				// Now if in town and monster about to attack, do a redraw, so we see monster
				// in right place
				if ((target != 6) && is_town() && (redraw_not_yet_done == TRUE)
					&& (party_can_see_monst(i) == TRUE)) {
					draw_terrain(0);					
					redraw_not_yet_done = FALSE;
					}
					
				// Draw w. monster in center, if can see	
				if ((cur_monst->m_d.ap > 0) && is_combat()
					// First make sure it has a target and is close, if not, don't bother
					&& (cur_monst->attitude > 0) && (cur_monst->m_d.picture_num > 0) 
					&& ((target != 6) || (cur_monst->attitude % 2 == 1)) 
					&& (party_can_see_monst(i) == TRUE) ) {
					center_on_monst = TRUE;
					center = cur_monst->m_loc;
					draw_terrain(0);
					pause((PSD[306][6] == 3) ? 9 : PSD[306][6]);
					}
					
					combat_posing_monster = current_working_monster = 100 + i;

					acted_yet = FALSE;
					

					// Now the monster, if evil, looks at the situation and maybe picks a tactic.
					// This only happens when there is >1 a.p. left, and tends to involve
					// running to a nice position.
					current_monst_tactic = 0;
					if ((target != 6) && (cur_monst->m_d.ap > 1) && (futzing == 0)) {
						l = closest_pc_loc(cur_monst->m_loc);
						if (((cur_monst->m_d.mu > 0) || (cur_monst->m_d.cl > 0)) &&
						(dist(cur_monst->m_loc,l) < 5) && (monst_adjacent(l,i) == FALSE))
							current_monst_tactic = 1; // this means flee
						
								
						if ( (((cur_monst->m_d.spec_skill > 0) && (cur_monst->m_d.spec_skill < 4))
							 || (cur_monst->m_d.spec_skill == 20)) && // Archer?
						(dist(cur_monst->m_loc,targ_space) < 6) && 
						 (monst_adjacent(targ_space,i) == FALSE))
							current_monst_tactic = 1; // this means flee
						}	
						

					// flee
					if ((monst_target[i] < 6) && (((cur_monst->m_d.morale <= 0)
						&& (cur_monst->m_d.spec_skill != 13) && (cur_monst->m_d.m_type != 8))
						|| (current_monst_tactic == 1))) {
						if (cur_monst->m_d.morale < 0)
							cur_monst->m_d.morale++;
						if (cur_monst->m_d.health > 50)
							cur_monst->m_d.morale++;
						r1 = rand_short(1,6);
						if (r1 == 3)
							cur_monst->m_d.morale++;
/*crash*/			if ((adven[monst_target[i]].main_status == status::Normal)	&& (cur_monst->mobile == TRUE)) {
							acted_yet = flee_party (i,cur_monst->m_loc,targ_space);
							if (acted_yet == TRUE) take_m_ap(1,cur_monst);
							}
						}
					if ((target != 6) && (cur_monst->attitude > 0) 
					 && (monst_can_see(i,targ_space) == TRUE)
					 && (can_see_monst(targ_space,i) == TRUE)) { // Begin spec. attacks

//	add_string_to_buf("{:d}: {:d}  {:d}  {:d}",i,cur_monst->m_d.breath,cur_monst->m_d.mu,cur_monst->m_d.cl);

					// Breathe (fire)
						if ( (cur_monst->m_d.breath > 0)
							&& (rand_short(1,8) < 4) && (acted_yet == FALSE)) {
								//print_nums(cur_monst->m_d.breath,cur_monst->m_d.breath_type,dist(cur_monst->m_loc,targ_space) );
								if ((target != 6)
									&& (dist(cur_monst->m_loc,targ_space) <= 8)) {
										acted_yet = monst_breathe(cur_monst,targ_space,cur_monst->m_d.breath_type);
									had_monst = TRUE;
									acted_yet = TRUE;
									take_m_ap(4,cur_monst);
									}
							}
					// Mage spell
						if ((cur_monst->m_d.mu > 0) && (rand_short(1,10) < ((cur_monst->m_d.cl > 0) ? 6 : 9) )
							&& (acted_yet == FALSE)) {
								if (((monst_adjacent(targ_space,i) == FALSE) || (rand_short(0,2) < 2) || (cur_monst->number >= 160)
									|| (cur_monst->m_d.level > 9))
										&& (dist(cur_monst->m_loc,targ_space) <= 10)) {	
											acted_yet = monst_cast_mage(cur_monst,target);
											had_monst = TRUE;
											acted_yet = TRUE;
											take_m_ap(5,cur_monst);
									}
							}
					// Priest spell
						if ((cur_monst->m_d.cl > 0) && (rand_short(1,8) < 7) && (acted_yet == FALSE)) {
								if (((monst_adjacent(targ_space,i) == FALSE) || (rand_short(0,2) < 2) || (cur_monst->m_d.level > 9))
										&& (dist(cur_monst->m_loc,targ_space) <= 10)) {
											acted_yet = monst_cast_priest(cur_monst,target);
											had_monst = TRUE;
											acted_yet = TRUE;
											take_m_ap(4,cur_monst);
									}
							}

				// Missile
						if ((abil_range[cur_monst->m_d.spec_skill] > 0) // breathing gas short range
							&& (rand_short(1,8) < abil_odds[cur_monst->m_d.spec_skill]) && (acted_yet == FALSE)) {
								// Don't fire when adjacent, unless non-gaze magical attack 
								if (((monst_adjacent(targ_space,i) == FALSE) || 
									((cur_monst->m_d.spec_skill > 7) && (cur_monst->m_d.spec_skill != 20)
									&& (cur_monst->m_d.spec_skill != 33)))
									// missile range 
									&& (dist(cur_monst->m_loc,targ_space) <= abil_range[cur_monst->m_d.spec_skill])) {
									print_monst_name(cur_monst->number);
									monst_fire_missile(i,cur_monst->m_d.skill,cur_monst->gaffect(affect::CursedBlessed),
										cur_monst->m_d.spec_skill,cur_monst->m_loc,target);

									// Vapors don't count as action
									if ((cur_monst->m_d.spec_skill == 11) || (cur_monst->m_d.spec_skill == 7) ||
									  (cur_monst->m_d.spec_skill == 20))
										take_m_ap(2,cur_monst);
										else if (cur_monst->m_d.spec_skill == 10)
											take_m_ap(1,cur_monst);
											else take_m_ap(3,cur_monst);
									had_monst = TRUE;
									acted_yet = TRUE;
									}
							}
						} // Special attacks
						
					// Attack pc
					if ((monst_target[i] < 6) && (adven[monst_target[i]].main_status == status::Normal) 
						&& (monst_adjacent(targ_space,i) == TRUE)  && (cur_monst->attitude % 2 == 1)
						 && (acted_yet == FALSE)) {
							monster_attack_pc(i,monst_target[i]);
							take_m_ap(4,cur_monst);
							acted_yet = TRUE;
							had_monst = TRUE;
						}
					// Attack monst
					if ((monst_target[i] >= 100) && (c_town.monst.dudes[monst_target[i] - 100].active > 0) 
						&& (monst_adjacent(targ_space,i) == TRUE)  && (cur_monst->attitude > 0)
						 && (acted_yet == FALSE)) {
							monster_attack_monster(i,monst_target[i] - 100);
							take_m_ap(4,cur_monst);
							acted_yet = TRUE;
							had_monst = TRUE;
						}
					
					if (acted_yet == TRUE) {
							print_buf();
							if (j == 0)
								pause(8);
							FlushEvents(2);						
						}
					
					if (overall_mode == 10) {
						if ((acted_yet == FALSE) && (cur_monst->mobile == TRUE)) {  // move monst
							move_target = (monst_target[i] != 6) ? monst_target[i] : closest_pc(cur_monst->m_loc);
							if (monst_hate_spot(i,&move_targ) == TRUE) // First, maybe move out of dangerous space
								seek_party (i,cur_monst->m_loc,move_targ);
								else { // spot is OK, so go nuts
								if ((cur_monst->attitude % 2 == 1) && (move_target < 6)) // Monsters seeking party do so
									if (adven[move_target].main_status == status::Normal) {
										seek_party (i,cur_monst->m_loc,pc_pos[move_target]);
										for (k = 0; k < 6; k++)
											if ((pc_parry[k] > 99) && (monst_adjacent(pc_pos[k],i) == TRUE)
											&& (cur_monst->active > 0)) {
												pc_parry[k] = 0;
												pc_attack(k,i);
												}
											}

								if (move_target >= 100) // Monsters seeking monsters do so
									if (c_town.monst.dudes[move_target - 100].active > 0) {
										seek_party (i,cur_monst->m_loc,c_town.monst.dudes[move_target - 100].m_loc);
										for (k = 0; k < 6; k++)
											if ((pc_parry[k] > 99) && (monst_adjacent(pc_pos[k],i) == TRUE)
												&& (cur_monst->active > 0) && (cur_monst->attitude % 2 == 1)) {
												pc_parry[k] = 0;
												pc_attack(k,i);
												}
										}
																			
								if (cur_monst->attitude == 0) {
									acted_yet = rand_move (i);
									futzing++;
									}
								}
							take_m_ap(1,cur_monst);
							}
						if ((acted_yet == FALSE) && (cur_monst->mobile == FALSE)) { // drain action points
							take_m_ap(1,cur_monst);
							futzing++;
							}
						}
						else if (acted_yet == FALSE) {
							take_m_ap(1,cur_monst);
							futzing++;
							}

					// pcs attack any fleeing monsters
					if ((overall_mode >= 10) && (overall_mode < 20))
						for (k = 0; k < 6; k++)
							if ((adven[k].main_status == status::Normal) && (monst_adjacent(pc_pos[k],i) == FALSE)
								&& (pc_adj[k] == TRUE) && (cur_monst->attitude % 2 == 1) && (cur_monst->active > 0) &&
								(adven[k].gaffect(affect::Sanctuary) == 0)) {
									combat_posing_monster = current_working_monster = k;
									pc_attack(k,i);
									combat_posing_monster = current_working_monster = 100 + i;
									pc_adj[k] = FALSE;
									}
				
				// Place fields for monsters that create them. Only done when monst sees foe
				if ((target != 6) && (can_see(cur_monst->m_loc,targ_space,0) < 5)) { ////
					if ((cur_monst->m_d.radiate_1 == 1) && (rand_short(0,100) < cur_monst->m_d.radiate_2))
						place_spell_pattern(square,cur_monst->m_loc,5,FALSE,7);
					if ((cur_monst->m_d.radiate_1 == 2) && (rand_short(0,100) < cur_monst->m_d.radiate_2))
						place_spell_pattern(square,cur_monst->m_loc,8,FALSE,7);
					if ((cur_monst->m_d.radiate_1 == 3) && (rand_short(0,100) < cur_monst->m_d.radiate_2))
						place_spell_pattern(square,cur_monst->m_loc,4,FALSE,7);
					if ((cur_monst->m_d.radiate_1 == 4) && (rand_short(0,100) < cur_monst->m_d.radiate_2))
						place_spell_pattern(square,cur_monst->m_loc,6,FALSE,7);
					if ((cur_monst->m_d.radiate_1 == 5) && (rand_short(0,100) < cur_monst->m_d.radiate_2))
						place_spell_pattern(square,cur_monst->m_loc,12,FALSE,7);
					if ((cur_monst->m_d.radiate_1 == 6) && (rand_short(0,100) < cur_monst->m_d.radiate_2))
						place_spell_pattern(square,cur_monst->m_loc,7,FALSE,7);
					if ((cur_monst->m_d.radiate_1 == 10) && (rand_short(0,100) < 5)){
						if (summon_monster(cur_monst->m_d.radiate_2,
							cur_monst->m_loc,130,cur_monst->attitude) == TRUE)
							{monst_spell_note(cur_monst->number,33); play_sound(61);}
						}
					if ((cur_monst->m_d.radiate_1 == 11) && (rand_short(0,100) < 20)){
						if (summon_monster(cur_monst->m_d.radiate_2,
							cur_monst->m_loc,130,cur_monst->attitude) == TRUE)
							{monst_spell_note(cur_monst->number,33); play_sound(61);}
						}
					if ((cur_monst->m_d.radiate_1 == 12) && (rand_short(0,100) < 50)){
						if (summon_monster(cur_monst->m_d.radiate_2,
							cur_monst->m_loc,130,cur_monst->attitude) == TRUE)
							{monst_spell_note(cur_monst->number,33); play_sound(61);}
						}
					}
					
				combat_posing_monster = current_working_monster = -1;
				// Redraw monster after it goes
				if ((cur_monst->attitude > 0) && (cur_monst->active > 0) && (cur_monst->m_d.ap == 0)
				 && is_combat() && (cur_monst->m_d.picture_num > 0) && (party_can_see_monst(i)  == TRUE)) {
					center = cur_monst->m_loc;
					draw_terrain(0);

					}
				
				// If monster dead, take away actions
				if (cur_monst->active == 0)
					cur_monst->m_d.ap = 0;

				//if ((futzing == 1) && (rand_short(0,1) == 0)) // If monster's just pissing around, give up
				//	cur_monst->m_d.ap = 0;
				if (futzing > 1) // If monster's just pissing around, give up
					cur_monst->m_d.ap = 0;
				}  // End of monster action loop
				
			
		}

	for (i = 0; i < num_monst; i++) {  // Begin monster time stuff loop
		// If party dead, no point
		if (adventurers_dead(adven))
			return;

		cur_monst = &c_town.monst.dudes[i];
		
		if ((cur_monst->active < 0) || (cur_monst->active > 2)) 
			cur_monst->active = 0; // clean up
				if (cur_monst->active != 0) { // Take care of monster effects
						if (cur_monst->gaffect(affect::Acid) > 0) {  // Acid
							if (printed_acid == FALSE) {
								add_string_to_buf("Acid:              ");
								printed_acid = TRUE;
								}
							r1 = get_ran(cur_monst->gaffect(affect::Acid),1,6);
							damage_monst(i, 6,r1, 0, damage_type::GeneralMagic);
							cur_monst->gaffect(affect::Acid)--;
							}
						
						if (cur_monst->gaffect(affect::Asleep) == 1)
							monst_spell_note(cur_monst->number,29);
						cur_monst->gaffect(affect::Asleep) = move_to_zero(cur_monst->gaffect(affect::Asleep));
						cur_monst->gaffect(affect::Paralyzed) = move_to_zero(cur_monst->gaffect(affect::Paralyzed));

						if (party.age % 2 == 0) {
							cur_monst->gaffect(affect::CursedBlessed) = move_to_zero(cur_monst->gaffect(affect::CursedBlessed));
							cur_monst->gaffect(affect::Speed) = move_to_zero(cur_monst->gaffect(affect::Speed));
							cur_monst->gaffect(affect::Webbed) = move_to_zero(cur_monst->gaffect(affect::Webbed));
							
							if (cur_monst->gaffect(affect::Poisoned) > 0) {  // Poison
								if (printed_poison == FALSE) {
									add_string_to_buf("Poisoned monsters:              ");
									printed_poison = TRUE;
									}
								r1 = get_ran(cur_monst->gaffect(affect::Poisoned),1,6);
								damage_monst(i, 6, r1, 0, damage_type::Poison);
								cur_monst->gaffect(affect::Poisoned)--;
								}
							if (cur_monst->gaffect(affect::Diseased) > 0) {  // Disease
								if (printed_disease == FALSE) {
									add_string_to_buf("Diseased monsters:              ");
									printed_disease = TRUE;
									}
								k = rand_short(1,5);
								switch (k) {
									case 1: case 2: poison_monst(cur_monst, 2);break;
									case 3:	slow_monst(cur_monst,2); break;
									case 4: curse_monst(cur_monst,2); break;
									case 5: scare_monst(cur_monst,10); break;					
									}
								if (rand_short(1,6) < 4)
									cur_monst->gaffect(affect::Diseased)--;
								}

							}

						if (party.age % 4 == 0) {
							if (cur_monst->m_d.mp < cur_monst->m_d.max_mp)
								cur_monst->m_d.mp += 2;
							cur_monst->gaffect(affect::Dumbfounded) = move_to_zero(cur_monst->gaffect(affect::Dumbfounded));
							}
					} // end take care of monsters			
		}
		
	// If in town, need to restore center
	if (overall_mode < 10)
		center = c_town.p_loc;
	if (had_monst == TRUE)
		put_pc_screen();
	for (i = 0; i < 6; i++)
		pc_parry[i] = 0;

	monsters_going = FALSE;
}

void monster_attack_pc(short who_att,short target)
{
	creature_data_type *attacker;
	short r1,r2,i, store_hp,sound_type = 0;
	damage_type dam_type = damage_type::Weapon;
	

	attacker = &c_town.monst.dudes[who_att];

	// A peaceful monster won't attack
	if (attacker->attitude % 2 != 1)
		return;

	// Draw attacker frames
	if (is_combat() 
		&& ((center_on_monst == TRUE) || (monsters_going == FALSE))) {
		if (attacker->m_d.spec_skill != 11)
		frame_space(attacker->m_loc,0,attacker->m_d.x_width,attacker->m_d.y_width);
		frame_space(pc_pos[target],1,1,1);	
		}

		

	if ((attacker->m_d.a[0] != 0) || (attacker->m_d.a[2] != 0))
			print_monst_attacks(attacker->number,target);

	// Check sanctuary
	if (adven[target].gaffect(affect::Sanctuary) > 0) {
		r1 = rand_short(0,100);
		if (r1 > skill_hit_chance(attacker->m_d.level / 2)) {
			add_string_to_buf("  Can't find target!                 ");		
			}
		return;
		}

	for (i = 0; i < 3; i++) {
		if ((attacker->m_d.a[i] > 0) && (adven[target].main_status == status::Normal)) {
//			add_string_to_buf("  Attacks {}.", adven[target].name);

			// Attack roll
			r1 = rand_short(0,100) - 5 * min(8,attacker->gaffect(affect::CursedBlessed)) + 5 * adven[target].gaffect(affect::CursedBlessed)
					+ 5 * pc_stat_adj(adven[target], skill::Dexterity) - 15;
			r1 += 5 * (attacker->gaffect(affect::Webbed) / 3);
			if (pc_parry[target] < 100)
				r1 += 5 * pc_parry[target];
			
			// Damage roll
			r2 = get_ran(attacker->m_d.a[i] / 100 + 1,1,attacker->m_d.a[i] % 100) 
				+ min(8,attacker->gaffect(affect::CursedBlessed)) - adven[target].gaffect(affect::CursedBlessed) + 1;
			if (difficulty_adjust() > 2)
				r2 = r2 * 2;
			if (difficulty_adjust() == 2)
				r2 = (r2 * 3) / 2;
				
			if ((adven[target].gaffect(affect::Asleep) > 0) || (adven[target].gaffect(affect::Paralyzed) > 0)) {
				r1 -= 80;
				r2 = r2 * 2;
				}

			draw_terrain(2);
			// Check if hit, and do effects
			if (r1 <= skill_hit_chance((attacker->m_d.skill + 4) / 2)) {
					if (attacker->m_d.m_type == 7)
						dam_type = damage_type::DemonAttack;
					if (attacker->m_d.m_type == 8)
						dam_type = damage_type::UndeadAttack;
		
					store_hp = adven[target].cur_health;
					sound_type = get_monst_sound(attacker,i);

					if ((damage_pc(target,r2,dam_type,attacker->m_d.m_type, sound_type, FALSE) == TRUE) &&
						(store_hp - adven[target].cur_health > 0)) {
						damaged_message(store_hp - adven[target].cur_health,
						 (i > 0) ? attacker->m_d.a23_type : attacker->m_d.a1_type);
						 
						if (adven[target].gaffect(affect::MartyrsShield) > 0) {
							add_string_to_buf("  Shares damage!                 ");
							damage_monst(who_att, 6, store_hp - adven[target].cur_health, 0, damage_type::GeneralMagic);
							}
						 
						if ((attacker->m_d.poison > 0) && (i == 0)) {
								poison_pc(target,attacker->m_d.poison);
							}

						// Gremlin
						if ((attacker->m_d.spec_skill == 21) && (rand_short(0,2) < 2)) {
							add_string_to_buf("  Steals food!                 ");
							print_buf();
							play_sound(26);
							party.food = (long) max(0, (short) (party.food) - rand_short(0,10) - 10);
							put_pc_screen();
							}
							
						// Disease
						if (((attacker->m_d.spec_skill == 25) || (attacker->m_d.spec_skill == 30))
						 && (rand_short(0,2) < 2)) {
							add_string_to_buf("  Causes disease!                 ");
							print_buf();
							disease_pc(target,(attacker->m_d.spec_skill == 25) ? 6 : 2);
							}
							
						// Undead xp drain							
						if (((attacker->m_d.spec_skill == 16) || (attacker->m_d.spec_skill == 17))
						  && (pc_has_abil_equip(adven[target],48) == 24)) {
							add_string_to_buf("  Drains life!                 ");
							drain_pc(target,(attacker->m_d.level * 3) / 2);
							put_pc_screen();
							}

						// Undead slow
						if ((attacker->m_d.spec_skill == 18) && (rand_short(0,8) < 6) && (pc_has_abil_equip(adven[target],48) == 24)) {
							add_string_to_buf("  Stuns! ");
							slow_pc(target,2);
							put_pc_screen();
							}
						// Dumbfound target
						if (attacker->m_d.spec_skill == 24) {
							add_string_to_buf("  Dumbfounds! ");
							dumbfound_pc(target,2);
							put_pc_screen();
							}

						// Web target
						if (attacker->m_d.spec_skill == 27) {
							add_string_to_buf("  Webs!                    ");
							web_pc(target,5);
							put_pc_screen();
							}
						// Sleep target
						if (attacker->m_d.spec_skill == 28) {
							add_string_to_buf("  Sleeps!                    ");
							sleep_pc(target,6, affect::Asleep,-15);
							put_pc_screen();
							}
						// Paralyze target
						if (attacker->m_d.spec_skill == 29) {
							add_string_to_buf("  Paralysis touch!                    ");
							sleep_pc(target,500, affect::Paralyzed,-5);
							put_pc_screen();
							}
						// Acid touch
						if (attacker->m_d.spec_skill == 31)  {
							add_string_to_buf("  Acid touch!      ");
							acid_pc(target,(attacker->m_d.level > 20) ? 4 : 2);
							}

						// Freezing touch
						if (((attacker->m_d.spec_skill == 15) || (attacker->m_d.spec_skill == 17))
						 && (rand_short(0,8) < 6) && (pc_has_abil_equip(adven[target],48) == 24)) {
							add_string_to_buf("  Freezing touch!");
							r1 = get_ran(3,1,10);
							damage_pc(target,r1, damage_type::Cold,-1);
							}
						// Killing touch
						if (attacker->m_d.spec_skill == 35)
							 {
							add_string_to_buf("  Killing touch!");
							r1 = get_ran(20,1,10);
							damage_pc(target,r1, damage_type::Unblockable,-1);
							}
						}	
				}
				else {
					add_string_to_buf("  Misses.");
					play_sound(2);
				}
		combat_posing_monster = -1;
		draw_terrain(2);
		combat_posing_monster = 100 + who_att;
		
		
		}
		if (adven[target].main_status != status::Normal)
			i = 3;
	}

}

void monster_attack_monster(short who_att,short attackee)
{
	creature_data_type *attacker,*target;
	short r1,r2,i,store_hp,sound_type = 0;
	damage_type dam_type = damage_type::Weapon;

	attacker = &c_town.monst.dudes[who_att];
	target = &c_town.monst.dudes[attackee];

	// Draw attacker frames
	if (is_combat() 
		&& ((center_on_monst == TRUE) || (monsters_going == FALSE))) {
		if (attacker->m_d.spec_skill != 11)
		frame_space(attacker->m_loc,0,attacker->m_d.x_width,attacker->m_d.y_width);
		frame_space(target->m_loc,1,1,1);	
		}


	if ((attacker->m_d.a[1] != 0) || (attacker->m_d.a[0] != 0))
			print_monst_attacks(attacker->number,100 + attackee);
	for (i = 0; i < 3; i++) {
		if ((attacker->m_d.a[i] > 0) && (target->active != 0)) {
//			add_string_to_buf("  Attacks {}.", adven[target].name);

			// if friendly to party, make able to attack
			if (target->attitude == 0)
				target->attitude = 2;

			// Attack roll
			r1 = rand_short(0,100) - 5 * min(10,attacker->gaffect(affect::CursedBlessed)) 
				+ 5 * target->gaffect(affect::CursedBlessed)	- 15;
			r1 += 5 * (attacker->gaffect(affect::Webbed) / 3);

			// Damage roll
			r2 = get_ran(attacker->m_d.a[i] / 100 + 1,1,attacker->m_d.a[i] % 100) 
				+ min(10,attacker->gaffect(affect::CursedBlessed)) - target->gaffect(affect::CursedBlessed) + 2;

			if ((target->gaffect(affect::Asleep) > 0) || (target->gaffect(affect::Paralyzed) > 0)) {
				r1 -= 80;
				r2 = r2 * 2;
				}

			draw_terrain(2);
			// Check if hit, and do effects
			if (r1 <= skill_hit_chance((attacker->m_d.skill + 4) / 2)) {
					if (attacker->m_d.m_type == 7)
						dam_type = damage_type::DemonAttack;
					if (attacker->m_d.m_type == 8)
						dam_type = damage_type::UndeadAttack;
					store_hp = target->m_d.health;
					
					sound_type = get_monst_sound(attacker,i);
					
					if (damage_monst(attackee,7,r2,0,dam_type, sound_type, FALSE) == TRUE) {
						damaged_message(store_hp - target->m_d.health,
						 (i > 0) ? attacker->m_d.a23_type : attacker->m_d.a1_type);
						 
						if ((attacker->m_d.poison > 0) && (i == 0)) {
								poison_monst(target,attacker->m_d.poison);
							}

						// Undead slow
						if ((attacker->m_d.spec_skill == 18) && (rand_short(0,8) < 6)) {
							add_string_to_buf("  Stuns!      ");
							slow_monst(target,2);
							}

						// Web target
						if (attacker->m_d.spec_skill == 27)  {
							add_string_to_buf("  Webs!      ");
							web_monst(target,4);
							}
						// Sleep target
						if (attacker->m_d.spec_skill == 28)  {
							add_string_to_buf("  Sleeps!      ");
							charm_monst(target,-15,11,6);
							}
						// Dumbfound target
						if (attacker->m_d.spec_skill == 24)  {
							add_string_to_buf("  Dumbfounds!      ");
							dumbfound_monst(target,2);
							}
						// Paralyze target
						if (attacker->m_d.spec_skill == 29)  {
							add_string_to_buf("  Paralysis touch!      ");
							charm_monst(target,-5,12,500);
							}
						// Acid touch
						if (attacker->m_d.spec_skill == 31)  {
							add_string_to_buf("  Acid touch!      ");
							acid_monst(target,3);
							}

						// Freezing touch
						if (((attacker->m_d.spec_skill == 15) || (attacker->m_d.spec_skill == 17))
						 && (rand_short(0,8) < 6)) {
							add_string_to_buf("  Freezing touch!");
							r1 = get_ran(3,1,10);
							damage_monst(attackee,7,r1,0, damage_type::Cold);
							}
							
						// Death touch
						if ((attacker->m_d.spec_skill == 35)
						 && (rand_short(0,8) < 6)) {
							add_string_to_buf("  Killing touch!");
							r1 = get_ran(20,1,10);
							damage_monst(attackee,7,r1,0, damage_type::Unblockable);
							}
						}
				}
				else {
					add_string_to_buf("  Misses.");
					play_sound(2);
				}
		combat_posing_monster = -1;
		draw_terrain(2);
		combat_posing_monster = 100 + who_att;
		}
		if (target->active == 0)
			i = 3;
	}

}


void monst_fire_missile(short m_num,short skill,short bless,short level,location source,short target)
//short target; // 100 +  - monster is target
{
	creature_data_type *m_target;
	short r1,r2,dam[40] = {0,1,2,3,4, 6,8,7,0,0, 0,0,0,0,0, 0,0,0,0,0, 
		8,0,0,0,0, 0,0,0,0,0, 0,0,0,0,6, 0,0,0,0,0},i,j;
	location targ_space;
	
	if (target == 6)
		return;
	if (target >= 100) {
		targ_space = c_town.monst.dudes[target - 100].m_loc;
		if (c_town.monst.dudes[target - 100].active == 0)
			return;
		}
		else {
			targ_space = is_combat() ? pc_pos[target] : c_town.p_loc;
			if (adven[target].main_status != status::Normal)
				return;
			}
		
	if (target >= 100)
		m_target = &c_town.monst.dudes[target - 100];
	if (((overall_mode >= 10) && (overall_mode <= 20)) && (center_on_monst == TRUE)) {
		frame_space(source,0,c_town.monst.dudes[m_num].m_d.x_width,c_town.monst.dudes[m_num].m_d.y_width);
		if (target >= 100)
			frame_space(targ_space,1,m_target->m_d.x_width,m_target->m_d.y_width);
			else frame_space(targ_space,1,1,1);
		}
	
	draw_terrain(2);
	if (level == 32) { // sleep cloud
		ASB("Creature breathes.");
		run_a_missile(source,targ_space,0,0,44,
			0,0,100);
		place_spell_pattern(rad_pat2,targ_space,12,FALSE,7);		
		}
	else if (level == 14) { // vapors
			//play_sound(44);
			if (target < 100) { // on PC
				add_string_to_buf("  Breathes on {}.                  ", adven[target].name);
				}
				else {  // on monst
					add_string_to_buf("  Breathes vapors.");
					}
			run_a_missile(source,targ_space,12,0,44,
				0,0,100);
			scloud_space(targ_space.x,targ_space.y);
		}
	else if (level == 19) { // webs
			//play_sound(14);
			if (target < 100) { // on PC
				add_string_to_buf("  Throws web at {}.                  ", adven[target].name);
				}
				else {  // on monst
					add_string_to_buf("  Throws web.");
					}
			run_a_missile(source,targ_space,8,0,14,
				0,0,100);
			web_space(targ_space.x,targ_space.y);
		}
	else if (level == 23) { // paral
			play_sound(51);
			if (target < 100) { // on PC
				add_string_to_buf("  Fires ray at {}.                  ", adven[target].name);
				sleep_pc(target,100, affect::Paralyzed,0);
				}
				else {  // on monst
					add_string_to_buf("  Shoots a ray.");
					charm_monst(m_target,0,12,100);
					}
			//run_a_missile(source,targ_space,8,0,14,
			//	0,0,100);
			//web_space(targ_space.x,targ_space.y);
		}
		else if (level == 8) { // petrify
			//play_sound(43);
			run_a_missile(source,targ_space,14,0,43,0,0,100);
			if (target < 100) { // on PC
				add_string_to_buf("  Gazes at {}.                  ", adven[target].name);
				r1 = rand_short(0,20) + adven[target].level / 4 + adven[target].gaffect(affect::CursedBlessed);
				if (pc_has_abil_equip(adven[target],49) < 24)
					r1 = 20;
				if (r1 > 14) {
						add_string_to_buf("  {} resists.                  ", adven[target].name);
					}
					else {
						add_string_to_buf("  {} is turned to stone.                  ", adven[target].name);
						kill_pc(target, status::Stone);
						}
				}
				else {
					monst_spell_note(m_target->number,9);
					r1 = rand_short(0,20) + m_target->m_d.level / 4 + m_target->gaffect(affect::CursedBlessed);
					if ((r1 > 14) || (m_target->m_d.immunities & 2)) 
						monst_spell_note(m_target->number,10);
						else {
							monst_spell_note(m_target->number,8);
							kill_monst(m_target,7);
							}				
					}
		}
		else if (level == 9) { /// Drain sp
			if (target < 100) { // pc
				// modify target is target has no sp
				if (adven[target].cur_sp < 4) {
					for (i = 0; i < 8; i++) {
						j = rand_short(0,5);
						if ((adven[j].main_status == status::Normal) && (adven[j].cur_sp > 4) &&
							(can_see(source,pc_pos[j],0) < 5) && (dist(source,pc_pos[j]) <= 8)) {
								target = j;
								i = 8;
								targ_space = pc_pos[target];
								}
						}
								
					}
				run_a_missile(source,targ_space,8,0,43,0,0,100);
				add_string_to_buf("  Drains {}.                  ", adven[target].name);
				adven[target].cur_sp = adven[target].cur_sp / 2;
				}
				else { // on monst
				run_a_missile(source,targ_space,8,0,43,0,0,100);
					monst_spell_note(m_target->number,11);
					if (m_target->m_d.mp >= 4)
						m_target->m_d.mp = m_target->m_d.mp / 2;
						else m_target->m_d.skill = 1;
					}
			}
		else if (level == 10) { // heat ray
			run_a_missile(source,targ_space,13,0,51,
				0,0,100);
			r1 = get_ran(7,1,6);
			start_missile_anim();
			if (target < 100) { // pc
				add_string_to_buf("  Hits {} with heat ray.", adven[target].name);
				damage_pc(target,r1, damage_type::Fire,-1);
				}
				else { // on monst
					add_string_to_buf("  Fires heat ray.");
					damage_monst(target - 100,7,r1,0, damage_type::Fire);
					}		
			do_explosion_anim(5,0);
			end_missile_anim();
			handle_marked_damage();
			}
		else if (level == 33) { // acid spit
			run_a_missile(source,targ_space,0,1,64,
				0,0,100);
			//play_sound(64);
			if (target < 100) { // pc
				add_string_to_buf("  Spits acid on {}.", adven[target].name);
				acid_pc(target,6);
				}
				else { // on monst
					add_string_to_buf("  Spits acid.");
					acid_monst(m_target,6);
					}		
			}
		else if (target < 100) {  // missile on PC

			switch (level) {
				case 1: case 2: case 20:
					run_a_missile(source,targ_space,3,1,12,0,0,100);
					add_string_to_buf("  Shoots at {}.", adven[target].name);
				break;
				case 3:
					run_a_missile(source,targ_space,5,1,14,0,0,100);
					add_string_to_buf("  Throws spear at {}.", adven[target].name);
				break;
				case 7:
					run_a_missile(source,targ_space,7,1,14,0,0,100);
					add_string_to_buf("  Throws razordisk at {}.", adven[target].name);
				break;		
				case 34:
					run_a_missile(source,targ_space,5,1,14,0,0,100);
					add_string_to_buf("  Fires spines at {}.", adven[target].name);
				break;		
				default:
					run_a_missile(source,targ_space,12,1,14,0,0,100);
					add_string_to_buf("  Throws rock at {}.", adven[target].name);
				break;
				}
	
			// Check sanctuary
			if (adven[target].gaffect(affect::Sanctuary) > 0) {
				r1 = rand_short(0,100);
				if (r1 > skill_hit_chance(level)) {
					add_string_to_buf("  Can't find target!                 ");		
					}
				return;
				}

			r1 = rand_short(0,100) - 5 * min(10,bless) + 5 * adven[target].gaffect(affect::CursedBlessed)
				- 5 * (can_see(source, pc_pos[target],0));
			if (pc_parry[target] < 100)
				r1 += 5 * pc_parry[target];
			r2 = get_ran(dam[level],1,7) + min(10,bless);

			if (r1 <= skill_hit_chance(dam[level] * 2)) {
//					add_string_to_buf("  Hits {}.", adven[target].name);

					if (damage_pc(target,r2,damage_type::Weapon,-1, 13, TRUE) == TRUE) {
						}	
				}
				else {
					add_string_to_buf("  Misses {}.", adven[target].name);
					}
		
		}
		else { // missile on monst
			switch (level) {
				case 1: case 2: case 20:
					run_a_missile(source,targ_space,3,1,12,0,0,100);
					 monst_spell_note(m_target->number,12);
				break;
				case 3:
					run_a_missile(source,targ_space,5,1,14,0,0,100);
					 monst_spell_note(m_target->number,13);
				break;		
				case 7:
					run_a_missile(source,targ_space,7,1,14,0,0,100);
					monst_spell_note(m_target->number,15);				
					break;
				case 34:
					run_a_missile(source,targ_space,5,1,14,0,0,100);
					monst_spell_note(m_target->number,32);				
					break;
				default:
					run_a_missile(source,targ_space,12,1,14,0,0,100);
					 monst_spell_note(m_target->number,14);
				break;
				}
			r1 = rand_short(0,100) - 5 * min(10,bless) + 5 * m_target->gaffect(affect::CursedBlessed)
				- 5 * (can_see(source, m_target->m_loc,0));
			r2 = get_ran(dam[level],1,7) + min(10,bless);

			if (r1 <= skill_hit_chance(dam[level] * 2)) {
//					monst_spell_note(m_target->number,16);

					damage_monst(target - 100,7,r2,0, damage_type::Weapon, 13, TRUE);
				}
				else {
					monst_spell_note(m_target->number,18);
					}		
			}
}


Boolean monst_breathe(creature_data_type *caster,location targ_space,short dam_type)
//dam_type; // 0 - fire  1 - cold  2 - magic
{
	short level,missile_t[4] = {13,6,8,8};
	const damage_type type[4]{ damage_type::Fire, damage_type::Cold, damage_type::GeneralMagic, damage_type::Unblockable };
	location l;
	
	draw_terrain(2);
	if (is_combat() && (center_on_monst == TRUE)) {
		frame_space(caster->m_loc,0,caster->m_d.x_width,caster->m_d.y_width);
		}
	//if (dam_type < 2)
	l = caster->m_loc;
	if ((caster->m_d.direction < 4) &&
		(caster->m_d.x_width > 1))
			l.x++;
			
	dam_type = caster->m_d.breath_type;
	run_a_missile(l,targ_space,missile_t[dam_type],0,44,0,0,100);
	//	play_sound(44);
		//else play_sound(64);
	level = caster->m_d.breath;
	//if (level > 10)
	//	play_sound(5);

	monst_breathe_note(caster->number);
	level = get_ran(caster->m_d.breath,1,8);
	if (overall_mode < 10)
		level = level / 3;
	start_missile_anim();
	hit_space(targ_space,level, type[dam_type],1,1);
	do_explosion_anim(5,0);
	end_missile_anim();
	handle_marked_damage();

	return TRUE;
}

Boolean monst_cast_mage(creature_data_type *caster,short targ)
{
	short r1,j,spell,i,level,target_levels,friend_levels_near,x;
	Boolean acted = FALSE;
	location target,vict_loc,ashes_loc = {0,0},l;
	creature_data_type *affected;
	long dummy;
	short caster_array[7][18] = {{1,1,1,2,2, 2,1,3,4,4, 1,1,1,2,2, 2,3,4},
								{5,5,5,6,7, 8,9,10,11,11, 2,2,2,5,7, 10,10,5},
								{5,5,2,9,11, 12,12,12,14,13, 13,12,12,2,2, 2,2,2},
								{15,15,16,17,17, 5,12,12,13,13, 17,17,16,17,16, 2,2,2},
								{15,18,19,19,20, 20,21,21,16,17, 18,18,18,18,19, 19,19,20},
								{23,23,22,22,21, 21,20,24,19,18, 18,18,18,18,18, 23,23,19},
								{23,23,24,25,26, 27,19,22,19,18, 18,18,18,18,26, 24,24,23}};
	short emer_spells[7][4] = {{2,0,0,5},
								{2,10,11,7},
								{2,13,12,13},
								{2,13,12,13},
								{18,20,19,18},
								{18,24,19,24},
								{18,26,19,27}};

	
	if (is_antimagic(caster->m_loc.x,caster->m_loc.y)) {
		return FALSE;
		}
	// is target dead?
	if ((targ < 6) && (adven[targ].main_status != status::Normal))
		return FALSE;
	if ((targ >= 100) && (c_town.monst.dudes[targ - 100].active == 0))
		return FALSE;
		
	level = max(1,caster->m_d.mu - caster->gaffect(affect::Dumbfounded)) - 1;
	
	target = find_fireball_loc(caster->m_loc,1,(caster->attitude % 2 == 1) ? 0 : 1,&target_levels);
	friend_levels_near = (caster->attitude % 2 != 1) ? count_levels(caster->m_loc,3) : -1 * count_levels(caster->m_loc,3);

	if ((caster->m_d.health * 4 < caster->m_d.m_health) && (rand_short(0,10) < 9))
		spell = emer_spells[level][3];
		else if ((((caster->gaffect(affect::Speed) < 0) && (rand_short(0,10) < 7)) || 
			((caster->gaffect(affect::Speed) == 0) && (rand_short(0,10) < 5))) && (emer_spells[level][0] != 0))
			spell = emer_spells[level][0];
			else if ((friend_levels_near <= -10) && (rand_short(0,10) < 7) && (emer_spells[level][1] != 0))
				spell = emer_spells[level][1];
				else if ((target_levels > 50) && (rand_short(0,10) < 7) && (emer_spells[level][2] != 0))
					spell = emer_spells[level][2];
					else {
						r1 = rand_short(0,17);
						spell = caster_array[level][r1];
						}
	
	// Hastes happen often now, but don't cast them redundantly			
	if ((caster->gaffect(affect::Speed) > 0) && ((spell == 2) || (spell == 18)))
		spell = emer_spells[level][3];
		 

	// Anything prventing spell?
	if ((target.x > 64) && (monst_mage_area_effect[spell - 1] > 0)) {
		r1 = rand_short(0,9);
		spell = caster_array[level][r1];
		if ((target.x > 64) && (monst_mage_area_effect[spell - 1] > 0)) 
			return FALSE;
		}
	if (monst_mage_area_effect[spell - 1] > 0) {
		targ = 6;
		}
		
	if (targ < 6) {
		vict_loc = is_combat() ? pc_pos[targ] : c_town.p_loc;
		if (is_town()) 
			vict_loc = target = c_town.p_loc;
		}
	if (targ >= 100)
		vict_loc = c_town.monst.dudes[targ - 100].m_loc;
	if ((targ == 6) && is_antimagic(target.x,target.y))
		return FALSE;

	// check antimagic
	if (is_combat())
		if ((targ < 6) && is_antimagic(pc_pos[targ].x,pc_pos[targ].y))
			return FALSE;
	if (is_town())
		if ((targ < 6) && is_antimagic(c_town.p_loc.x,c_town.p_loc.y))
			return FALSE;
	if ((targ >= 100) && is_antimagic(c_town.monst.dudes[targ - 100].m_loc.x, c_town.monst.dudes[targ - 100].m_loc.y))
		return FALSE;

	// How about shockwave? Good idea?
	if ((spell == 27) && (caster->attitude % 2 != 1))
		spell = 26;
	if ((spell == 27) && (caster->attitude % 2 == 1) && (count_levels(caster->m_loc,10) < 45))
		spell = 26;

//	add_string_to_buf("m att {:d} trg {:d} trg2 x{:d}y{:d} spl {:d} mp {:d} tl:{:d} ",caster->attitude,targ,
//		(short)target.x,(short)target.y,spell,caster->m_d.mp,target_levels);
	
	l = caster->m_loc;
	if ((caster->m_d.direction < 4) && (caster->m_d.x_width > 1))
		l.x++;
	
	if (caster->m_d.mp >= monst_mage_cost[spell - 1]) {
		monst_cast_spell_note(caster->number,spell,0);
		acted = TRUE;
		caster->m_d.mp -= monst_mage_cost[spell - 1];
		
		draw_terrain(2);
		switch (spell) {
			case 1: // spark
				run_a_missile(l,vict_loc,6,1,11,0,0,80);
				r1 = get_ran(2,1,4);
				damage_target(targ,r1, damage_type::Fire);
				break;
			case 2: // minor haste
				play_sound(25);
				caster->gaffect(affect::Speed) += 2;
				break;
			case 3: // strength
				play_sound(25);
				caster->gaffect(affect::CursedBlessed) += 3;
				break;
			case 4: // flame cloud 
				run_a_missile(l,vict_loc,2,1,11,0,0,80);
				place_spell_pattern(single,vict_loc,5,FALSE,7);
				break;
			case 5: // flame
				run_a_missile(l,vict_loc,2,1,11,0,0,80);
				start_missile_anim();
				r1 = get_ran(caster->m_d.level,1,4);
				damage_target(targ,r1, damage_type::Fire);
				break;
			case 6: // minor poison
				run_a_missile(l,vict_loc,11,0,25,0,0,80);
				if (targ < 6)
					poison_pc(targ,2 + rand_short(0,1));
					else poison_monst(&c_town.monst.dudes[targ - 100],2 + rand_short(0,1));
				break;
			case 7: // slow
				run_a_missile(l,vict_loc,15,0,25,0,0,80);
				if (targ < 6)
					slow_pc(targ,2 + caster->m_d.level / 2);
					else slow_monst(&c_town.monst.dudes[targ - 100],2 + caster->m_d.level / 2);
				break;
			case 8: // dumbfound
				run_a_missile(l,vict_loc,14,0,25,0,0,80);
				if (targ < 6)
					dumbfound_pc(targ,2);
					else dumbfound_monst(&c_town.monst.dudes[targ - 100],2);
				break;
			case 9: // scloud
				run_a_missile(l,target,0,0,25,0,0,80);
				place_spell_pattern(square,target,7,FALSE,7);
				break;
			case 10: // summon beast
				r1 = get_summon_monster(1);
				if (r1 < 0)
					break;
				x = get_ran(3,1,4);
				//Delay(12,&dummy); // gives sound time to end
				play_sound(25);
				play_sound(-61);
				summon_monster(r1,caster->m_loc,
				 ((caster->attitude % 2 != 1) ? 0 : 100) + x,caster->attitude);
				break;
			case 11: // conflagration
				run_a_missile(l,target,13,1,25,0,0,80);
				place_spell_pattern(rad_pat2,target,5,FALSE,7);
				break;
			case 12: // fireball
				r1 = 1 + (caster->m_d.level * 3) / 4;
				if (r1 > 29) r1 = 29;
				run_a_missile(l,target,2,1,11,0,0,80);
				start_missile_anim();
				place_spell_pattern(square,target,50 + r1,TRUE,7);
				ashes_loc = target;
				break;
			case 13: case 20: case 26:// summon
				play_sound(25);
				if (spell == 13) {
					r1 = get_summon_monster(1);
					if (r1 < 0)
						break;
					j = get_ran(2,1,3) + 1;
					}
				if (spell == 20) {
					r1 = get_summon_monster(2);
					if (r1 < 0)
						break;
					j = get_ran(2,1,2) + 1;
					}
				if (spell == 26) {
					r1 = get_summon_monster(3);
					if (r1 < 0)
						break;
					j = rand_short(2,3);
					}
				Delay(12,&dummy); // gives sound time to end
				x = get_ran(4,1,4);
				for (i = 0; i < j; i++){
					play_sound(-61);
					if (summon_monster(r1,caster->m_loc,
					 ((caster->attitude % 2 != 1) ? 0 : 100) + x,caster->attitude) == FALSE) {
						add_string_to_buf("  Summon failed."); i = j;}
					}
				break;
			case 14: // web
				play_sound(25);
				place_spell_pattern(rad_pat2,target,1,FALSE,7);
				break;
			case 15: // poison
				run_a_missile(l,vict_loc,11,0,25,0,0,80);
				x = rand_short(0,3);
				if (targ < 6)
					poison_pc(targ,4 + x);
					else poison_monst(&c_town.monst.dudes[targ - 100],4 + x);
				break;		
			case 16: // ice bolt
				run_a_missile(l,vict_loc,6,1,11,0,0,80);
				r1 = get_ran(5 + (caster->m_d.level / 5),1,8);
				start_missile_anim();
				damage_target(targ,r1, damage_type::Cold);
				break;
			case 17: // slow gp
				play_sound(25);
				if (caster->attitude % 2 == 1)
					for (i = 0; i < 6; i++)
						if (pc_near(i,caster->m_loc,8))
							slow_pc(i,2 + caster->m_d.level / 4);
				for (i = 0; i < T_M; i++) {
					if ((c_town.monst.dudes[i].active != 0) && 
					(((c_town.monst.dudes[i].attitude % 2 == 1) && (caster->attitude % 2 != 1)) || 
					((c_town.monst.dudes[i].attitude % 2 != 1) && (caster->attitude % 2 == 1)) ||
					((c_town.monst.dudes[i].attitude % 2 == 1) && (caster->attitude != c_town.monst.dudes[i].attitude)))
						 && (dist(caster->m_loc,c_town.monst.dudes[i].m_loc) <= 7)) 
							slow_monst(&c_town.monst.dudes[i],2 + caster->m_d.level / 4);
					}
				break;
			case 18: // major haste
				play_sound(25);
				for (i = 0; i < T_M; i++)
					if ((monst_near(i,caster->m_loc,8,0)) && 
						(caster->attitude == c_town.monst.dudes[i].attitude)) {
						affected = &c_town.monst.dudes[i];	
						affected->gaffect(affect::Speed) += 3;
						}
				play_sound(4);
				break;		
			case 19: // firestorm
				run_a_missile(l,target,2,1,11,0,0,80);
				r1 = 1 + (caster->m_d.level * 3) / 4 + 3;
				if (r1 > 29) r1 = 29;
				start_missile_anim();
				place_spell_pattern(rad_pat2,target,50 + r1,TRUE,7);
				ashes_loc = target;
				break;




			case 21: // shockstorm
				run_a_missile(l,target,6,1,11,0,0,80);
				place_spell_pattern(rad_pat2,target,4,FALSE,7);
				break;
			case 22: // m. poison
				run_a_missile(l,vict_loc,11,1,11,0,0,80);
				x = rand_short(1,2);
				if (targ < 6)
					poison_pc(targ,6 + x);
					else poison_monst(&c_town.monst.dudes[targ - 100],6 + x);
				break;
			case 23: // kill!!!
				run_a_missile(l,vict_loc,9,1,11,0,0,80);
				r1 = 35 + get_ran(3,1,10);
				start_missile_anim();
				damage_target(targ,r1, damage_type::GeneralMagic);
				break;
			case 24: // daemon
				x = get_ran(3,1,4);
				play_sound(25);
				play_sound(-61);
				Delay(12,&dummy); // gives sound time to end
				summon_monster(85,caster->m_loc,
				 ((caster->attitude % 2 != 1) ? 0 : 100) + x,caster->attitude);
				break;
			case 25: // major bless
				play_sound(25);
				for (i = 0; i < T_M; i++)
					if ((monst_near(i,caster->m_loc,8,0)) && 
						(caster->attitude == c_town.monst.dudes[i].attitude)) {
						affected = &c_town.monst.dudes[i];	
						affected->m_d.health += get_ran(2,1,10);	
						r1 = get_ran(3,1,4);						
						affected->gaffect(affect::CursedBlessed) = min(8,affected->gaffect(affect::CursedBlessed) + r1);
						affected->gaffect(affect::Webbed) = 0;
						if (affected->gaffect(affect::Speed) < 0)
							affected->gaffect(affect::Speed) = 0;
						affected->m_d.morale += get_ran(3,1,10);	
						}
				play_sound(4);
				break;
			case 27: // shockwave
				do_shockwave(caster->m_loc);
				break;
			}
		}
		else caster->m_d.mp++;
		
	if (ashes_loc.x > 0)
		make_sfx(ashes_loc.x,ashes_loc.y,6);
	do_explosion_anim(5,0);
	end_missile_anim();
	handle_marked_damage();

	return acted;
}

Boolean monst_cast_priest(creature_data_type *caster,short targ)
{
	short r1,r2,spell,i,x,level,target_levels,friend_levels_near;
	Boolean acted = FALSE;
	location target,vict_loc,l;
	creature_data_type *affected;
	short caster_array[7][10] = {{1,1,1,1,3,3,3,4,4,4},
									{5,5,6,6,7,7,8,8,8,9},
									{9,6,6,8,11,12,12,5,5,12},
									{12,12,13,13,14,9,9,14,14,15},
									{19,18,13,19,15,18,18,19,16,18},
									{22,18,16,19,18,18,21,22,23,23},
									{26,26,25,24,26,22,24,22,26,25}};
	short emer_spells[7][4] = {{0,1,0,2},
								{0,8,0,2},
								{0,8,0,10},
								{0,14,0,10},
								{0,19,18,17},
								{0,19,18,20},
								{25,25,26,24}};
	location ashes_loc = {0,0};
	
	
	if ((targ < 6) && (adven[targ].main_status != status::Normal))
		return FALSE;
	if ((targ >= 100) && (c_town.monst.dudes[targ - 100].active == 0))
		return FALSE;
	if (is_antimagic(caster->m_loc.x,caster->m_loc.y)) {
		return FALSE;
		}
	level = max(1,caster->m_d.cl - caster->gaffect(affect::Dumbfounded)) - 1;

	target = find_fireball_loc(caster->m_loc,1,(caster->attitude % 2 == 1) ? 0 : 1,&target_levels);
	friend_levels_near = (caster->attitude % 2 != 1) ? count_levels(caster->m_loc,3) : -1 * count_levels(caster->m_loc,3);

	if ((caster->m_d.health * 4 < caster->m_d.m_health) && (rand_short(0,10) < 9))
		spell = emer_spells[level][3];
		else if ((caster->gaffect(affect::Speed) < 0) && (rand_short(0,10) < 7) && (emer_spells[level][0] != 0))
			spell = emer_spells[level][0];
			else if ((friend_levels_near <= -10) && (rand_short(0,10) < 7) && (emer_spells[level][1] != 0))
				spell = emer_spells[level][1];
				else if ((target_levels > 50 < 0) && (rand_short(0,10) < 7) && (emer_spells[level][2] != 0))
					spell = emer_spells[level][2];
					else {
						r1 = rand_short(0,9);
						spell = caster_array[level][r1];
						}



	// Anything preventing spell?
	if ((target.x > 64) && (monst_priest_area_effect[spell - 1] > 0))  {
		r1 = rand_short(0,9);
		spell = caster_array[level][r1];
		if ((target.x > 64) && (monst_priest_area_effect[spell - 1] > 0))  		
			return FALSE;
		}
	if (monst_priest_area_effect[spell - 1] > 0)
		targ = 6;
	if (targ < 6)
		vict_loc = is_town() ? c_town.p_loc : pc_pos[targ];
	if (targ >= 100)
		vict_loc = c_town.monst.dudes[targ - 100].m_loc;
	if ((targ == 6) && is_antimagic(target.x,target.y))
		return FALSE;
	if ((targ < 6) && is_antimagic(pc_pos[targ].x,pc_pos[targ].y))
		return FALSE;
	if ((targ >= 100) && is_antimagic(c_town.monst.dudes[targ - 100].m_loc.x, c_town.monst.dudes[targ - 100].m_loc.y))
		return FALSE;

	
//	add_string_to_buf("p att {:d} trg {:d} trg2 x{:d}y{:d} spl {:d} mp {:d}",caster->attitude,targ,
//		(short)target.x,(short)target.y,spell,caster->m_d.mp);

	// snuff heals if unwounded
	if ((caster->m_d.health == caster->m_d.m_health) &&
	 ((spell == 17) || (spell == 20)))
	 	spell--;

	l = caster->m_loc;
	if ((caster->m_d.direction < 4) && (caster->m_d.x_width > 1))
		l.x++;

	if (caster->m_d.mp >= monst_priest_cost[spell - 1]) {
		monst_cast_spell_note(caster->number,spell,1);
		acted = TRUE;
		caster->m_d.mp -= monst_priest_cost[spell - 1];
		draw_terrain(2);
		switch (spell) {
			case 3: // wrack
				run_a_missile(l,vict_loc,8,0,24,0,0,80);
				r1 = get_ran(2,1,4);
				start_missile_anim();
				damage_target(targ,r1, damage_type::Unblockable);
				break;
			case 4: // stumble
				play_sound(24);
				place_spell_pattern(single,vict_loc,1,FALSE,7);
				break;
			case 1: case 5: // Blesses
				play_sound(24);
				caster->gaffect(affect::CursedBlessed) = min(8,caster->gaffect(affect::CursedBlessed) + (spell == 1) ? 3 : 5);
				play_sound(4);
				break;
			case 6: // curse
				run_a_missile(l,vict_loc,8,0,24,0,0,80);
				x = rand_short(0,1);
				if (targ < 6)
					curse_pc(targ,2 + x);
					else curse_monst(&c_town.monst.dudes[targ - 100],2 + x);
				break;
			case 7: // wound 
				run_a_missile(l,vict_loc,8,0,24,0,0,80);
					r1 = get_ran(2,1,6) + 2;
				start_missile_anim();
					damage_target(targ,r1, damage_type::GeneralMagic);
				break;		
			case 8: case 22: // summon spirit,summon guardian
				play_sound(24);
				play_sound(-61);
					
				x =  get_ran(3,1,4);
				summon_monster(((spell == 8) ? 125 : 122),caster->m_loc,
				 ((caster->attitude % 2 != 1) ? 0 : 100) + x,caster->attitude);
				break;
			case 9: // disease
				run_a_missile(l,vict_loc,11,0,24,0,0,80);
				x = rand_short(0,2);
				if (targ < 6)
					disease_pc(targ,2 + x);
					else disease_monst(&c_town.monst.dudes[targ - 100],2 + x);			
				break;
			case 11: // holy scourge
				run_a_missile(l,vict_loc,15,0,24,0,0,80);
				if (targ < 6) {
					r1 = rand_short(0,2);
					slow_pc(targ,2 + r1);
					r1 = rand_short(0,2);
					curse_pc(targ,3 + r1);
					}
					else {
						r1 = rand_short(0,2);
						slow_monst(&c_town.monst.dudes[targ - 100],r1);						 
						r1 = rand_short(0,2);
						curse_monst(&c_town.monst.dudes[targ - 100],r1);
						}
				break;
			case 12: // smite 
				run_a_missile(l,vict_loc,6,0,24,0,0,80);
				r1 = get_ran(4,1,6) + 2;
				start_missile_anim();
				damage_target(targ,r1, damage_type::Cold);
				break;		
			case 14: // sticks to snakes
				play_sound(24);
				r1 = rand_short(1,4) + 2;
				for (i = 0; i < r1; i++) {
					play_sound(-61);
					r2 = rand_short(0,7);
					x = get_ran(3,1,4);
					summon_monster((r2 == 1) ? 100 : 99,caster->m_loc,
					 ((caster->attitude % 2 != 1) ? 0 : 100) + x,caster->attitude);
					}	
				break;
			case 15: // martyr's shield
				play_sound(24);
				caster->gaffect(affect::MartyrsShield) = min(10,caster->gaffect(affect::MartyrsShield) + 5);
				break;
			case 19: // summon host
				play_sound(24);
				x = get_ran(3,1,4) + 1; 
				play_sound(-61);
				summon_monster(126,caster->m_loc,
					((caster->attitude % 2 != 1) ? 0 : 100) + x,caster->attitude); 
				for (i = 0; i < 4; i++)	{
					play_sound(-61);
					if (summon_monster(125,caster->m_loc,
						 ((caster->attitude % 2 != 1) ? 0 : 100) + x,caster->attitude) == FALSE)
						 	i = 4;
					}
				break;

			case 13: case 23: // holy scourge,curse all,pestilence		
				play_sound(24);
				r1 = get_ran(2,0,2);
				r2 = rand_short(0,2);
				if (caster->attitude % 2 == 1)
					for (i = 0; i < 6; i++)
						if (pc_near(i,caster->m_loc,8)) {
							if (spell == 13)
								curse_pc(i,2 + r1);
							if (spell == 23)
								disease_pc(i,2 + r2);
							}
				for (i = 0; i < T_M; i++) {
					if ((c_town.monst.dudes[i].active != 0) && 
					(((c_town.monst.dudes[i].attitude % 2 == 1) && (caster->attitude % 2 != 1)) || 
					((c_town.monst.dudes[i].attitude % 2 != 1) && (caster->attitude % 2 == 1)) ||
					((c_town.monst.dudes[i].attitude % 2 == 1) && (caster->attitude != c_town.monst.dudes[i].attitude)))
						 && (dist(caster->m_loc,c_town.monst.dudes[i].m_loc) <= 7)) {
							if (spell == 13)
								curse_monst(&c_town.monst.dudes[i],2 + r1);
							if (spell == 23)
								disease_monst(&c_town.monst.dudes[i],2 + r2);
							}
					}
				break;
		
			case 2: case 10: case 17: case 20: // heals
				play_sound(24);
				switch(spell) {
					case 2: r1 = get_ran(2,1,4) + 2; break;
					case 10: r1 = get_ran(3,1,6); break;
					case 17: r1 = get_ran(5,1,6) + 3; break;
					case 20: r1 = 50; break;
					}
				caster->m_d.health = min(caster->m_d.health + r1, caster->m_d.m_health);
				break;		
			case 16: case 24:// bless all,revive all
				play_sound(24);
				r1 =  get_ran(2,1,4); r2 = get_ran(3,1,6);
				for (i = 0; i < T_M; i++)
					if ((monst_near(i,caster->m_loc,8,0)) && 
						(caster->attitude == c_town.monst.dudes[i].attitude)) {
						affected = &c_town.monst.dudes[i];	
						if (spell == 16)
							affected->gaffect(affect::CursedBlessed) = min(8,affected->gaffect(affect::CursedBlessed) + r1);
						if (spell == 24)
							affected->m_d.health += r1;
						}
				play_sound(4);
				break;
			case 18: // Flamestrike
				run_a_missile(l,target,2,0,11,0,0,80);
				r1 = 2 + caster->m_d.level / 2 + 2;
				start_missile_anim();
				place_spell_pattern(square,target,50 + r1,TRUE,7);
				ashes_loc = target;
				break;

		
			case 21: // holy ravaging
				run_a_missile(l,vict_loc,14,0,53,0,0,80);
				r1 = get_ran(4,1,8);
				r2 = rand_short(0,2);
				damage_target(targ,r1, damage_type::GeneralMagic);
				if (targ < 6) {
					slow_pc(targ,6);
					poison_pc(targ,5 + r2);
					}
					else {
						slow_monst(&c_town.monst.dudes[targ - 100],6);						 
						poison_monst(&c_town.monst.dudes[targ - 100],5 + r2);
						}
				break;
			case 25: // avatar
				play_sound(24);
				monst_spell_note(caster->number,26);
				caster->m_d.health = caster->m_d.m_health;
				caster->gaffect(affect::CursedBlessed) = 8;
				caster->gaffect(affect::Poisoned) = 0;
				caster->gaffect(affect::Speed) = 8;
				caster->gaffect(affect::Webbed) = 0;
				caster->gaffect(affect::Diseased) = 0;
				caster->gaffect(affect::Dumbfounded) = 0;
				caster->gaffect(affect::MartyrsShield) = 8;
				break;
			case 26: // divine thud
				run_a_missile(l,target,9,0,11,0,0,80);
				r1 = (caster->m_d.level * 3) / 4 + 5;
				if (r1 > 29) r1 = 29;
				start_missile_anim();
				place_spell_pattern(rad_pat2,target,130 + r1,TRUE,7 );
				ashes_loc = target;
				break;
			}
	
	
		}
		else caster->m_d.mp++;
	if (ashes_loc.x > 0)
		make_sfx(ashes_loc.x,ashes_loc.y,6);
	do_explosion_anim(5,0);
	end_missile_anim();
	handle_marked_damage();

	return acted;
}

void damage_target(short target, short dam, damage_type type)
{
	return damage_target(target, dam, to_underlying(type));
}

void damage_target(short target,short dam,short type)
{

	if (target == 6)
		return;
	if (target < 6)
		damage_pc(target,dam,type,-1);
	else
		damage_monst(target - 100, 7, dam, 0, type);
}


//	target = find_fireball_loc(caster->m_loc,1,(caster->attitude == 1) ? 0 : 1,&target_levels);

location find_fireball_loc(location where,short radius,short mode,short *m)
//short mode; // 0 - hostile casting  1 - friendly casting
{
	location check_loc,cast_loc = {120,0};
	short cur_lev,level_max = 10;
	
	for (check_loc.x = 1; check_loc.x < town_size[town_type] - 1; check_loc.x ++)
		for (check_loc.y = 1; check_loc.y < town_size[town_type] - 1; check_loc.y ++)
			if ((dist(where,check_loc) <= 8) && (can_see(where,check_loc,2) < 5) && (get_obscurity(check_loc.x,check_loc.y) < 5)) {
					{
						cur_lev = count_levels(check_loc,radius);
						if (mode == 1)
							cur_lev = cur_lev * -1;
						if ( ((cur_lev > level_max) || ((cur_lev == level_max) && (rand_short(0,1) == 0)))
						 && (dist(where,check_loc) > radius)) {
							level_max = cur_lev;
							cast_loc = check_loc;
							}
					}
			}
	*m = level_max;
	
	return cast_loc;
}

location closest_pc_loc(location where)
{
	short i;
	location pc_where = {120,120};
	
	for (i = 0; i < 6; i++)
		if (adven[i].main_status == status::Normal)
			if ((dist(where,pc_pos[i])) < (dist(where,pc_where)))
				pc_where = pc_pos[i];
	return pc_where;
}

short count_levels(location where,short radius)
{
	short i,store = 0;

	for (i = 0; i < T_M; i++)
		if (monst_near(i,where,radius,0) == TRUE) {
			if (c_town.monst.dudes[i].attitude % 2 == 1)
				store = store - c_town.monst.dudes[i].m_d.level;
				else store = store + c_town.monst.dudes[i].m_d.level;
			}
	if (is_combat()) {
		for (i = 0; i < 6; i++)
			if (pc_near(i,where,radius) == TRUE)
				store = store + 10;
		}
	if (is_town())
		if ((vdist(where,c_town.p_loc) <= radius) && (can_see(where,c_town.p_loc,2) < 5))
			store += 20;
			
	return store;
}

Boolean pc_near(short pc_num,location where,short radius)
{
	// Assuming not looking
	if (overall_mode >= 10) {
		if ((adven[pc_num].main_status == status::Normal) && (vdist(pc_pos[pc_num],where) <= radius))
			return TRUE;
			else return FALSE;
		}
	if ((adven[pc_num].main_status == status::Normal) && (vdist(c_town.p_loc,where) <= radius))
		return TRUE;
		else return FALSE;
}

/*short pc_there(where)
location where;
{
	short i;
	
	for (i = 0; i < 6; i++)
		if ((adven[i].main_status == status::Normal) && (pc_pos[i].x == where.x) && (pc_pos[i].y == where.y))
			return i;
	return 6;
} */

Boolean monst_near(short m_num,location where,short radius,short active)
//short active; // 0 - any monst 1 - monster need be active
{
	if ((c_town.monst.dudes[m_num].active != 0) && (vdist(c_town.monst.dudes[m_num].m_loc,where) <= radius)
		 && ((active == 0) || (c_town.monst.dudes[m_num].active == 2)) )
		return TRUE;
		else return FALSE;
}

void fireball_space(location loc,short dam)
{
place_spell_pattern(square,loc,50 + dam,FALSE,7);
}

void place_spell_pattern(effect_pat_type pat,location center,short type,Boolean prep_for_anim,short who_hit)
//type;  // 0 - take codes in pattern, OW make all nonzero this type
// Types  0 - Null  1 - web  2 - fire barrier  3 - force barrier  4 - force wall  5 - fire wall
//   6 - anti-magic field  7 - stink cloud  8 - ice wall  9 - blade wall  10 - quickfire
//   11 - dispel  12 - sleep field
//  50 + i - 80 :  id6 fire damage  90 + i - 120 : id6 cold damage 	130 + i - 160 : id6 magic dam.
// if prep for anim is TRUE, supporess look checks and go fast
{
	short i,j,r1,k,store = 0;
	unsigned char effect;
	location spot_hit;
	location s_loc;
	BoeRect active;
	creature_data_type *which_m;
	Boolean monster_hit = FALSE;
	
	location pool_locs[5] = {{13,2},{11,19},{34,22},{46,15},{46,7}};
	
	if (type > 0)
		modify_pattern(&pat,type);

	// first clip out special results.
	if ((is_town() || (is_combat() && (which_combat_type == 1))) && (c_town.town_num == 46) && (center.x == 24) && (center.y == 41)
		&& (PSD[46][9] == 0) && (type >= 50) && (type < 90)) {
		end_missile_anim();
		run_a_missile(pc_pos[current_pc],center,2,1,11,0,0,200);
		mondo_boom(center,0);
		FCD(3464,0);
		PSD[46][9] = 1;
		for (i = 0; i < 4; i++)
			elim_monst(138 + i,46,9);
		alter_space(24,41,0);
		return;
		}
	for (i = 0; i < 5; i++)
		store += PSD[20][i];
	if ((is_town() || (is_combat() && (which_combat_type == 1))) && (c_town.town_num == 23))
		for (i = 0; i < 5; i++)
			if ((center.x == pool_locs[i].x) && (center.y == pool_locs[i].y)
				&& (PSD[20][i] == 0) && (type >= 50) && (type < 90)) {
			end_missile_anim();
			run_a_missile(pc_pos[current_pc],center,2,1,11,0,0,200);
			mondo_boom(center,0);
			if (store == 4) {
				FCD(3239,0);
				PSD[23][3] = 20;
				erase_specials();
				}
				else FCD(3238,0);
			PSD[20][i] = 1;
			alter_space(pool_locs[i].x,pool_locs[i].y,0);
			return;
			}


	active = c_town.town.in_town_rect;
	// eliminate barriers that can't be seen
		for (i = boe_clamp(center.x - 4,active.left + 1,active.right - 1); 
		 i <= boe_clamp(center.x + 4,active.left + 1,active.right - 1); i++)
			for (j = boe_clamp(center.y - 4,active.top + 1,active.bottom - 1);
			 j <= boe_clamp(center.y + 4,active.top + 1,active.bottom - 1); j++) {
				s_loc.x = i; s_loc.y = j;
				if (can_see(center,s_loc,0) == 5)
					 pat.pattern[i - center.x + 4][j - center.y + 4] = 0;
			}


	// First actually make barriers, then draw them, then inflict damaging effects.
	for (i = boe_clamp(center.x - 4,0,town_size[town_type] - 1); i <= boe_clamp(center.x + 4,0,town_size[town_type] - 1); i++)
		for (j = boe_clamp(center.y - 4,0,town_size[town_type] - 1); j <= boe_clamp(center.y + 4,0,town_size[town_type] - 1); j++)
			if (get_obscurity(i,j) < 5) {
				effect = pat.pattern[i - center.x + 4][j - center.y + 4];
				switch (effect) {
					case 1: web_space(i,j); break;
					case 2: make_fire_barrier(i,j); break;
					case 3: make_force_barrier(i,j); break;
					case 4: 
						make_force_wall(i,j); break;
					case 5: make_fire_wall(i,j); break;
					case 6: make_antimagic(i,j); break;
					case 7: scloud_space(i,j); break;
					case 8: make_ice_wall(i,j); break;
					case 9: make_blade_wall(i,j); break;	
					case 10:				
						make_quickfire(i,j);
						break;
					case 11:
						dispel_fields(i,j,0);
						break;
					case 12: sleep_cloud_space(i,j); break;	
					}
					}
	draw_terrain(0);	
	if (is_town()) // now make things move faster if in town
		fast_bang = 2;
		
	// Damage to pcs
	for (k = 0; k < 6; k++) 
		for (i = boe_clamp(center.x - 4,0,town_size[town_type] - 1); i <= boe_clamp(center.x + 4,0,town_size[town_type] - 1); i++)
			for (j = boe_clamp(center.y - 4,0,town_size[town_type] - 1); j <= boe_clamp(center.y + 4,0,town_size[town_type] - 1); j++) {
				spot_hit.x = i;
				spot_hit.y = j;
				if ((get_obscurity(i,j) < 5) && (adven[k].main_status == status::Normal)
					&& ((is_combat() &&(same_point(pc_pos[k],spot_hit) == TRUE)) ||
					(is_town() && (same_point(c_town.p_loc,spot_hit) == TRUE)))) {
					effect = pat.pattern[i - center.x + 4][j - center.y + 4];
					switch (effect) {
						case 4: 
							r1 = get_ran(2,1,6);
							damage_pc(k,r1, damage_type::GeneralMagic,-1);
							break;
						case 5: 
							r1 = rand_short(1,6) + 1;
							damage_pc(k,r1, damage_type::Fire,-1);
							break;
						case 8: 
							r1 = get_ran(2,1,6);
							damage_pc(k,r1, damage_type::Cold,-1);
							break;
						case 9: 
							r1 = get_ran(4,1,8);
							damage_pc(k,r1, damage_type::Weapon,-1);
							break;	
						default:	
							if ((effect >= 50) && (effect < 80)) {
								r1 = get_ran(effect - 50,1,6);
								damage_pc(k,r1, damage_type::Fire,-1);
								}
							if ((effect >= 90) && (effect < 120)) {
								r1 = get_ran(effect - 90,1,6);
								damage_pc(k,r1, damage_type::Cold,-1);
								}
							if ((effect >= 130) && (effect < 160)) {
								r1 = get_ran(effect - 130,1,6);
								damage_pc(k,r1, damage_type::GeneralMagic,-1);
								}
						break;
  						}
					}
		}
		
	fast_bang = 0;
	
	// Damage to monsters
	for (k = 0; k < T_M; k++) 
		if ((c_town.monst.dudes[k].active > 0) && (dist(center,c_town.monst.dudes[k].m_loc) <= 5)) {
		monster_hit = FALSE;
		// First actually make barriers, then draw them, then inflict damaging effects.
		for (i = boe_clamp(center.x - 4,0,town_size[town_type] - 1); i <= boe_clamp(center.x + 4,0,town_size[town_type] - 1); i++)
			for (j = boe_clamp(center.y - 4,0,town_size[town_type] - 1); j <= boe_clamp(center.y + 4,0,town_size[town_type] - 1); j++) {
				spot_hit.x = i;
				spot_hit.y = j;

				if ((monster_hit == FALSE) && (get_obscurity(i,j) < 5) && (monst_on_space(spot_hit,k) > 0)) {

					if (pat.pattern[i - center.x + 4][j - center.y + 4] > 0)
						monster_hit = TRUE;
					effect = pat.pattern[i - center.x + 4][j - center.y + 4];
					switch (effect) {
						case 1:
							which_m = &c_town.monst.dudes[k];
							web_monst(which_m,3);
							break;
						case 4: 
							r1 = get_ran(3,1,6);
							damage_monst(k, who_hit, r1,0, damage_type::GeneralMagic);
							break;
						case 5: 
							r1 = get_ran(2,1,6);
							which_m = &c_town.monst.dudes[k];
							if (which_m->m_d.spec_skill == 22)
								break;
							damage_monst(k, who_hit, r1,0, damage_type::Fire);
							break;
						case 7:
							which_m = &c_town.monst.dudes[k];
							curse_monst(which_m,rand_short(1,2));
							break;
						case 8: 
							which_m = &c_town.monst.dudes[k];
							r1 = get_ran(3,1,6);
							if (which_m->m_d.spec_skill == 23)
								break;
							damage_monst(k, who_hit, r1,0, damage_type::Cold);
							break;
						case 9: 
							r1 = get_ran(6,1,8);
							damage_monst(k, who_hit, r1,0, damage_type::Weapon);
							break;	
						case 12: 
							which_m = &c_town.monst.dudes[k];
							charm_monst(which_m,0,11,3);
							break;	
						default:	
							if ((effect >= 50) && (effect < 80)) {
								r1 = get_ran(effect - 50,1,6);
								damage_monst(k,who_hit,  r1,0, damage_type::Fire);
								}
							if ((effect >= 90) && (effect < 120)) {
								r1 = get_ran(effect - 90,1,6);
								damage_monst(k,who_hit,  r1,0, damage_type::Cold);
								}
							if ((effect >= 130) && (effect < 160)) {
								r1 = get_ran(effect - 130,1,6);
								damage_monst(k,who_hit,  r1,0, damage_type::GeneralMagic);
								}
						}
					}
			}
		}
}


void modify_pattern(effect_pat_type *pat,short type)
{
	short i,j;
	
	for (i = 0; i < 9; i++)
		for (j = 0; j < 9; j++)
			if (pat->pattern[i][j] > 0)
				pat->pattern[i][j] = type;
}

void do_shockwave(location target)
{
	short i;

	start_missile_anim();
	for (i = 0; i < 6; i++)
		if ((dist(target,pc_pos[i]) > 0) && (dist(target,pc_pos[i]) < 11)
			&& (adven[i].main_status == status::Normal))
				damage_pc(i, get_ran(2 + dist(target,pc_pos[i]) / 2, 1, 6), damage_type::Unblockable,-1);
  	for (i = 0; i < T_M; i++)
		if ((c_town.monst.dudes[i].active != 0) && (dist(target,c_town.monst.dudes[i].m_loc) > 0)
			 && (dist(target,c_town.monst.dudes[i].m_loc) < 11)
			 && (can_see(target,c_town.monst.dudes[i].m_loc,0) < 5))
				damage_monst(i, current_pc, get_ran(2 + dist(target,c_town.monst.dudes[i].m_loc) / 2 , 1, 6), 0, damage_type::Unblockable);
	do_explosion_anim(5,0);
	end_missile_anim();
	handle_marked_damage();
}

void radius_damage(location target,short radius, short dam, short type)////
{
	short i;
	
	if (is_town()) {
		for (i = 0; i < 6; i++)
			if ((dist(target,c_town.p_loc) > 0) && (dist(target,c_town.p_loc) <= radius)
				&& (adven[i].main_status == status::Normal))
					damage_pc(i, dam, type,-1);
		for (i = 0; i < T_M; i++)
			if ((c_town.monst.dudes[i].active != 0) && (dist(target,c_town.monst.dudes[i].m_loc) > 0)
				 && (dist(target,c_town.monst.dudes[i].m_loc) <= radius)
				 && (can_see(target,c_town.monst.dudes[i].m_loc,0) < 5))
					damage_monst(i, current_pc, dam, 0, type);
	   return;
		}
		
	start_missile_anim();
	for (i = 0; i < 6; i++)
		if ((dist(target,pc_pos[i]) > 0) && (dist(target,pc_pos[i]) <= radius)
			&& (adven[i].main_status == status::Normal))
				damage_pc(i, dam, type,-1);
	for (i = 0; i < T_M; i++)
		if ((c_town.monst.dudes[i].active != 0) && (dist(target,c_town.monst.dudes[i].m_loc) > 0)
			 && (dist(target,c_town.monst.dudes[i].m_loc) <= radius)
			 && (can_see(target,c_town.monst.dudes[i].m_loc,0) < 5))
				damage_monst(i, current_pc, dam, 0, type);
	do_explosion_anim(5,0);
	end_missile_anim();
	handle_marked_damage();
}

// Slightly kludgy way to only damage PCs in space)
void hit_pcs_in_space(location target,short dam, damage_type type,short report,short hit_all)
{
	//short store_active[T_M],i;
	
	//for (i = 0; i < T_M; i++) {
	//	store_active[i] = c_town.monst.dudes[i].active;
	//	c_town.monst.dudes[i].active = 0;
	//	}
	hit_space(target, dam,type, report, 10 + hit_all);
	//for (i = 0; i < T_M; i++) 
	//	c_town.monst.dudes[i].active = store_active[i];
}

void hit_space(location target, short dam, damage_type type, short report, short hit_all)
{
	return hit_space(target, dam, to_underlying(type), report, hit_all);
}

void hit_space(location target,short dam,short type,short report,short hit_all)
//type; // 0 - weapon   1 - fire   2 - poison   3 - general magic   4 - unblockable  5 - cold
			//	6 - demon	7 - undead
//short report; // 0 - no  1 - print result
//hit_all; // 0 - nail top thing   1 - hit all in space  + 10 ... no monsters
{
	short i;
	Boolean stop_hitting = FALSE,hit_monsters = TRUE;

//	add_string_to_buf("  {:d} {:d}.                                  ",target.x,target.y);
	if ((target.x < 0) || (target.x > 63) || (target.y < 0) || (target.y > 63))
		return;
	
	if (hit_all >= 10) {
		hit_monsters = FALSE;
		hit_all -= 10;
		}
	
	if (is_antimagic(target.x,target.y) && ((type == 1) || (type == 3) || (type == 5))) {
		return;
		}
	
	if (dam <= 0) {
		add_string_to_buf("  No damage.");
		return;
		}
	
	for (i = 0; i < T_M; i++)
		if ((hit_monsters == TRUE) && (c_town.monst.dudes[i].active != 0) && (stop_hitting == FALSE))
			if (monst_on_space(target,i)) {
				if (processing_fields == TRUE)
					damage_monst(i, 6, dam, 0, type);
					else damage_monst(i, (monsters_going == TRUE) ? 7 : current_pc, dam, 0, type);
				stop_hitting = (hit_all == 1) ? FALSE : TRUE;	
				}
	
	if (overall_mode >= 10)
		for (i = 0; i < 6; i++)
			if ((adven[i].main_status == status::Normal) && (stop_hitting == FALSE))
				if (same_point(pc_pos[i],target) == TRUE) {
						damage_pc(i,dam,type,-1);					
						stop_hitting = (hit_all == 1) ? FALSE : TRUE;				
					}
	if (overall_mode < 10)
		if (same_point(target,c_town.p_loc) == TRUE) {
			fast_bang = 1;
			hit_party(dam,type);
			fast_bang = 0;
			stop_hitting = (hit_all == 1) ? FALSE : TRUE;				
			}

	if ((report == 1) && (hit_all == 0) && (stop_hitting == FALSE))
		add_string_to_buf("  Missed.");

}



void do_poison()
{
	short i,r1 = 0;
	Boolean some_poison = FALSE;
		
	for (i = 0; i < 6; i++)
		if (adven[i].main_status == status::Normal)
			if (adven[i].gaffect(affect::Poisoned) > 0)
				some_poison = TRUE;
	if (some_poison == TRUE) {
		add_string_to_buf("Poison:                        ");
		for (i = 0; i < 6; i++)
			if (adven[i].main_status == status::Normal)
				if (adven[i].gaffect(affect::Poisoned) > 0) {
					r1 = get_ran(adven[i].gaffect(affect::Poisoned),1,6);
					damage_pc(i,r1, damage_type::Poison,-1);
					if (rand_short(0,8) < 6)
						adven[i].reduce_affect(affect::Poisoned);
					if (rand_short(0,8) < 6)
						if (adven[i].has_trait(trait::GoodConstitution))
							adven[i].reduce_affect(affect::Poisoned);
				}
		put_pc_screen();
		//if (overall_mode < 10)
		//	boom_space(party.p_loc,overall_mode,2,r1);
		}
}


void handle_disease()
{
	short i,r1 = 0;
	Boolean disease = FALSE;
		
	for (i = 0; i < 6; i++)
		if (adven[i].main_status == status::Normal)
			if (adven[i].gaffect(affect::Diseased) > 0)
				disease = TRUE;
				
	if (disease == TRUE) {
		add_string_to_buf("Disease:                        ");
		for (i = 0; i < 6; i++)
			if (adven[i].main_status == status::Normal)
				if (adven[i].gaffect(affect::Diseased) > 0) {
					r1 = rand_short(1,10);
					switch (r1) {
						case 1: case 2:
							poison_pc(i,2);
							break;
						case 3: case 4:
							slow_pc(i,2);
							break;
						case 5:
							drain_pc(i,5);
							break;
						case 6: case 7:
							curse_pc(i,3);
							break;
						case 8:
							dumbfound_pc(i,3);
							break;
						case 9: case 10:
							add_string_to_buf("  {} unaffected. ", adven[i].name);
							break;
						}
					r1 = rand_short(0,7);
					if (adven[i].has_trait(trait::GoodConstitution))
						r1 -= 2;
					if ((rand_short(0,7) <= 0) || (pc_has_abil_equip(adven[i],67) < 24))
						adven[i].reduce_affect(affect::Diseased);
				}
		put_pc_screen();
		}	
}

void handle_acid()
{
	short i,r1 = 0;
	Boolean some_acid = FALSE;
		
	for (i = 0; i < 6; i++)
		if (adven[i].main_status == status::Normal)
			if (adven[i].gaffect(affect::Acid) > 0)
				some_acid = TRUE;
				
	if (some_acid == TRUE) {
		add_string_to_buf("Acid:                        ");
		for (i = 0; i < 6; i++)
			if (adven[i].main_status == status::Normal)
				if (adven[i].gaffect(affect::Acid) > 0) {
					r1 = get_ran(adven[i].gaffect(affect::Acid),1,6);
					damage_pc(i,r1, damage_type::GeneralMagic,-1);
					adven[i].reduce_affect(affect::Acid);
				}
		if (overall_mode < 10)
			boom_space(party.p_loc,overall_mode,3,r1,8);
		}
}

Boolean no_pcs_left()
{
	short i = 0;
	
	while (i < 6) {
		if (adven[i].main_status == status::Normal)
			return FALSE;
		i++;
		}
	return TRUE;
	
}

Boolean hit_end_c_button()
{
	Boolean end_ok = TRUE;
	
	if (which_combat_type == 0) {
		end_ok = out_monst_all_dead();
		}
//	if (adven[0].extra[7] > 0)
//		end_ok = TRUE;

	if (end_ok == TRUE)
		end_combat();
	return end_ok;
}

Boolean out_monst_all_dead()
{
	short i;

	for (i = 0; i < T_M; i++)
		if ((c_town.monst.dudes[i].active > 0) && (c_town.monst.dudes[i].attitude % 2 == 1))
			return FALSE;
	return TRUE;
}

void end_combat()
{
	short i;
	
	for (i = 0; i < 6; i++) {
		if (adven[i].main_status == status::Fled)
			adven[i].main_status = status::Normal;
		adven[i].gaffect(affect::PoisonedWeapon) = 0;
		adven[i].gaffect(affect::CursedBlessed) = 0;			
		adven[i].gaffect(affect::Speed) = 0;		
		}
	if (which_combat_type == 0) {
		overall_mode = 0;
		load_area_graphics();
		}
	combat_active_pc = 6;
	current_pc = store_current_pc;
	if (adven[current_pc].main_status != status::Normal)
		current_pc = first_active_pc();
	put_item_screen(stat_window,0);
	draw_buttons(0);
}


Boolean combat_cast_mage_spell()
{
	short spell_num,target,i,store_sp,bonus = 1,r1,store_sound = 0,store_m_type = 0,num_opp = 0;
	creature_data_type *which_m;
	monster_record_type get_monst;
	
	if (is_antimagic(pc_pos[current_pc].x,pc_pos[current_pc].y)) {
		add_string_to_buf("  Not in antimagic field.");
		return FALSE;
		}
	store_sp = adven[current_pc].cur_sp;
	if (adven[current_pc].cur_sp == 0)
		add_string_to_buf("Cast: No spell points.        ");		
	else if (adven[current_pc].skills[skill::MageSpells] == 0)
		add_string_to_buf("Cast: No mage skill.        ");		
	else if (get_encumberance(adven[current_pc]) > 1) { 
		add_string_to_buf("Cast: Too encumbered.        ");
		take_ap(6);
		give_help(40,0,0);
		return TRUE;
		}	
		else {	
		
		
	if (spell_forced == FALSE)		
		spell_num = pick_spell(current_pc,0,2);
		else {
			if (repeat_cast_ok(0) == FALSE)
				return FALSE;
			spell_num = pc_last_cast[0][current_pc];			
			}
	
	if (spell_num == 35) {
		store_sum_monst = pick_trapped_monst();
		if (store_sum_monst == 0)
			return FALSE;
		get_monst = return_monster_template(store_sum_monst);
		if (store_sp < get_monst.level) {
			add_string_to_buf("Cast: Not enough spell points.        ");		
			return FALSE;
			}
		store_sum_monst_cost = get_monst.level;
		}

	bonus = pc_stat_adj(adven[current_pc], skill::Intelligence);
	combat_posing_monster = current_working_monster = current_pc;
	if (spell_num >= 70)
		return FALSE;
	if (spell_num < 70) {
			print_spell_cast(spell_num,0);
			if (refer_mage[spell_num] == 0) {
				take_ap(6);
				draw_terrain(2);
				do_mage_spell(current_pc,spell_num);
				combat_posing_monster = current_working_monster = -1;
				}
				else if (refer_mage[spell_num] == 2) {
					start_spell_targeting(spell_num);
					}
				else if (refer_mage[spell_num] == 3) {
					start_fancy_spell_targeting(spell_num);
					}
					else {
						start_missile_anim();
						take_ap(6);
						draw_terrain(2);
						switch (spell_num) {
							case 54:
								adven[current_pc].cur_sp -= s_cost[0][spell_num];		
								add_string_to_buf("  The ground shakes.          ");
								do_shockwave(pc_pos[current_pc]);
								break;
								
							case 2: case 21: case 3: case 14: case 29:
//								target = select_pc(11,0);
								target = store_spell_target;
								if (target < 6) {
									adven[current_pc].cur_sp -= s_cost[0][spell_num];
									play_sound(4);
									switch (spell_num) {
										case 14:
											add_string_to_buf("  {} receives venom.               ",
												 adven[target].name);
											poison_weapon(target,3 + bonus,1);
											store_m_type = 11;
											break;
										
										case  3:
											add_string_to_buf("  {} stronger.                     ",
												 adven[target].name);
											adven[target].gaffect(affect::CursedBlessed) = adven[target].gaffect(affect::CursedBlessed) + 3;		
											store_m_type = 8;
											break;
										case 29:			
											add_string_to_buf("  {} resistant.                     ",
												 adven[target].name);
											adven[target].gaffect(affect::MagicResistant) = adven[target].gaffect(affect::MagicResistant) + 5 + bonus;		
											store_m_type = 15;
											break;
										
										default:
											i = (spell_num == 2) ? 2 : max(2,adven[current_pc].level / 2 + bonus);
											adven[target].gaffect(affect::Speed) = min(8,
												adven[target].gaffect(affect::Speed) + i);
											add_string_to_buf("  {} hasted.                       ",
												 adven[target].name);
											store_m_type = 8;
											break;
										}
									add_missile(pc_pos[target],store_m_type,0,0,0);
							}
								break;
								
							case 39: case 55:
								store_sound = 25;
								adven[current_pc].cur_sp -= s_cost[0][spell_num];
								
										
								for (i = 0; i < 6; i++) 
									if (adven[i].main_status == status::Normal) {
									adven[i].gaffect(affect::Speed) = min(8,
										adven[i].gaffect(affect::Speed) + ((spell_num == 39) ? 1 + adven[current_pc].level / 8 + bonus : 3 + bonus));		
									if (spell_num == 55) {
										poison_weapon(i,2,1);
										adven[i].gaffect(affect::CursedBlessed) += 4;										
										add_missile(pc_pos[i],14,0,0,0);
										}
										else add_missile(pc_pos[i],8,0,0,0);
									}
								//play_sound(4);
								if (spell_num == 39)
									add_string_to_buf("  Party hasted.     ");
								else 
									add_string_to_buf("  Party blessed!           ");
								break;
								

								
							case 32: case 47: case 56: // affect monsters in area spells
								adven[current_pc].cur_sp -= s_cost[0][spell_num];
								store_sound = 25;	
								if (spell_num == 47) 
									store_sound = 54;
								switch (spell_num) {
									case 32: add_string_to_buf("  Enemy slowed:       "); break;
									case 49: add_string_to_buf("  Enemy ravaged:              ");break;
									case 47: add_string_to_buf("  Enemy scared:   ");break;
									case 56: add_string_to_buf("  Enemy paralyzed:   ");break;
									}
								for (i = 0; i < T_M; i++) {
										if ((c_town.monst.dudes[i].active != 0) && (c_town.monst.dudes[i].attitude % 2 == 1) 
										 && (dist(pc_pos[current_pc],c_town.monst.dudes[i].m_loc) <= mage_range[spell_num])
										 && (can_see(pc_pos[current_pc],c_town.monst.dudes[i].m_loc,0) < 5)) {
												which_m = &c_town.monst.dudes[i];
												switch (spell_num) {
													case 47:
														r1 = get_ran(adven[current_pc].level / 3,1,8);
														scare_monst(which_m,r1);
														store_m_type = 10;
														break;
													case 32: case 49:
														slow_monst(which_m,5 + bonus);
														if (spell_num == 49) 
															curse_monst(which_m,3 + bonus);
														store_m_type = 8;
														break;
													case 56:
														charm_monst(which_m,15,12,1000);
														store_m_type = 15;
														break;
													}
											num_opp++;
											add_missile(c_town.monst.dudes[i].m_loc,store_m_type,0,
												14 * (which_m->m_d.x_width - 1),18 * (which_m->m_d.y_width - 1));
											}	
	
										}
								break;	
																
						}
					
					}
			if (num_opp < 10)
				do_missile_anim((num_opp < 5) ? 50 : 25,pc_pos[current_pc],store_sound); 
				else play_sound(store_sound);
			end_missile_anim();
			put_pc_screen();

		}
	}
	combat_posing_monster = current_working_monster = -1;
	// Did anything actually get cast?
	if (store_sp == adven[current_pc].cur_sp)
		return FALSE;
		else return TRUE;	
}


Boolean combat_cast_priest_spell()
{
	short spell_num,target,i,store_sp,bonus,store_sound = 0,store_m_type = 0,num_opp = 0;
	creature_data_type *which_m;
						
	if (is_antimagic(pc_pos[current_pc].x,pc_pos[current_pc].y)) {
		add_string_to_buf("  Not in antimagic field.");
		return FALSE;
		}
	if (spell_forced == FALSE)				
		spell_num = pick_spell(current_pc,1,2);
		else {
			if (repeat_cast_ok(1) == FALSE)
				return FALSE;
			spell_num = pc_last_cast[1][current_pc];			
			}

	store_sp = adven[current_pc].cur_sp;
	
	if (spell_num >= 70)
		return FALSE;
	bonus = pc_stat_adj(adven[current_pc], skill::Intelligence);

	combat_posing_monster = current_working_monster = current_pc;

	if (adven[current_pc].cur_sp == 0)
		add_string_to_buf("Cast: No spell points.        ");		
	else if (spell_num < 70) {
			print_spell_cast(spell_num,1);
			if (refer_priest[spell_num] == 0) {
				take_ap(5);
				draw_terrain(2);
				do_priest_spell(current_pc,spell_num);		
				}
				else if (refer_priest[spell_num] == 2) {
					start_spell_targeting(100 + spell_num);
					}
				else if (refer_priest[spell_num] == 3) {
					start_fancy_spell_targeting(100 + spell_num);
					}
				else {				
					start_missile_anim();
					take_ap(5);
					draw_terrain(2);
					switch (spell_num) {
						case 0: case 10: 
//							target = select_pc(11,0);
								target = store_spell_target;
							if (target < 6) {
								store_sound = 4;
								adven[current_pc].cur_sp -= s_cost[1][spell_num];
								adven[target].gaffect(affect::CursedBlessed) += (spell_num == 0) ? 2 : 
									max(2,(adven[current_pc].level * 3) / 4 + 1 + bonus);
								add_string_to_buf("  {} blessed.              ",
									 adven[target].name);
								add_missile(pc_pos[target],8,0,0,0);
								}						
						break;

						case 38:
							adven[current_pc].cur_sp -= s_cost[1][spell_num];		
							for (i = 0; i < 6; i++) 
								if (adven[i].main_status == status::Normal) {
									adven[i].gaffect(affect::CursedBlessed) += adven[current_pc].level / 3;										
								add_missile(pc_pos[i],8,0,0,0);
								}
							add_string_to_buf("  Party blessed.                    ");
							store_sound = 4;
							break;	
							
						case 58:
							adven[current_pc].cur_sp -= s_cost[1][spell_num];		
							add_string_to_buf("  {} is an avatar! ",
								 adven[current_pc].name);
							pc_heal(adven[current_pc],200);
							cure_pc(adven[current_pc],8);
							adven[current_pc].gaffect(affect::CursedBlessed) = 8;
							adven[current_pc].gaffect(affect::Speed) = 8;
							adven[current_pc].gaffect(affect::Invulnerable) = 3;
							adven[current_pc].gaffect(affect::MagicResistant) = 8;
							adven[current_pc].gaffect(affect::Webbed) = 0;
							adven[current_pc].gaffect(affect::Diseased) = 0;
							adven[current_pc].gaffect(affect::Dumbfounded) = 0;
							adven[current_pc].gaffect(affect::MartyrsShield) = 8;
							break;
							
						case 31: case 51: case 53:			
								adven[current_pc].cur_sp -= s_cost[1][spell_num];		
								store_sound = 24;
								for (i = 0; i < T_M; i++) {
									if ((c_town.monst.dudes[i].active != 0) &&(c_town.monst.dudes[i].attitude % 2 == 1) &&
									 (dist(pc_pos[current_pc],c_town.monst.dudes[i].m_loc) <= priest_range[spell_num])) {
											which_m = &c_town.monst.dudes[i];
											switch (spell_num) {
												case 31:
													curse_monst(which_m,3 + bonus);
													store_m_type = 8;
													break;
												case 51:
													charm_monst(which_m,28 - bonus,0,0);	
													store_m_type = 14;
													break;
												case 53:
													disease_monst(which_m,3 + bonus);
													store_m_type = 0;
													break;
												}
											num_opp++;
											add_missile(c_town.monst.dudes[i].m_loc,store_m_type,0,
												14 * (which_m->m_d.x_width - 1),18 * (which_m->m_d.y_width - 1));
											}
									}
															
								break;	
								
						case 52:
								adven[current_pc].cur_sp -= s_cost[1][spell_num];		
								play_sound(24);
								add_string_to_buf("  Protective field created.");
								place_spell_pattern(protect_pat,pc_pos[current_pc],0,FALSE,6);
							break;
					}
				}
			if (num_opp < 10)
				do_missile_anim((num_opp < 5) ? 50 : 25,pc_pos[current_pc],store_sound); 
				else play_sound(store_sound);
			end_missile_anim();
			put_pc_screen();			
		}

	combat_posing_monster = current_working_monster = -1;
	// Did anything actually get cast?
	if (store_sp == adven[current_pc].cur_sp)
		return FALSE;
		else return TRUE;	
}

void start_spell_targeting(short num)
{

	// First, remember what spell was cast.
	spell_being_cast = num;
	
	// Then, is num >= 1000, it's a freebie, so remove the 1000
	if (num >= 1000)
		num -= 1000;
	add_string_to_buf("  Target spell.                ");
	if (num < 100)		
		add_string_to_buf("  (Hit 'm' to cancel.)");
		else add_string_to_buf("  (Hit 'p' to cancel.)");
	overall_mode = 11;
	current_spell_range = (num >= 100) ? priest_range[num - 100] : mage_range[num];
	current_pat = single;

	switch (num) {  // Different spells have different messages and diff. target shapes
		case 19:
			current_pat = small_square;
			break;
		case 18: case 22: case 141: case 126: case 15: case 119:
			current_pat = square;
			break;
		case 17: case 40: case 44: case 28: case 51: case 157: case 145: case 64: case 67:
			current_pat = rad_pat2;
			break;
		case 153: case 65: case 66:
			current_pat = rad_pat3;
			break;
		case 25: case 37: case 159:
			add_string_to_buf("  (Hit space to rotate.)");
			force_wall_position = 0;
			current_pat = field[0];
			break;
		}	
}

void start_fancy_spell_targeting(short num)
{
	short i;
	location null_loc = {120,0};
	
	// First, remember what spell was cast.
	spell_being_cast = num;
		// Then, is num >= 1000, it's a freebie, so remove the 1000
	if (num >= 1000)
		num -= 1000;

	for (i = 0; i < 8; i++)
		spell_targets[i] = null_loc;
	add_string_to_buf("  Target spell.                ");
	if (num < 100)		
		add_string_to_buf("  (Hit 'm' to cancel.)");
		else add_string_to_buf("  (Hit 'p' to cancel.)");
	add_string_to_buf("  (Hit space to cast.)");
	overall_mode = 14;
	current_pat = single;
	current_spell_range = (num >= 100) ? priest_range[num - 100] : mage_range[num];
	
	switch (num) { // Assign special targeting shapes and number of targets
		case 129: // smite
			num_targets_left = boe_clamp(adven[current_pc].level / 4 + pc_stat_adj(adven[current_pc], skill::Intelligence) / 2,1,8);
			break; 
		case 134: // sticks to snakes
			num_targets_left = adven[current_pc].level / 5 + pc_stat_adj(adven[current_pc], skill::Intelligence) / 2;
			break;
		case 143: // summon host
			num_targets_left = 5;
			break;
		case 27: // flame arrows
			num_targets_left = adven[current_pc].level / 4 + pc_stat_adj(adven[current_pc], skill::Intelligence) / 2;
			break;
		case 36: // venom arrows
			num_targets_left = adven[current_pc].level / 5 + pc_stat_adj(adven[current_pc], skill::Intelligence) / 2;
			break;
		case 61: case 49: // paralysis, death arrows
			num_targets_left = adven[current_pc].level / 8 + pc_stat_adj(adven[current_pc], skill::Intelligence) / 3;
			break;
		case 45: // spray fields
			num_targets_left = adven[current_pc].level / 5 + pc_stat_adj(adven[current_pc], skill::Intelligence) / 2;
			current_pat = t;
			break;
		case 26: // summon 1
			num_targets_left = boe_clamp(adven[current_pc].level / 4 + pc_stat_adj(adven[current_pc], skill::Intelligence) / 2,1,7);
			break;
		case 43: // summon 2
			num_targets_left = boe_clamp(adven[current_pc].level / 6 + pc_stat_adj(adven[current_pc], skill::Intelligence) / 2,1,6);
			break;
		case 58: // summon 3
			num_targets_left = boe_clamp(adven[current_pc].level / 8 + pc_stat_adj(adven[current_pc], skill::Intelligence) / 2,1,5);
			break;
		}	
	
	num_targets_left = boe_clamp(num_targets_left,1,8);
}

void spell_cast_hit_return()
{
	if (force_wall_position < 10) {
		force_wall_position = (force_wall_position + 1) % 8;
		current_pat = field[force_wall_position];
		}
}

void process_fields()
{
	short i,j,k,r1;
	location loc;
	char qf[64][64];
	BoeRect r;

	if (is_out())
		return;
	
	if (quickfire) {
		r = c_town.town.in_town_rect;
		for (i = 0; i < town_size[town_type]; i++)
			for (j = 0; j < town_size[town_type]; j++) 
				qf[i][j] = is_quickfire(i,j) ? 2 : 0;
		for (k = 0; k < (is_combat() ? 4 : 1); k++) {
			for (i = r.left + 1; i < r.right ; i++)
				for (j = r.top + 1; j < r.bottom ; j++) 			
					if (is_quickfire(i,j)) {
						r1 = rand_short(1,8);
						if (r1 != 1) {
							qf[i - 1][j] = 1;
							qf[i + 1][j] = 1;
							qf[i][j + 1] = 1;
							qf[i][j - 1] = 1;
							}
						}
			for (i = r.left + 1; i < r.right ; i++)
				for (j = r.top + 1; j < r.bottom ; j++) 
					if (qf[i][j] > 0) {
					make_quickfire(i,j);
					}
			}
		}

	for (i = 0; i < T_M; i++)
		if (c_town.monst.dudes[i].active > 0)
			monst_inflict_fields(i);

	// First fry PCs, then call to handle damage to monsters
	processing_fields = TRUE; // this, in hit_space, makes damage considered to come from whole party
	if (force_wall) {
		force_wall = FALSE;
		for (i = 0; i < town_size[town_type]; i++)
			for (j = 0; j < town_size[town_type]; j++) 
				if (is_force_wall(i,j)) {
							r1 = get_ran(3,1,6);
							loc.x = i; loc.y = j;
							hit_pcs_in_space(loc,r1, damage_type::GeneralMagic,1,1);
					r1 = rand_short(1,6);
					if (r1 == 2)
						take_force_wall(i,j);
						else {
							force_wall = TRUE;
							}
					}
		}
	if (fire_wall) {
		fire_wall = FALSE;
		for (i = 0; i < town_size[town_type]; i++)
			for (j = 0; j < town_size[town_type]; j++) 
				if (is_fire_wall(i,j)) {
							loc.x = i; loc.y = j;
							r1 = get_ran(2,1,6) + 1;
							 hit_pcs_in_space(loc,r1, damage_type::Fire,1,1);
					r1 = rand_short(1,4);
					if (r1 == 2)
						take_fire_wall(i,j);
						else {
							fire_wall = TRUE;
							}
					}
		}
	if (antimagic) {
		antimagic = FALSE;
		for (i = 0; i < town_size[town_type]; i++)
			for (j = 0; j < town_size[town_type]; j++) 
				if (is_antimagic(i,j)) {
					r1 = rand_short(1,8);
					if (r1 == 2)
						take_antimagic(i,j);
						else antimagic = TRUE;
					}
		}
	if (scloud) {
		scloud = FALSE;
		for (i = 0; i < town_size[town_type]; i++)
			for (j = 0; j < town_size[town_type]; j++) 
				if (is_scloud(i,j)) {
					r1 = rand_short(1,4);
					if (r1 == 2)
						take_scloud(i,j);
						else {
							scloud_space(i,j);
							scloud = TRUE;
							}
					}
		}		
	if (sleep_field) {
		sleep_field = FALSE;
		for (i = 0; i < town_size[town_type]; i++)
			for (j = 0; j < town_size[town_type]; j++) 
				if (is_sleep_cloud(i,j)) {
					r1 = rand_short(1,4);
					if (r1 == 2)
						take_sleep_cloud(i,j);
						else {
							sleep_cloud_space(i,j);
							sleep_field = TRUE;
							}
					}
		}	
	if (ice_wall) {
		ice_wall = FALSE;
		for (i = 0; i < town_size[town_type]; i++)
			for (j = 0; j < town_size[town_type]; j++) 
				if (is_ice_wall(i,j)) {
							loc.x = i; loc.y = j;
							r1 = get_ran(3,1,6);
							hit_pcs_in_space(loc,r1, damage_type::Cold,1,1);
					r1 = rand_short(1,6);
					if (r1 == 1)
						take_ice_wall(i,j);
						else {
							ice_wall = TRUE;
							}
					}
		}		
	if (blade_wall) {
		blade_wall = FALSE;
		for (i = 0; i < town_size[town_type]; i++)
			for (j = 0; j < town_size[town_type]; j++) 
				if (is_blade_wall(i,j)) {
							loc.x = i; loc.y = j;
							r1 = get_ran(6,1,8);
							hit_pcs_in_space(loc,r1, damage_type::Weapon,1,1);
					r1 = rand_short(1,5);
					if (r1 == 1)
						take_blade_wall(i,j);
						else {
							blade_wall = TRUE;
							}
					}
		}
	
	processing_fields = FALSE;
	monsters_going = TRUE; // this changes who the damage is considered to come from
							// in hit_space
	
	if (quickfire) {
		for (i = 0; i < town_size[town_type]; i++)
			for (j = 0; j < town_size[town_type]; j++)
				if (is_quickfire(i,j)) {
					loc.x = i; loc.y = j;
					r1 = get_ran(2,1,8);
					hit_pcs_in_space(loc,r1, damage_type::Fire,1,1);
					}
		}	

	monsters_going = FALSE;	
}

void scloud_space(short m,short n)
{
	location target;
	short i;
	
	target.x = (char) m;
	target.y = (char) n;

	make_scloud(m,n);
	
	if (overall_mode >= 10)
		for (i = 0; i < 6; i++)
			if (adven[i].main_status == status::Normal)
				if (same_point(pc_pos[i],target) == TRUE) {
						curse_pc(i,rand_short(1,2));					
					}
	if (overall_mode < 10)
		if (same_point(target,c_town.p_loc) == TRUE) {
			for (i = 0; i < 6; i++)
				if (adven[i].main_status == status::Normal)
					curse_pc(i,rand_short(1,2));					
			}
}

void web_space(short m,short n)
{
	location target;
	short i; 
	
	target.x = (char) m;
	target.y = (char) n;

	make_web(m,n);

	if (overall_mode >= 10)
		for (i = 0; i < 6; i++)
			if (adven[i].main_status == status::Normal)
				if (same_point(pc_pos[i],target) == TRUE) {
						web_pc(i,3);					
					}
	if (overall_mode < 10)
		if (same_point(target,c_town.p_loc) == TRUE) {
			for (i = 0; i < 6; i++)
					web_pc(i,3);					
			}
}
void sleep_cloud_space(short m,short n)
{
	location target;
	short i; 
	
	target.x = (char) m;
	target.y = (char) n;

	make_sleep_cloud(m,n);

	if (overall_mode >= 10)
		for (i = 0; i < 6; i++)
			if (adven[i].main_status == status::Normal)
				if (same_point(pc_pos[i],target) == TRUE) {
					sleep_pc(i,3, affect::Asleep,0);
					}
	if (overall_mode < 10)
		if (same_point(target,c_town.p_loc) == TRUE) {
			for (i = 0; i < 6; i++)
				sleep_pc(i,3, affect::Asleep,0);
			}
}


void take_m_ap(short num,creature_data_type *monst)
{
	monst->m_d.ap = max(0,monst->m_d.ap - num);
}

void add_new_action(short pc_num)
{
	if (pc_num < 6)
		pc_moves[pc_num]++;
}

short get_monst_sound(creature_data_type *attacker,short which_att) {
	short type,strength;
	
	type = (which_att == 0) ? attacker->m_d.a1_type :  attacker->m_d.a23_type;
	strength = attacker->m_d.a[which_att];
	
	switch (type) {
		case 3:
			return 11;
			break;
		case 4: 
			return 4;
			break;
		case 1: 
			return 9;
			break;
		case 2: 
			return 10;
			break;
			
		default:
			if (attacker->m_d.m_type == 0) {
				if (strength > 9)
					return 3;
					else return 2;	
				}
			if ((attacker->m_d.m_type == 0) ||(attacker->m_d.m_type == 6) ||(attacker->m_d.m_type == 9) ){
				return 2;
				}
			if (attacker->m_d.m_type == 4)
				return 1;
			if (attacker->m_d.m_type == 5)
				return 4;
			return 0;
			break;
		}
	return 0;
	}
