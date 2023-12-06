#include "../game_file_structures.hpp"

#define DRAG_EDGE		15

#define	DISPLAY_LEFT	23
#define	DISPLAY_TOP		23
#define	BITMAP_WIDTH	28
#define	BITMAP_HEIGHT	36
#define	NUM_BUTTONS		15

#define	NUM_DLOG_B		53

#define NLS	25
// number of left slots for buttons
#define	NRS	400
// number of right slots for scrolling list
#define NRSONPAGE	33
// number of right slots for scrolling list on page at 1 time

#define	RIGHT_AREA_UL_X	287
#define	RIGHT_AREA_UL_Y	5
#define	RIGHT_AREA_WIDTH	290
#define	RIGHT_AREA_HEIGHT	400

#define	TER_RECT_UL_X	6
#define	TER_RECT_UL_Y	25
#define	GWorldPtr	HBITMAP

#define	CDGT	cd_retrieve_text_edit_str
#define	CDGN	cd_retrieve_text_edit_num
#define	CDST	cd_set_text_edit_str
#define	CDSN	cd_set_text_edit_num

// for game
typedef struct {
	short personality;
	short str1,str2;
	} talk_save_type;



typedef struct {
	short active,attitude;
	unsigned char number;
	location m_loc;
	monster_record_type m_d;
	short mobile;
	short summoned;
	creature_start_type monst_start;
	} creature_data_type;

typedef struct {
	creature_data_type dudes[60];
	short which_town;
	short friendly;
	} creature_list_type;

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
	char spec_items[50];
	} party_record_type;

typedef struct {
	char strings_ls[NLS][40];
	char strings_rs[NRS][40];
	char out_strs[120][256];

	} piles_of_stuff_dumping_type;
	
short get_ran (short times,short  min,short  max);
short s_pow(short x,short y);
short dist(location p1,location p2);
void pause(short length);
void beep();
void SysBeep(short a);
void Delay(short val,long *dummy);

short minmax(short min,short max,short k);
short abs(short x);
