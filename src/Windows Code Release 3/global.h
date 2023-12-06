#include "game_file_structures.hpp"

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

// for game
typedef struct {
	short personality;
	short town_num;
	short str1,str2;
	} talk_save_type;




typedef struct {
	short active,attitude;
	unsigned char number;
	location m_loc;
	monster_record_type m_d;
	Boolean mobile;
	short summoned;
	creature_start_type monst_start;
	} creature_data_type;
	
	

typedef struct {
	creature_data_type dudes[60];
	short which_town;
	short friendly;
	} creature_list_type;

typedef struct {
	short town_num, difficulty;
	town_record_type	town;
	char explored[64][64];
	Boolean	hostile;
	creature_list_type	monst;
	Boolean	in_boat;
	location p_loc;
	}	current_town_type;

typedef struct {
	Boolean exists;
	short direction;
	out_wandering_type what_monst;
	location which_sector,m_loc;	
	} outdoor_creature_type;
	
typedef struct {
	long age;
	short gold,food;
	unsigned char stuff_done[310][10],item_taken[200][8];
	short light_level;
	location outdoor_corner,i_w_c,p_loc,loc_in_sec;
	boat_record_type boats[30];
	horse_record_type horses[30];
	creature_list_type creature_save[4];
	short in_boat,in_horse;
	outdoor_creature_type out_c[10];
	item_record_type magic_store_items[5][10];
	short imprisoned_monst[4];
	char m_seen[256];
	char journal_str[50];
	short journal_day[50];
	short special_notes_str[140][2];
	talk_save_type talk_save[120];
	short direction,at_which_save_slot;
	char alchemy[20];
	Boolean can_find_town[200];
	short key_times[100];
	short party_event_timers[30];
	short global_or_town[30];
	short node_to_call[30];
	char spec_items[50],help_received[120];
	short m_killed[200];
	long total_m_killed,total_dam_done,total_xp_gained,total_dam_taken;
	char scen_name[256];
	} party_record_type;

typedef struct {
	char town_maps[100][8][64];
	} stored_town_maps_type;
typedef struct {
	char town_strs[180][256];
	} piles_of_stuff_dumping_type;
typedef struct {
	char scen_header_strs[25][3][80];
	char scen_names[25][256];
	scen_item_data_type scen_item_list;
	} piles_of_stuff_dumping_type2;
typedef struct {
	char far talk_strs[170][256];
	} piles_of_stuff_dumping_type3;
typedef struct {
	char out_strs[9][256];
	} outdoor_strs_type;
typedef struct {
	outdoor_strs_type far outdoor_text[2][2];
	} piles_of_stuff_dumping_type4;
typedef struct {
char far scen_strs[160][256];
	} piles_of_stuff_dumping_type5;


typedef struct {
	short main_status;
	char name[20];
	short skills[30];
	short max_health,cur_health,max_sp,cur_sp,experience,skill_pts,level;
	short status[15];
	item_record_type items[24];
	Boolean equip[24];
	Boolean priest_spells[62],mage_spells[62];
	short which_graphic,weap_poisoned;
	Boolean advan[15],traits[15];
	short race,exp_adj,direction;
	} pc_record_type;

typedef struct {
	unsigned char setup[4][64][64];
	} setup_save_type;
	

typedef struct {
	item_record_type items[NUM_TOWN_ITEMS];
	} town_item_list;

typedef struct {short i;} flag_type;

typedef struct {unsigned char pattern[9][9];} effect_pat_type;

typedef struct {
	item_record_type items[NUM_TOWN_ITEMS];
	} stored_items_list_type;

typedef struct {
	char outdoor_maps[100][6][48];
	} stored_outdoor_maps_type; 


short s_pow(short x,short y);
short a_v(short x);
short ex_abs(short x);
short get_ran (short times,short  min,short  max);
Boolean same_point(location p1,location p2);
void pause(short length);
short minmax(short min,short max,short k);
short move_to_zero(short val);
void Delay(short val,long *dummy);
void alter_rect(RECT *r) ;
Boolean sd_legit(short a, short b);
