
#include <Windows.h>
#include <array>
#include <fstream>
#include <exception>
#include <format>
#include <cassert>
#include <commdlg.h>

#include <cstring>
#include "global.h"
#include <cstdio>
#include "fileio.h"
#include "text.h"
#include "town.h"
#include "items.h"
#include "graphics.h"
#include "locutils.h"
#include "fields.h"
#include "newgraph.h"
#include "dlgutils.h"
#include "gutils.h"
#include "infodlgs.h"
#include "graphutl.h"
#include "exlsound.h"
#include "boe/endian_adjust.hpp"
#include "boe/savedata_serialization.hpp"
#include "boe/utility.hpp"
#include "scenario.hpp"

HWND	the_dialog;
extern party_record_type far	party;
extern Adventurers adven;
extern outdoor_record_type outdoors[2][2];
extern unsigned char out[96][96];
extern unsigned char out_e[96][96];
extern short overall_mode, give_delays, stat_screen_mode;
extern Boolean in_startup_mode, registered, play_sounds, in_scen_debug, sys_7_avail, save_maps, party_in_memory;
extern current_town_type	c_town;
extern town_item_list 	t_i;
extern location center;
extern long register_flag;
extern HWND mainPtr;
extern stored_items_list_type stored_items[3];
extern stored_outdoor_maps_type o_maps;
extern big_tr_type  t_d;
extern short town_size[3];
extern short town_type, current_pc;
extern Boolean web, crate, barrel, fire_barrier, force_barrier, quickfire, force_wall, fire_wall, antimagic, scloud, ice_wall, blade_wall;
extern Boolean sleep_field;
extern setup_save_type setup_save;
extern unsigned char misc_i[64][64], sfx[64][64];
extern unsigned char template_terrain[64][64];
extern town_record_type anim_town;
extern tiny_tr_type anim_t_d;
extern Boolean modeless_exists[18];
extern location monster_targs[T_M];
extern HWND modeless_dialogs[18];
extern short which_combat_type;
extern Boolean game_run_before;
extern Boolean ed_reg;
extern long ed_flag, ed_key;
extern short display_mode, cur_town_talk_loaded;
extern Boolean give_intro_hint;
extern char terrain_blocked[256];
extern char scen_strs2[110][256];
extern stored_town_maps_type town_maps, town_maps2;
extern scenario_data_type scenario;
extern unsigned char combat_terrain[64][64];
extern piles_of_stuff_dumping_type data_store;
extern piles_of_stuff_dumping_type2 data_store2;
extern talking_record_type talking;

extern short terrain_pic[256];
extern scen_header_type scen_headers[25];
HBITMAP spec_scen_g = NULL;

extern HDC main_dc, main_dc2, main_dc3;
extern piles_of_stuff_dumping_type3 data_store3;
extern piles_of_stuff_dumping_type4 data_store4;
extern piles_of_stuff_dumping_type5 data_store5;

typedef struct {
	char expl[96][96];
}	out_info_type;

Boolean loaded_yet = FALSE, got_nagged = FALSE, ae_loading = FALSE;
Boolean cur_scen_is_win = TRUE;

void save_outdoor_maps();
void add_outdoor_maps();

static char szFileName[128] = "blades.sav";
static char szTitleName[128] = "blades.sav";
OPENFILENAME ofn;
OFSTRUCT save_dir, save_dir2;

// Trying this to fix bug. Hope it works.
tiny_tr_type tiny_t;
ave_tr_type ave_t;

outdoor_record_type dummy_out;////
town_record_type dummy_town;
short jl;
extern char file_path_name[256];

static const char szFilter[]{
	"Blades of Exile Save Files (*.SAV)\0" "*.sav\0"
	"Text Files (*.TXT)\0" "*.txt\0"
	"All Files (*.*)\0" "*.*\0"
	};

static bool FSClose(auto& file)
{
	file.close();
	return !file.fail();
}

static bool SetFPos(auto& file, long len, std::ios_base::seekdir origin)
{
	file.seekg(len, origin);
	return !file.fail();
}

static bool file_read_type(auto& file, auto& type)
{
	file.read(reinterpret_cast<char*>(&type), sizeof(type));
	return !file.fail();
}

#if 0
static void stream_read_type(std::istream& file, auto& type)
{
	assert((file.exceptions() & std::ios_base::failbit) == std::ios_base::failbit);
	file.read(reinterpret_cast<char*>(&type), sizeof(type));
}

template< typename T >
static T stream_read_type(std::istream& file)
{
	assert((file.exceptions() & std::ios_base::failbit) == std::ios_base::failbit);
	T type;
	file.read(reinterpret_cast<char*>(&type), sizeof(type));
	return type;
}
#endif

static void file_read_string(std::ifstream& file, long len, char* arr)
{
	file.read(arr, len);
	arr[len] = '\0';
}

static bool file_write_type(auto& file, const auto& type)
{
	file.write(reinterpret_cast<const char*>(&type), sizeof(type));
	return !file.fail();
}

#if 0
static void stream_write_type(auto& file, const auto& type)
{
	assert((file.exceptions() & std::ios_base::failbit) == std::ios_base::failbit);
	file.write(reinterpret_cast<const char*>(&type), sizeof(type));
}
#endif

void file_initialize()
{
	OpenFile("outdoor.dat", &save_dir, OF_PARSE);
	OpenFile("town.dat", &save_dir2, OF_PARSE);

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = mainPtr;
	ofn.hInstance = NULL;
	ofn.lpstrFilter = szFilter;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = NULL;
	ofn.nMaxFile = 128;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 128;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = NULL;
	ofn.Flags = 0;
	ofn.nFileOffset = 0;
	ofn.nFileExtension = 0;
	ofn.lpstrDefExt = "txt";
	ofn.lCustData = 0L;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
}

void load_file()
{
	ofn.hwndOwner = mainPtr;
	ofn.lpstrFile = szFileName;
	ofn.lpstrFileTitle = szTitleName;
	ofn.Flags = 0;

	if (GetOpenFileName(&ofn) == 0)
		return;
	// Was: _lopen(szFileName, OF_READ | OF_SHARE_DENY_WRITE);
	std::ifstream file_id;
	file_id.open(szFileName, std::ios_base::binary);
	if (file_id.fail()) {
		beep();
		return;
	}

	bool town_restore = false;
	bool in_scen = false;

	try
	{
		file_id.exceptions(std::ios_base::failbit);
		savedata_read_all(file_id, town_restore, in_scen,
			party, setup_save, adven, out_e, c_town, t_d, t_i, stored_items,
			town_maps, town_maps2, o_maps, sfx, misc_i
		);
	}
	catch (boe_error const& e)
	{
		FCD(e.error_code(), 0);
		return;
	}
	catch (std::ios_base::failure const&)
	{
		SysBeep(2);
		FCD(1064, 0);
		return;
	}

	if (!FSClose(file_id)) {
		add_string_to_buf("Load: Can't close file.          ");
		SysBeep(2);
		return;
	}

	party_in_memory = TRUE;

	// now if not in scen, this is it.
	if (!in_scen) {
		if (in_startup_mode == FALSE) {
			reload_startup();
			in_startup_mode = TRUE;
			draw_startup(0);
		}
		return;
	}

	if (load_scenario() == FALSE)
		return;
	// if at this point, startup must be over, so make this call to make sure we're ready,
	// graphics wise
	end_startup();

	set_up_ter_pics();
	load_outdoors(party.outdoor_corner.x + 1, party.outdoor_corner.y + 1, 1, 1, 0, 0, NULL);
	load_outdoors(party.outdoor_corner.x, party.outdoor_corner.y + 1, 0, 1, 0, 0, NULL);
	load_outdoors(party.outdoor_corner.x + 1, party.outdoor_corner.y, 1, 0, 0, 0, NULL);
	load_outdoors(party.outdoor_corner.x, party.outdoor_corner.y, 0, 0, 0, 0, NULL);

	overall_mode = town_restore ? 1 : 0;
	stat_screen_mode = 0;
	build_outdoors();
	erase_out_specials();
	update_pc_graphics();

	//SelectWindow(mainPtr);
	//SetPort(mainPtr);
	//if (modeless_exists[5] == TRUE)
	//	BringToFront(modeless_dialogs[5]);

	if (!town_restore) {
		center = party.p_loc;
		load_area_graphics();
	}
	else {
		load_town(c_town.town_num, 2, -1, NULL);
		load_town(c_town.town_num, 1, -1, NULL);
		for (short i = 0; i < T_M; i++)
		{
			monster_targs[i].x = 0;  monster_targs[i].y = 0;
		}

		town_type = scenario_town_size(c_town.town_num);
		// Set up field booleans
		for (short j = 0; j < town_size[town_type]; j++)
			for (short k = 0; k < town_size[town_type]; k++) {
				// Set up field booleans
				if (is_web(j, k) == TRUE)
					web = TRUE;
				if (is_crate(j, k) == TRUE)
					crate = TRUE;
				if (is_barrel(j, k) == TRUE)
					barrel = TRUE;
				if (is_fire_barrier(j, k) == TRUE)
					fire_barrier = TRUE;
				if (is_force_barrier(j, k) == TRUE)
					force_barrier = TRUE;
				if (is_quickfire(j, k) == TRUE)
					quickfire = TRUE;
			}
		force_wall = TRUE; fire_wall = TRUE; antimagic = TRUE; scloud = TRUE; ice_wall = TRUE; blade_wall = TRUE;
		sleep_field = TRUE;
		center = c_town.p_loc;
		load_area_graphics();
	}

	create_clip_region();
	redraw_screen(0);
	//else flood_bg();

	current_pc = first_active_pc();
	loaded_yet = TRUE;


	add_string_to_buf("Load: Game loaded.            ");

	// Set sounds, map saving, and speed
	if (((play_sounds == TRUE) && (party.stuff_done[306][1] == 1)) ||
		((play_sounds == FALSE) && (party.stuff_done[306][1] == 0))) {
		flip_sound();
	}
	give_delays = party.stuff_done[306][2];
	if (party.stuff_done[306][0] == 0)
		save_maps = TRUE;
	else save_maps = FALSE;

	in_startup_mode = FALSE;
	in_scen_debug = FALSE;


}

void save_file(short mode)
//mode;  // 0 - normal  1 - save as
{
	Boolean town_save = FALSE;
	if ((in_startup_mode == FALSE) && (is_town()))
		town_save = TRUE;

	ofn.hwndOwner = mainPtr;
	ofn.lpstrFile = szFileName;
	ofn.lpstrFileTitle = szTitleName;
	ofn.Flags = OFN_OVERWRITEPROMPT;

	if ((mode == 1) || (in_startup_mode == TRUE)) {
		if (GetSaveFileName(&ofn) == 0)
			return;
	}

	// Was: _lopen(szFileName, OF_WRITE | OF_SHARE_EXCLUSIVE)
	// And then if it failed: _lcreat(szFileName, 0)
	std::ofstream file_id;
	file_id.open(szFileName, std::ios_base::binary);
	if (file_id.fail())
	{
		beep();
		return;
	}

	try
	{
		file_id.exceptions(std::ios_base::failbit);

		savedata_write_all(file_id, town_save, in_startup_mode, save_maps,
			party, setup_save, adven, out_e, c_town, t_d, t_i,
			stored_items, town_maps, town_maps2, o_maps, sfx, misc_i);
	}
	catch (std::ios_base::failure const&)
	{
		add_string_to_buf("Save: Couldn't write to file.         ");
		SysBeep(2);
		return;
	}
	if (!FSClose(file_id)) {
		add_string_to_buf("Save: Couldn't close file.         ");
		SysBeep(2);
		return;
	}

	if (in_startup_mode == FALSE)
		add_string_to_buf("Save: Game saved.              ");
}

void set_terrain(location l, unsigned char terrain_type)
{
	t_d.terrain[l.x][l.y] = terrain_type;
	combat_terrain[l.x][l.y] = terrain_type;
}

void change_rect_terrain(BoeRect r, unsigned char terrain_type, short probability, Boolean hollow)
// prob is 0 - 20, 0 no, 20 always
{
	location l;
	short i, j;

	for (i = 0; i < town_size[town_type]; i++)
		for (j = 0; j < town_size[town_type]; j++) {
			l.x = i;
			l.y = j;
			if ((i >= r.left) && (i <= r.right) && (j >= r.top) && (j <= r.bottom)
				&& ((hollow == FALSE) || (i == r.left) || (i == r.right) || (j == r.top) || (j == r.bottom))
				&& ((hollow == TRUE) || (rand_short(1, 20) <= probability)))
				set_terrain(l, terrain_type);
		}
}

void build_scen_file_name(char* file_n)
{
	short i, last_slash = -1;

	for (i = 0; i < 256; i++)
		if ((file_path_name[i] == 92) || (file_path_name[i] == '/'))
			last_slash = i;
	if (last_slash < 0) {
		format_to_buf(file_n, "BLADSCEN/{}", party.scen_name);
		return;
	}
	strcpy(file_n, file_path_name);
	file_n += last_slash + 1;
	format_to_buf(file_n, "BLADSCEN/{}", party.scen_name);
}

void build_scen_ed_name(char* file_n)
{
	short i, last_slash = -1;

	for (i = 0; i < 256; i++)
		if ((file_path_name[i] == 92) || (file_path_name[i] == '/'))
			last_slash = i;
	if (last_slash < 0) {
		format_to_buf(file_n, "BLSCENED/bladdata.bld");
		return;
	}
	strcpy(file_n, file_path_name);
	file_n += last_slash + 1;
	format_to_buf(file_n, "BLSCENED/bladdata.bld");
}

// mode 0 want town and talking, 1 talking only, 2 want a string only, and extra is string num
// Hey's let's be kludgy and overload these value again! If extra is -1, and mode 2, that
// means we want to load all the strings and only the strings
void load_town(short town_num, short mode, short extra, char* str)
{

	std::ifstream file_id;
	short i, j;
	long store;
	bool success = false;
	long len_to_jump = 0;
	short which_town;
	char file_name[256];

	if (town_num != boe_clamp(town_num, 0, scenario_num_towns() - 1)) {
		give_error("The scenario tried to place you into a non-existant town.", "", 0);
		return;
	}

	which_town = town_num;

	//HGetVol((StringPtr) start_name,&start_volume,&start_dir);
	build_scen_file_name(file_name);
	// Was: OpenFile(file_name, &store_str, OF_READ /* | OF_SEARCH */);
	file_id.open(file_name, std::ios_base::binary);
	if (file_id.fail()) {
		FCD(949, 0);
		return;
	}
	len_to_jump = sizeof(scenario_data_type);
	len_to_jump += sizeof(scen_item_data_type);
	for (i = 0; i < 300; i++)
		len_to_jump += (long)scenario.scen_str_len[i];
	store = 0;
	for (i = 0; i < 100; i++)
		for (j = 0; j < 2; j++)
			store += (long)(scenario.out_data_size[i][j]);
	for (i = 0; i < which_town; i++)
		for (j = 0; j < 5; j++)
			store += (long)(scenario.town_data_size[i][j]);
	len_to_jump += store;

	if (!SetFPos(file_id, len_to_jump, std::ios_base::beg)) { FSClose(file_id); oops_error(35); }

	if (mode == 0) {
		success = file_read_type(file_id, c_town.town);
		if (cur_scen_is_win != TRUE)
		{
			endian_adjust(c_town.town);
		}
	}
	else success = file_read_type(file_id, dummy_town);
	if (!success) { FSClose(file_id); oops_error(36); }

	switch (scenario_town_size(which_town)) {
	case 0:
		if (mode == 0) {
			file_read_type(file_id, t_d);
			if (cur_scen_is_win != TRUE)
			{
				endian_adjust(t_d);
			}
		}
		else success = SetFPos(file_id, sizeof(big_tr_type), std::ios_base::cur);

		break;

	case 1:
		if (mode == 0) {
			file_read_type(file_id, ave_t);
			for (i = 0; i < 48; i++)
				for (j = 0; j < 48; j++) {
					t_d.terrain[i][j] = ave_t.terrain[i][j];
					t_d.lighting[i / 8][j] = ave_t.lighting[i / 8][j];
				}
			//print_nums(5555,5555,5555);
			//for (i = 0; i < 8; i++)
			//	for (j = 0; j < 48; j++)
			//		if (t_d.lighting[i][j] != 0) {
			//			print_nums(i,j,t_d.lighting[i][j]);
			//			}

			for (i = 0; i < 16; i++) {
				t_d.room_rect[i] = ave_t.room_rect[i];
			}
			for (i = 0; i < 40; i++) {
				t_d.creatures[i] = ave_t.creatures[i];
			}
			for (i = 40; i < 60; i++) {
				t_d.creatures[i].number = 0;
			}
			if (cur_scen_is_win != TRUE)
			{
				endian_adjust(t_d);
			}
		}
		else success = SetFPos(file_id, sizeof(ave_tr_type), std::ios_base::cur);

		break;

	case 2:
		if (mode == 0) {
			file_read_type(file_id, tiny_t);
			for (i = 0; i < 32; i++)
				for (j = 0; j < 32; j++) {
					t_d.terrain[i][j] = tiny_t.terrain[i][j];
					t_d.lighting[i / 8][j] = tiny_t.lighting[i / 8][j];
				}
			for (i = 0; i < 16; i++) {
				t_d.room_rect[i] = tiny_t.room_rect[i];
			}
			for (i = 0; i < 30; i++) {
				t_d.creatures[i] = tiny_t.creatures[i];
			}
			for (i = 30; i < 60; i++) {
				t_d.creatures[i].number = 0;
			}
			if (cur_scen_is_win != TRUE)
			{
				endian_adjust(t_d);
			}
		}
		else success = SetFPos(file_id, sizeof(tiny_tr_type), std::ios_base::cur);
		break;
	}

	for (i = 0; i < 140; i++) {
		const long len = (mode == 0) ? (long)(c_town.town.strlens[i]) : (long)(dummy_town.strlens[i]);
		switch (mode) {
		case 0:
			file_read_string(file_id, len, data_store.town_strs[i]);
			break;
		case 1:
			SetFPos(file_id, len, std::ios_base::cur);
			break;
		case 2:
			if (extra < 0) {
				file_read_string(file_id, len, data_store.town_strs[i]);
			}
			else if (i == extra) {
				file_read_string(file_id, len, str);
			}
			else SetFPos(file_id, len, std::ios_base::cur);
			break;
		}
	}

	if (mode < 2) {
		success = file_read_type(file_id, talking);
		if (cur_scen_is_win != TRUE)
		{
			endian_adjust(talking);
		}
		if (!success) { FSClose(file_id); oops_error(37); }

		for (i = 0; i < 170; i++) {
			file_read_string(file_id, (long)talking.strlens[i], data_store3.talk_strs[i]);
		}
		cur_town_talk_loaded = town_num;
	}
	if (mode == 0)
		town_type = scenario_town_size(which_town);
	if (!FSClose(file_id)) { oops_error(38); }

	// Now more initialization is needed. First need to properly create the misc_i array.

	// Initialize barriers, etc. Note non-sfx gets forgotten if this is a town recently visited.
	if (mode == 0) {
		for (i = 0; i < 64; i++)
			for (j = 0; j < 64; j++) {
				misc_i[i][j] = 0;
				sfx[i][j] = 0;
			}
		for (i = 0; i < 50; i++)
			if ((c_town.town.spec_id[i] >= 0) && (c_town.town.special_locs[i].x < 100)) {
				make_special(c_town.town.special_locs[i].x, c_town.town.special_locs[i].y);
			}
		for (i = 0; i < 50; i++) {
			if ((c_town.town.preset_fields[i].field_type > 0) && (c_town.town.preset_fields[i].field_type < 9))
				misc_i[c_town.town.preset_fields[i].field_loc.x][c_town.town.preset_fields[i].field_loc.y] =
				misc_i[c_town.town.preset_fields[i].field_loc.x][c_town.town.preset_fields[i].field_loc.y] |
				(unsigned char)(s_pow(2, c_town.town.preset_fields[i].field_type - 1));
			if ((c_town.town.preset_fields[i].field_type >= 14) && (c_town.town.preset_fields[i].field_type <= 21))
				sfx[c_town.town.preset_fields[i].field_loc.x][c_town.town.preset_fields[i].field_loc.y] =
				sfx[c_town.town.preset_fields[i].field_loc.x][c_town.town.preset_fields[i].field_loc.y] |
				(unsigned char)(s_pow(2, c_town.town.preset_fields[i].field_type - 14));

		}
	}

}

void shift_universe_left()
{
	short i, j;

	make_cursor_watch();

	save_outdoor_maps();
	party.outdoor_corner.x--;
	party.i_w_c.x++;
	party.p_loc.x += 48;
	outdoors[1][0] = outdoors[0][0];
	outdoors[1][1] = outdoors[0][1];
	data_store4.outdoor_text[1][0] = data_store4.outdoor_text[0][0];
	data_store4.outdoor_text[1][1] = data_store4.outdoor_text[0][1];

	for (i = 48; i < 96; i++)
		for (j = 0; j < 96; j++)
			out_e[i][j] = out_e[i - 48][j];

	for (i = 0; i < 48; i++)
		for (j = 0; j < 96; j++)
			out_e[i][j] = 0;

	for (i = 0; i < 10; i++) {
		if (party.out_c[i].m_loc.x > 48)
			party.out_c[i].exists = FALSE;
		if (party.out_c[i].exists == TRUE)
			party.out_c[i].m_loc.x += 48;
	}

	load_outdoors(party.outdoor_corner.x, party.outdoor_corner.y, 0, 0, 0, 0, NULL);
	load_outdoors(party.outdoor_corner.x, party.outdoor_corner.y + 1, 0, 1, 0, 0, NULL);
	build_outdoors();


	// reload graphics -- who knows what we added
	load_area_graphics();
	make_cursor_sword();

}

void shift_universe_right()
{
	short i, j;

	make_cursor_watch();
	save_outdoor_maps();
	party.outdoor_corner.x++;
	party.i_w_c.x--;
	party.p_loc.x -= 48;
	outdoors[0][0] = outdoors[1][0];
	outdoors[0][1] = outdoors[1][1];
	data_store4.outdoor_text[0][0] = data_store4.outdoor_text[1][0];
	data_store4.outdoor_text[0][1] = data_store4.outdoor_text[1][1];
	for (i = 0; i < 48; i++)
		for (j = 0; j < 96; j++)
			out_e[i][j] = out_e[i + 48][j];
	for (i = 48; i < 96; i++)
		for (j = 0; j < 96; j++)
			out_e[i][j] = 0;


	for (i = 0; i < 10; i++) {
		if (party.out_c[i].m_loc.x < 48)
			party.out_c[i].exists = FALSE;
		if (party.out_c[i].exists == TRUE)
			party.out_c[i].m_loc.x -= 48;
	}
	load_outdoors(party.outdoor_corner.x + 1, party.outdoor_corner.y, 1, 0, 0, 0, NULL);
	load_outdoors(party.outdoor_corner.x + 1, party.outdoor_corner.y + 1, 1, 1, 0, 0, NULL);
	build_outdoors();

	// reload graphics -- who knows what we added
	load_area_graphics();
	make_cursor_sword();

}

void shift_universe_up()
{
	short i, j;

	make_cursor_watch();
	save_outdoor_maps();
	party.outdoor_corner.y--;
	party.i_w_c.y++;
	party.p_loc.y += 48;
	outdoors[0][1] = outdoors[0][0];
	outdoors[1][1] = outdoors[1][0];

	data_store4.outdoor_text[0][1] = data_store4.outdoor_text[0][0];
	data_store4.outdoor_text[1][1] = data_store4.outdoor_text[1][0];
	for (i = 0; i < 96; i++)
		for (j = 48; j < 96; j++)
			out_e[i][j] = out_e[i][j - 48];
	for (i = 0; i < 96; i++)
		for (j = 0; j < 48; j++)
			out_e[i][j] = 0;

	for (i = 0; i < 10; i++) {
		if (party.out_c[i].m_loc.y > 48)
			party.out_c[i].exists = FALSE;
		if (party.out_c[i].exists == TRUE)
			party.out_c[i].m_loc.y += 48;
	}
	load_outdoors(party.outdoor_corner.x, party.outdoor_corner.y, 0, 0, 0, 0, NULL);
	load_outdoors(party.outdoor_corner.x + 1, party.outdoor_corner.y, 1, 0, 0, 0, NULL);

	build_outdoors();

	// reload graphics -- who knows what we added
	load_area_graphics();
	make_cursor_sword();

}

void shift_universe_down()
{
	short i, j;

	make_cursor_watch();

	save_outdoor_maps();
	party.outdoor_corner.y++;
	party.i_w_c.y--;
	party.p_loc.y = party.p_loc.y - 48;
	outdoors[0][0] = outdoors[0][1];
	outdoors[1][0] = outdoors[1][1];

	data_store4.outdoor_text[0][0] = data_store4.outdoor_text[0][1];
	data_store4.outdoor_text[1][0] = data_store4.outdoor_text[1][1];
	for (i = 0; i < 96; i++)
		for (j = 0; j < 48; j++)
			out_e[i][j] = out_e[i][j + 48];
	for (i = 0; i < 96; i++)
		for (j = 48; j < 96; j++)
			out_e[i][j] = 0;

	for (i = 0; i < 10; i++) {
		if (party.out_c[i].m_loc.y < 48)
			party.out_c[i].exists = FALSE;
		if (party.out_c[i].exists == TRUE)
			party.out_c[i].m_loc.y = party.out_c[i].m_loc.y - 48;
	}
	load_outdoors(party.outdoor_corner.x, party.outdoor_corner.y + 1, 0, 1, 0, 0, NULL);
	load_outdoors(party.outdoor_corner.x + 1, party.outdoor_corner.y + 1, 1, 1, 0, 0, NULL);

	build_outdoors();

	// reload graphics -- who knows what we added
	load_area_graphics();
	make_cursor_sword();

}

void position_party(const location& out, const location& pc_pos)
{
	short i, j;

	save_outdoor_maps();
	party.p_loc = pc_pos;
	party.loc_in_sec = global_to_local(party.p_loc);

	if ( party.outdoor_corner != out ) {
		party.outdoor_corner = out;
		load_outdoors(party.outdoor_corner.x + 1, party.outdoor_corner.y + 1, 1, 1, 0, 0, NULL);
		load_outdoors(party.outdoor_corner.x, party.outdoor_corner.y + 1, 0, 1, 0, 0, NULL);
		load_outdoors(party.outdoor_corner.x + 1, party.outdoor_corner.y, 1, 0, 0, 0, NULL);
		load_outdoors(party.outdoor_corner.x, party.outdoor_corner.y, 0, 0, 0, 0, NULL);
	}
	party.i_w_c.x = (party.p_loc.x > 47) ? 1 : 0;
	party.i_w_c.y = (party.p_loc.y > 47) ? 1 : 0;
	for (i = 0; i < 10; i++)
		party.out_c[i].exists = FALSE;
	for (i = 0; i < 96; i++)
		for (j = 0; j < 96; j++)
			out_e[i][j] = 0;
	build_outdoors();
}

void position_party(short out_x, short out_y, short pc_pos_x, short pc_pos_y)
{
	assert(std::in_range<signed char>(out_x));
	assert(std::in_range<signed char>(out_y));
	assert(std::in_range<signed char>(pc_pos_x));
	assert(std::in_range<signed char>(pc_pos_y));
	position_party(
		{ static_cast<char>(out_x), static_cast<char>(out_y) },
		{ static_cast<char>(pc_pos_x), static_cast<char>(pc_pos_y) }
	);
}

void build_outdoors()
{
	short i, j;
	for (i = 0; i < 48; i++)
		for (j = 0; j < 48; j++) {
			out[i][j] = outdoors[0][0].terrain[i][j];
			out[48 + i][j] = outdoors[1][0].terrain[i][j];
			out[i][48 + j] = outdoors[0][1].terrain[i][j];
			out[48 + i][48 + j] = outdoors[1][1].terrain[i][j];
		}

	fix_boats();
	add_outdoor_maps();
	make_out_trim();
	if (in_startup_mode == FALSE)
		erase_out_specials();

	for (i = 0; i < 10; i++)
		if (party.out_c[i].exists == TRUE)
			if ((party.out_c[i].m_loc.x < 0) || (party.out_c[i].m_loc.y < 0) ||
				(party.out_c[i].m_loc.x > 95) || (party.out_c[i].m_loc.y > 95))
				party.out_c[i].exists = FALSE;

}

short onm(char x_sector, char y_sector)
{
	short i;

	i = y_sector * scenario_out_width() + x_sector;
	return i;
}


// This adds the current outdoor map info to the saved outdoor map info
void save_outdoor_maps()
{
	short i, j;

	for (i = 0; i < 48; i++)
		for (j = 0; j < 48; j++) {
			if (out_e[i][j] > 0)
				o_maps.outdoor_maps[onm(party.outdoor_corner.x, party.outdoor_corner.y)][i / 8][j] =
				o_maps.outdoor_maps[onm(party.outdoor_corner.x, party.outdoor_corner.y)][i / 8][j] |
				(char)(s_pow(2, i % 8));
			if (party.outdoor_corner.x + 1 < scenario_out_width()) {
				if (out_e[i + 48][j] > 0)
					o_maps.outdoor_maps[onm(party.outdoor_corner.x + 1, party.outdoor_corner.y)][i / 8][j] =
					o_maps.outdoor_maps[onm(party.outdoor_corner.x + 1, party.outdoor_corner.y)][i / 8][j] |
					(char)(s_pow(2, i % 8));
			}
			if (party.outdoor_corner.y + 1 < scenario_out_height()) {
				if (out_e[i][j + 48] > 0)
					o_maps.outdoor_maps[onm(party.outdoor_corner.x, party.outdoor_corner.y + 1)][i / 8][j] =
					o_maps.outdoor_maps[onm(party.outdoor_corner.x, party.outdoor_corner.y + 1)][i / 8][j] |
					(char)(s_pow(2, i % 8));
			}
			if ((party.outdoor_corner.y + 1 < scenario_out_height()) &&
				(party.outdoor_corner.x + 1 < scenario_out_width())) {
				if (out_e[i + 48][j + 48] > 0)
					o_maps.outdoor_maps[onm(party.outdoor_corner.x + 1, party.outdoor_corner.y + 1)][i / 8][j] =
					o_maps.outdoor_maps[onm(party.outdoor_corner.x + 1, party.outdoor_corner.y + 1)][i / 8][j] |
					(char)(s_pow(2, i % 8));
			}
		}
}

void add_outdoor_maps()  // This takes the existing outdoor map info and supplements it
// with the saved map info
{
	short i, j;

	for (i = 0; i < 48; i++)
		for (j = 0; j < 48; j++) {
			if ((out_e[i][j] == 0) &&
				((o_maps.outdoor_maps[onm(party.outdoor_corner.x, party.outdoor_corner.y)][i / 8][j] &
					(char)(s_pow(2, i % 8))) != 0))
				out_e[i][j] = 1;
			if (party.outdoor_corner.x + 1 < scenario_out_width()) {
				if ((out_e[i + 48][j] == 0) &&
					((o_maps.outdoor_maps[onm(party.outdoor_corner.x + 1, party.outdoor_corner.y)][i / 8][j] &
						(char)(s_pow(2, i % 8))) != 0))
					out_e[i + 48][j] = 1;
			}
			if (party.outdoor_corner.y + 1 < scenario_out_height()) {
				if ((out_e[i][j + 48] == 0) &&
					((o_maps.outdoor_maps[onm(party.outdoor_corner.x, party.outdoor_corner.y + 1)][i / 8][j] &
						(char)(s_pow(2, i % 8))) != 0))
					out_e[i][j + 48] = 1;
			}
			if ((party.outdoor_corner.y + 1 < scenario_out_height()) &&
				(party.outdoor_corner.x + 1 < scenario_out_width())) {
				if ((out_e[i + 48][j + 48] == 0) &&
					((o_maps.outdoor_maps[onm(party.outdoor_corner.x + 1, party.outdoor_corner.y + 1)][i / 8][j] &
						(char)(s_pow(2, i % 8))) != 0))
					out_e[i + 48][j + 48] = 1;
			}
		}
}



void fix_boats()
{
	short i;

	for (i = 0; i < 30; i++)
		if ((party.boats[i].exists == TRUE) && (party.boats[i].which_town == 200)) {
			if (party.boats[i].boat_sector.x == party.outdoor_corner.x)
				party.boats[i].boat_loc.x = party.boats[i].boat_loc_in_sec.x;
			else if (party.boats[i].boat_sector.x == party.outdoor_corner.x + 1)
				party.boats[i].boat_loc.x = party.boats[i].boat_loc_in_sec.x + 48;
			else party.boats[i].boat_loc.x = 500;
			if (party.boats[i].boat_sector.y == party.outdoor_corner.y)
				party.boats[i].boat_loc.y = party.boats[i].boat_loc_in_sec.y;
			else if (party.boats[i].boat_sector.y == party.outdoor_corner.y + 1)
				party.boats[i].boat_loc.y = party.boats[i].boat_loc_in_sec.y + 48;
			else party.boats[i].boat_loc.y = 500;
		}
	for (i = 0; i < 30; i++)
		if ((party.horses[i].exists == TRUE) && (party.horses[i].which_town == 200)) {
			if (party.horses[i].horse_sector.x == party.outdoor_corner.x)
				party.horses[i].horse_loc.x = party.horses[i].horse_loc_in_sec.x;
			else if (party.horses[i].horse_sector.x == party.outdoor_corner.x + 1)
				party.horses[i].horse_loc.x = party.horses[i].horse_loc_in_sec.x + 48;
			else party.horses[i].horse_loc.x = 500;
			if (party.horses[i].horse_sector.y == party.outdoor_corner.y)
				party.horses[i].horse_loc.y = party.horses[i].horse_loc_in_sec.y;
			else if (party.horses[i].horse_sector.y == party.outdoor_corner.y + 1)
				party.horses[i].horse_loc.y = party.horses[i].horse_loc_in_sec.y + 48;
			else party.horses[i].horse_loc.y = 500;
		}
}


void load_outdoors(short to_create_x, short to_create_y, short targ_x, short targ_y,
	short mode, short extra, char* str)
	//short	to_create_x, to_create_y; // actual sector being loaded
//short 	targ_x, targ_y; // 0 or 1
{
	std::ifstream file_id;
	bool success = false;
	short i, j, out_sec_num;
	char file_name[256];
	long len_to_jump = 0, store = 0;

	if ((to_create_x != boe_clamp(to_create_x, 0, scenario_out_width() - 1)) ||
		(to_create_y != boe_clamp(to_create_y, 0, scenario_out_height() - 1))) { // not exist
		for (i = 0; i < 48; i++)
			for (j = 0; j < 48; j++)
				outdoors[targ_x][targ_y].terrain[i][j] = 5;
		for (i = 0; i < 18; i++) {
			outdoors[targ_x][targ_y].special_id[i] = -1;
			outdoors[targ_x][targ_y].special_locs[i].x = 100;
		}
		return;
	}

	build_scen_file_name(file_name);
	// Was: OpenFile(file_name, &store_str, OF_READ /* | OF_SEARCH */);
	file_id.open(file_name, std::ios_base::binary);
	if (file_id.fail()) {
		outdoor_alert();
		PostQuitMessage(0);
	}

	out_sec_num = scenario_out_width() * to_create_y + to_create_x;

	len_to_jump = sizeof(scenario_data_type);
	len_to_jump += sizeof(scen_item_data_type);
	for (i = 0; i < 300; i++)
		len_to_jump += (long)scenario.scen_str_len[i];
	store = 0;
	for (i = 0; i < out_sec_num; i++)
		for (j = 0; j < 2; j++)
			store += (long)(scenario.out_data_size[i][j]);
	len_to_jump += store;

	if (!SetFPos(file_id, len_to_jump, std::ios_base::beg)) { FSClose(file_id); oops_error(32); }

	if (mode == 0) {
		success = file_read_type(file_id, outdoors[targ_x][targ_y]);
		if (cur_scen_is_win != TRUE)
		{
			endian_adjust(outdoors[targ_x][targ_y]);
		}
		if (!success) { FSClose(file_id); oops_error(33); }
	}
	else success = file_read_type(file_id, dummy_out);

	if (mode == 0) {
		for (i = 0; i < 9; i++) {
			file_read_string(file_id, (long)outdoors[targ_x][targ_y].strlens[i], data_store4.outdoor_text[targ_x][targ_y].out_strs[i]);
		}
	}
	if (mode == 1) {
		for (i = 0; i < 120; i++) {
			const long len = (long)(dummy_out.strlens[i]);
			if (i == extra) {
				file_read_string(file_id, len, str);
			}
			else SetFPos(file_id, len, std::ios_base::cur);
		}

	}

	if (!FSClose(file_id)) { oops_error(33); }
}


void get_reg_data()
{
	std::ifstream f;
	short i;
	long vals[10];

	// Was: OpenFile("bladmisc.dat", &store, OF_READ /* | OF_SEARCH */);
	f.open("bladmisc.dat", std::ios_base::binary);
	if (f.fail()) {
		game_run_before = FALSE;
		build_data_file(1);
		// Was: OpenFile("bladmisc.dat", &store, OF_READ /* | OF_SEARCH */);
		f.open("bladmisc.dat", std::ios_base::binary);
		if (f.fail()) {
			reg_alert();
			register_flag = -1;
			return;
		}
		else {
			return;
		}
	}
	SetFPos(f, 0, std::ios_base::beg);

	for (i = 0; i < 10; i++) {
		static_assert(sizeof(vals[i]) == 4);
		file_read_type(f, vals[i]);
		if (i == 2)
			give_intro_hint = vals[i];
		if (i == 3)
			play_sounds = (vals[i] == 0) ? FALSE : TRUE;
		if (i == 4)
			display_mode = (short)vals[i];
		if ((display_mode < 0) || (display_mode > 5))
			display_mode = 0;
		if (i == 6)
			register_flag = vals[i];
		if (i == 8)
			ed_flag = vals[i];
	}
	if (vals[5] == 2000) {
		game_run_before = FALSE;
		build_data_file(1);
		return;
	}
	else {
		//if (vals[9] == init_data(vals[8]))
		//	ed_reg = TRUE;
		//if (vals[7] == init_data(vals[6]))
		//	registered = TRUE;
	}
}

void build_data_file(short mode)
//mode; // 0 - make first time file  1 - customize  2 - new write
{
	short i;
	long val_store, s_vals[10] = { 0,0,0,0,0, 0,0,0,0,0 };
	std::fstream f;

	// Was: OpenFile("bladmisc.dat", &store, OF_READWRITE /* | OF_SEARCH */);
	f.open("bladmisc.dat", std::ios_base::binary | std::ios_base::in | std::ios_base::out);
	if (f.fail())
		// Was: OpenFile("bladmisc.dat", &store, OF_WRITE | OF_CREATE /* | OF_SEARCH */);
		f.open("bladmisc.dat", std::ios_base::binary | std::ios_base::out);
	else {
		SetFPos(f, 0, std::ios_base::beg);
		for (i = 0; i < 10; i++)
		{
			static_assert(sizeof(s_vals[i]) == 4);
			file_read_type(f, s_vals[i]);
		}

		//format_to_buf(debug_str,"Starting {:d}: {:d}",i,(short) s_vals[i]);
		//add_string_to_buf(debug_str);
	}

	if (f.fail()) {
		reg_alert();
		register_flag = -1;
		ed_flag = -1;
		return;
	}
	SetFPos(f, 0, std::ios_base::beg);

	for (i = 0; i < 10; i++) {
		if (mode < 2)
			val_store = (long)(rand_short(20000, 30000));
		else val_store = s_vals[i];
		switch (i) {
		case 2: // tip of day
			if (mode < 2)
				val_store = 1;
			else val_store = give_intro_hint;
			break;
		case 3: // sounds
			if (mode < 2)
				val_store = 1;
			else if (play_sounds == TRUE)
				val_store = 1;
			else val_store = 0;
			break;
		case 4:  // display_mode
			if (mode < 2)
				val_store = 0;
			else val_store = display_mode;
			break;
		case 5:
			val_store = (mode == 0) ? 2000 : 1000;
			break;
		case 6:
			if (mode == 2)
				val_store = s_vals[i];
			register_flag = val_store;
			break;
		case 7:
			if ((mode == 2) && (registered == TRUE))
				val_store = init_data(register_flag);
			else val_store = val_store + 50000;
			break;
		case 8:
			if (mode < 2)
				val_store = (long)(rand_short(1000, 5000));
			else val_store = s_vals[i];
			ed_flag = val_store;
			break;
		case 9:
			if ((mode == 2) && (ed_reg == TRUE))
				val_store = init_data(ed_flag);
			else val_store = val_store + 50000;
			break;
		}
		static_assert(sizeof(val_store) == 4);
		file_write_type(f, val_store);
	}
}

// expecting party record to contain name of proper scenario to load
Boolean load_scenario()
{

	short i;
	std::ifstream file_id;
	Boolean file_ok = FALSE;
	char file_name[256];
	build_scen_file_name(file_name);

	// Was: OpenFile(file_name, &store, OF_READ /* | OF_SEARCH */);
	file_id.open(file_name, std::ios_base::binary);
	if (file_id.fail()) {
		oops_error(10000);
		SysBeep(2);	return FALSE;
	}

	if (!file_read_type(file_id, scenario)) {
		oops_error(29); return FALSE;
	}
	if ((scenario.flag1 == 10) && (scenario.flag2 == 20)
		&& (scenario.flag3 == 30)
		&& (scenario.flag4 == 40)) {
		file_ok = TRUE;
		cur_scen_is_win = FALSE;
		if (cur_scen_is_win != TRUE)
		{
			endian_adjust(scenario);
		}
	}
	if ((scenario.flag1 == 20) && (scenario.flag2 == 40)
		&& (scenario.flag3 == 60)
		&& (scenario.flag4 == 80)) {
		file_ok = TRUE;
		cur_scen_is_win = TRUE;
	}
	if (file_ok == FALSE) {
		give_error("This is not a legitimate Blades of Exile scenario.", "", 0);
		return FALSE;
	}
	if (!file_read_type(file_id, data_store2.scen_item_list)) {
		oops_error(30); return FALSE;
	}
	if (cur_scen_is_win != TRUE)
	{
		endian_adjust(data_store2.scen_item_list);
	}
	for (i = 0; i < 270; i++) {
		const long len = (long)(scenario.scen_str_len[i]);
		if (i < 160) {
			file_read_string(file_id, len, data_store5.scen_strs[i]);
		}
		else {
			file_read_string(file_id, len, scen_strs2[i - 160]);
		}
	}

	load_spec_graphics();
	set_up_ter_pics();
	return TRUE;
}

void set_up_ter_pics()
{
	short i;

	set_terrain_blocked();
	for (i = 0; i < 256; i++)
		terrain_pic[i] = scenario_ter_type(i).picture;
}
void oops_error(short error)
{
	char error_str[256];

	SysBeep(50);
	SysBeep(50);
	SysBeep(50);
	format_to_buf(error_str, "Giving the scenario editor more memory might also help. Be sure to back your scenario up often. Error number: {:d}.", error);
	give_error("The program encountered an error while loading/saving/creating the scenario. To prevent future problems, the program will now terminate. Trying again may solve the problem.", (char*)error_str, 0);
	//ExitToShell();
}

/*

typedef struct {
	unsigned char flag1, flag2, flag3, flag4;
	unsigned char ver[3],min_run_ver,prog_make_ver[3],num_towns;
	unsigned char out_width,out_height,difficulty,intro_pic,default_ground;
	} scen_header_type;
*/
void build_scen_headers()
{
	short i;
	short cur_entry = 0;
	HWND listbox;
	WORD count;
	char filename[256], filename2[256];

	for (i = 0; i < 25; i++)
		scen_headers[i].flag1 = 0;
	listbox = CreateWindow("listbox", NULL,
		WS_CHILDWINDOW, 0, 0, 0, 0,
		mainPtr, reinterpret_cast<HMENU>(1), reinterpret_cast<HINSTANCE>(GetWindowLongPtr(mainPtr, GWLP_HINSTANCE)), NULL);
	SendMessage(listbox, LB_DIR, 0x0, reinterpret_cast<LPARAM>("BLADSCEN/*.EXS"));
	count = (WORD)SendMessage(listbox, LB_GETCOUNT, 0, 0L);

	count = min(count, 20);

	for (i = 0; i < count; i++) {
		SendMessage(listbox, LB_GETTEXT, i, reinterpret_cast<LPARAM>(filename2));
		format_to_buf(filename, "BLADSCEN/{}", filename2);

		if (load_scenario_header(filename, cur_entry) == TRUE) {
			// now we need to store the file name, first stripping any path that occurs
			// before it
			strcpy(data_store2.scen_names[cur_entry], filename2);
			cur_entry++;
		}
	}

	DestroyWindow(listbox);
}

// This is only called at startup, when bringing headers of active scenarios.
// This wipes out the scenario record, so be sure not to call it while in an active scenario.
Boolean load_scenario_header(char* filename, short header_entry)
{

	short i;
	std::ifstream file_id;
	short store;
	Boolean file_ok = FALSE;
	char load_str[256];
	Boolean mac_header = TRUE;

	// Was: OpenFile(filename, &store_str, OF_READ /* | OF_SEARCH */);
	file_id.open(filename, std::ios_base::binary);
	if (file_id.fail()) {
		ASB(filename);
		return FALSE;
	}

	if (!file_read_type(file_id, scen_headers[header_entry])) {
		return FALSE;
	}
	if ((scen_headers[header_entry].flag1 == 10) && (scen_headers[header_entry].flag2 == 20)
		&& (scen_headers[header_entry].flag3 == 30)
		&& (scen_headers[header_entry].flag4 == 40)) {
		file_ok = TRUE;
		mac_header = TRUE;
	}
	if ((scen_headers[header_entry].flag1 == 20) && (scen_headers[header_entry].flag2 == 40)
		&& (scen_headers[header_entry].flag3 == 60)
		&& (scen_headers[header_entry].flag4 == 80)) {
		file_ok = TRUE;
		mac_header = FALSE;
	}
	if (file_ok == FALSE) {
		scen_headers[header_entry].flag1 = 0;
		return FALSE;
	}

	// So file is OK, so load in string data and close it.
	SetFPos(file_id, 0, std::ios_base::beg);
	if (!file_read_type(file_id, scenario)) {
		oops_error(29); return FALSE;
	}
	store = scenario.rating;
	if (mac_header == TRUE)
		endian_adjust(store);
	scen_headers[header_entry].default_ground = store;

	if (!SetFPos(file_id, sizeof(scen_item_data_type), std::ios_base::cur))
		return FALSE;

	for (i = 0; i < 3; i++) {
		file_read_string(file_id, (long)(short)scenario.scen_str_len[i], load_str);
		if (i == 0)
			load_str[29] = 0;
		else load_str[59] = 0;
		strcpy(data_store2.scen_header_strs[header_entry][i], load_str);
	}

	return TRUE;
}

//extern GWorldPtr spec_scen_g;
void load_spec_graphics()
{
	short i;
	char file_name[256];

	if (spec_scen_g != NULL) {
		DeleteObject(spec_scen_g);
		spec_scen_g = NULL;
	}
	//build_scen_file_name(file_name);
	format_to_buf(file_name, "bladscen/{}", party.scen_name);
	for (i = 0; i < 256; i++) {
		if (file_name[i] == '.') {
			file_name[i + 1] = 'b';
			file_name[i + 2] = 'm';
			file_name[i + 3] = 'p';
			i = 256;
		}
	}
	spec_scen_g = ReadDib(file_name, main_dc);
}


short init_data(short flag)
{
	long k = 0;

	k = (long)flag;
	k = k * k;
	jl = jl * jl + 84 + k;
	k = k + 51;
	jl = jl * 2 + 1234 + k;
	k = k % 3000;
	jl = jl * 54;
	jl = jl * 2 + 1234 + k;
	k = k * 82;
	k = k % 10000;
	k = k + 10000;

	return (short)k;
}

short town_s(short flag)
{
	long k = 0;

	k = (long)flag;
	k = k * k * k;
	jl = jl * 54;
	jl = jl * 2 + 1234 + k;
	k = k + 51;
	k = k % 3000;
	jl = jl * 2 + 1234 + k;
	k = k * scenario_num_towns();
	k = k % 10000;
	jl = jl * jl + 84 + k;
	k = k + 10000;

	return (short)k;
}

short out_s(short flag)
{
	long k = 0;

	k = (long)flag;
	k = k * k * k;
	jl = jl * jl + 84 + k;
	k = k + scenario.out_data_size[0][1];
	k = k % 3000;
	k = k * 4;
	jl = jl * 2 + 1234 + k;
	jl = jl * 54;
	jl = jl * 2 + 1234 + k;
	k = k % 10000;
	k = k + 4;

	return (short)k;
}

static short str_size_1(short flag)
{
	long k = 0;

	k = (long)flag;
	k = k * k;
	jl = jl * 2 + 1234 + k;
	jl = jl * jl + 84 + k;
	k = k + scenario.scen_str_len[0] + scenario.scen_str_len[1] + scenario.scen_str_len[2];
	jl = jl * 2 + 1234 + k;
	k = k % 3000;
	jl = jl * 54;
	jl = jl * jl + 84 + k;
	k = k * 4;
	k = k % 5000;
	k = k - 9099;

	return (short)k;
}

static short str_size_2(short flag)
{
	long k = 0;

	k = (long)flag;
	jl = jl * jl + 84 + k;
	k = k * k * k * k;
	jl = jl * 54;
	k = k + 80;
	k = k % 3000;
	jl = jl * 2 + 1234 + k;
	k = k * scenario_out_width() * scenario_out_height();
	jl = jl * jl + 84 + k;
	k = k % 3124;
	k = k - 5426;

	return (short)k;
}

static short str_size_3(short flag)
{
	long k = 0;

	k = (long)flag;
	k = k * (scenario.town_data_size[0][0] + scenario.town_data_size[0][1] + scenario.town_data_size[0][2] + scenario.town_data_size[0][3]);
	k = k + 80;
	jl = jl * jl + 84 + k;
	k = k % 3000;
	jl = jl * 2 + 1234 + k;
	k = k * 45;
	jl = jl * 54;
	jl = jl * jl + 84 + k;
	k = k % 887;
	k = k + 9452;

	return (short)k;
}

short get_buf_ptr(short flag)
{
	long k = 0;

	k = (long)flag;
	jl = jl * jl + 84 + k;
	k = k * (scenario_out_width() + scenario_out_width() + scenario_out_height() + scenario.town_data_size[0][3]);
	k = k + 80;
	jl = jl * jl + 84 + k;
	k = k % 2443;
	jl = jl * 54;
	k = k * 322;
	jl = jl * 2 + 1234 + k;
	k = k % 2542;
	jl = jl * jl + 84 + k;
	k = k + 234;

	return (short)k;
}

Boolean check_p(short pword)
{
	if (scenario.flag_b != town_s(pword))
		return FALSE;
	if (scenario.flag_c != out_s(pword))
		return FALSE;
	if (scenario.flag_e != str_size_1(pword))
		return FALSE;
	if (scenario.flag_f != str_size_2(pword))
		return FALSE;
	if (scenario.flag_h != str_size_3(pword))
		return FALSE;
	if (scenario.flag_d != init_data(pword))
		return FALSE;

	return TRUE;
}


void town_alert()
{
	MessageBox(mainPtr, "Cannot find or read file town.dat! Please make sure it is in the same folder as Exile III.",
		"File Error", MB_OK | MB_ICONEXCLAMATION);
	ExitToShell();
}
void outdoor_alert()
{
	MessageBox(mainPtr, "Cannot find or read file outdoor.dat! Please make sure it is in the same folder as Exile III.",
		"File Error", MB_OK | MB_ICONEXCLAMATION);
	ExitToShell();
}
void reg_alert()
{
	MessageBox(mainPtr, "Cannot create/read file misc.dat! This volume may be locked. Exile III can be still be played.",
		"File Error", MB_OK | MB_ICONEXCLAMATION);
}

//	MessageBox(mainPtr,"A","Debug note",MB_OK | MB_ICONEXCLAMATION);
Boolean load_blades_data()
{
	std::ifstream file_id;
	char file_name[256];

	build_scen_ed_name(file_name);

	// Was: OpenFile(file_name, &store, OF_READ /* | OF_SEARCH */);
	file_id.open(file_name, std::ios_base::binary);
	if (file_id.fail()) {
		return FALSE;
	}
	return TRUE;
}
