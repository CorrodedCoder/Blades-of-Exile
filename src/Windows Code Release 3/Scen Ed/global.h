#include "boe/game_file_structures.hpp"

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
	char strings_ls[NLS][40];
	char strings_rs[NRS][40];
	char out_strs[120][256];

	} piles_of_stuff_dumping_type;
	
short s_pow(short x,short y);
short dist(location p1,location p2);
void pause(short length);
void beep();
void SysBeep(short a);
void Delay(short val,long *dummy);

short abs(short x);
