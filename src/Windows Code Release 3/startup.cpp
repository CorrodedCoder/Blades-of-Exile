#include <Windows.h>
#include <array>
#include <cstdio>

#include "global.h"
#include "graphics.h"
#include "fileio.h"
#include "actions.h"
#include "dlgutils.h"
#include "items.h"
#include "party.h"
#include "exlsound.h"
#include "startup.h"
#include "game_globals.hpp"

extern party_record_type party;
extern scen_header_type scen_headers[25];
extern Boolean unreg_party_in_scen_not_check;
extern Boolean in_startup_mode,registered,play_sounds,party_in_memory;
extern short ulx,uly;

//void start_game();

std::array<RECT, 6> startup_button;

Boolean handle_startup_press(POINT the_point)
{
	short i,scen;

	the_point.x -= ulx;
	the_point.y -= uly;
	
	for (i = 0; i < 5; i++) 
		if (PtInRect(&startup_button[i],the_point) == TRUE) {
//			flash_round_rect(start_button_rects[i],10);
			//OffsetRect(&start_button_rects[i],ul.h,ul.v);
			//startup_button_click(start_button_rects[i]);
			
		draw_start_button(i,5);
		if (play_sounds == TRUE)
			play_sound(37);
			else Delay(5);
		draw_start_button(i,0);			
			switch (i) {
				case 0:
					startup_load();
					break;
					
				case 1:
					draw_startup(0);
					start_new_game();
					update_pc_graphics();
					draw_startup(0);
					break;
			
				case 2:
					give_reg_info();
						draw_startup(0);
					break;

				case 3:
					if (party_in_memory == FALSE) {
						FCD(867,0);
						break;
						}
					scen = pick_prefab_scen();
					if (scen < 0)
						break;
					if ((registered == FALSE) && (scen > 0)) {
						FCD(913,0);
						break;
						}

					switch (scen) {
						case 0: format_to_buf(party.scen_name,"valleydy.exs"); break;
						// if not reg, rub out
						case 1: format_to_buf(party.scen_name,"stealth.exs"); break;
						case 2: format_to_buf(party.scen_name,"zakhazi.exs"); break;
						}
					put_party_in_scen();
					break;

				case 4: // intro
					if (party_in_memory == FALSE) {
						FCD(867,0);
						break;
						}
					if (registered == FALSE) {
						FCD(913,0);
						break;
						}
					// if not reg, rub out

					scen = pick_a_scen();
					if (scen_headers[scen].prog_make_ver[0] >= 2) {
						FCD(912,0);
						break;
						}
					if (scen >= 0) {
						if (registered == FALSE)
							unreg_party_in_scen_not_check = TRUE;
						format_to_buf(party.scen_name,"{}", scenario_ext.name_string(scen));
						put_party_in_scen();
						}

					break;

				case 5:
					Delay(50);
					return TRUE;
					break;
			
				}
			}
	return FALSE;
}
/*
void start_game () 
{
	init_party(0);

	setup_outdoors(party.p_loc);

	load_area_graphics();
	
	draw_main_screen();
	
	in_startup_mode = FALSE;
	
	adjust_monst_menu();
	adjust_spell_menus();
	
}*/

void startup_load()////
{
					load_file();
					update_pc_graphics();
					if (in_startup_mode == FALSE) {
						//end_anim();
						end_startup();
						post_load();
						}
						else {
							menu_activate(0);
							draw_startup(0);
							}

}
