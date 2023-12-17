#pragma once

const auto NUM_TOWN_ITEMS = 115;

typedef char Boolean;
static_assert(sizeof(Boolean) == 1);

typedef struct BoeRect
{
	short left;
	short top;
	short right;
	short bottom;
} BoeRect;
static_assert(sizeof(BoeRect) == 8);

typedef struct {
	char x, y;
} location;
static_assert(sizeof(location) == 2);

typedef struct {
	short type, sd1, sd2, pic, m1, m2, ex1a, ex1b, ex2a, ex2b, jumpto;
} special_node_type;
static_assert(sizeof(special_node_type) == 22);

typedef struct {
	short personality, type;
	char link1[4], link2[4];
	short extras[4];
} talking_node_type;
static_assert(sizeof(talking_node_type) == 20);

typedef struct {
	unsigned char strlens[200];
	talking_node_type talk_nodes[60];
} talking_record_type;
static_assert(sizeof(talking_record_type) == 1400);

typedef struct {
	short picture;
	unsigned char blockage, flag1, flag2, special, trans_to_what, fly_over, boat_over;
	unsigned char block_horse, light_radius, step_sound, shortcut_key, res1, res2, res3;
} terrain_type_type;
static_assert(sizeof(terrain_type_type) == 16);

typedef	struct {
	unsigned char monst[4];
} wandering_type;
static_assert(sizeof(wandering_type) == 4);

typedef struct {
	unsigned char monst[7];
	unsigned char friendly[3];
	short spec_on_meet, spec_on_win, spec_on_flee, cant_flee;
	short end_spec1, end_spec2;
} out_wandering_type;
static_assert(sizeof(out_wandering_type) == 22);

typedef struct {
	unsigned char	terrain[48][48];
	location	special_locs[18];
	unsigned char		special_id[18];
	location	exit_locs[8];
	char		exit_dests[8];
	location	sign_locs[8];
	out_wandering_type	wandering[4], special_enc[4];
	location	wandering_locs[4];
	BoeRect info_rect[8];
	unsigned char strlens[180];
	special_node_type specials[60];
} outdoor_record_type;
static_assert(sizeof(outdoor_record_type) == 4146);

typedef struct {
	unsigned char number;
	unsigned char start_attitude;
	location start_loc;
	unsigned char  mobile;
	unsigned char  time_flag;
	unsigned char  extra1, extra2;
	short spec1, spec2;
	char spec_enc_code, time_code;
	short monster_time, personality;
	short special_on_kill, facial_pic;
} creature_start_type;
static_assert(sizeof(creature_start_type) == 22);

typedef struct {
	short variety, item_level;
	char awkward, bonus, protection, charges, type;
	unsigned char graphic_num, ability, type_flag, is_special;
	short value;
	Boolean identified, magic;
	unsigned char weight, description_flag;
	char full_name[25], name[15];
	unsigned char reserved1, reserved2;
	unsigned char magic_use_type, ability_strength, treas_class, real_abil;
} short_item_record_type;
static_assert(sizeof(short_item_record_type) == 66);

typedef struct {
	short variety, item_level;
	char awkward, bonus, protection, charges, type, magic_use_type;
	unsigned char graphic_num, ability, ability_strength, type_flag, is_special, a;
	short value;
	unsigned char weight, special_class;
	location item_loc;
	char full_name[25], name[15];
	unsigned char treas_class, item_properties, reserved1, reserved2;
} item_record_type;
static_assert(sizeof(item_record_type) == 66);

typedef struct {
	location item_loc;
	short item_code, ability;
	unsigned char charges, always_there, property, contained;
} preset_item_type;
static_assert(sizeof(preset_item_type) == 10);

typedef struct {
	location field_loc;
	short field_type;
} preset_field_type;
static_assert(sizeof(preset_field_type) == 4);

typedef struct {
	short town_chop_time, town_chop_key;
	wandering_type	wandering[4];
	location	wandering_locs[4];
	location	special_locs[50];
	unsigned char		spec_id[50];
	location	sign_locs[15];
	short		lighting;
	location start_locs[4];
	location exit_locs[4];
	short exit_specs[4];
	BoeRect in_town_rect;
	preset_item_type preset_items[64];
	short max_num_monst;
	preset_field_type preset_fields[50];
	short spec_on_entry, spec_on_entry_if_dead;
	short timer_spec_times[8];
	short timer_specs[8];
	unsigned char strlens[180];
	special_node_type specials[100];
	unsigned char specials1, specials2, res1, res2;
	short difficulty;
} town_record_type;
static_assert(sizeof(town_record_type) == 3506);

typedef struct {
	unsigned char terrain[64][64];
	BoeRect room_rect[16];
	creature_start_type creatures[60];
	unsigned char lighting[8][64];
} big_tr_type;
static_assert(sizeof(big_tr_type) == 6056);

typedef struct {
	unsigned char terrain[48][48];
	BoeRect room_rect[16];
	creature_start_type creatures[40];
	unsigned char lighting[6][48];
} ave_tr_type;
static_assert(sizeof(ave_tr_type) == 3600);

typedef struct {
	unsigned char terrain[32][32];
	BoeRect room_rect[16];
	creature_start_type creatures[30];
	unsigned char lighting[4][32];
} tiny_tr_type;
static_assert(sizeof(tiny_tr_type) == 1940);

typedef struct {
	short block_type;
	short block_destroy_time;
	char block_alignment;
	char block_key_time;
	location block_loc;
} city_block_type;
static_assert(sizeof(city_block_type) == 8);

typedef struct {
	BoeRect what_rect;
	unsigned char ter_type;
	unsigned char hollow;
} city_ter_rect_type;
static_assert(sizeof(city_ter_rect_type) == 10);

typedef struct {
	creature_start_type creatures[30];
	city_block_type city_block[15];
	city_ter_rect_type city_ter_rect[10];
} template_town_type;
static_assert(sizeof(template_town_type) == 880);

typedef struct {
	item_record_type scen_items[400];
	char monst_names[256][20];
	char ter_names[256][30];
} scen_item_data_type;
static_assert(sizeof(scen_item_data_type) == 39200);


typedef struct {
	short ter_type, item_num[10], item_odds[10], property;
} item_storage_shortcut_type;
static_assert(sizeof(item_storage_shortcut_type) == 44);

typedef struct {
	unsigned char m_num, level, m_name[26];
	short health, m_health, mp, max_mp;
	unsigned char armor, skill;
	short a[3];
	unsigned char a1_type, a23_type, m_type, speed, ap, mu, cl, breath, breath_type, treasure, spec_skill, poison;
	short morale, m_morale;
	short corpse_item, corpse_item_chance;
	short status[15];
	unsigned char direction, immunities, x_width, y_width, radiate_1, radiate_2;
	unsigned char default_attitude, summon_type, default_facial_pic, res1, res2, res3;
	short picture_num;
} monster_record_type;
static_assert(sizeof(monster_record_type) == 108);

typedef struct {
	location horse_loc, horse_loc_in_sec, horse_sector;
	short which_town;
	Boolean exists, property;
} horse_record_type;
static_assert(sizeof(horse_record_type) == 10);

typedef struct {
	location boat_loc, boat_loc_in_sec, boat_sector;
	short which_town;
	Boolean exists, property;
} boat_record_type;
static_assert(sizeof(boat_record_type) == 10);

typedef struct {
	unsigned char flag1, flag2, flag3, flag4;
	unsigned char ver[3], min_run_ver, prog_make_ver[3], num_towns;
	unsigned char out_width, out_height, difficulty, intro_pic, default_ground;
} scen_header_type;
static_assert(sizeof(scen_header_type) == 17);

typedef struct {
	unsigned char flag1, flag2, flag3, flag4;
	unsigned char ver[3], min_run_ver, prog_make_ver[3], num_towns;
	unsigned char out_width, out_height, difficulty, intro_pic, default_ground;
	unsigned char town_size[200];
	unsigned char town_hidden[200], a;
	short flag_a;
	short intro_mess_pic, intro_mess_len;
	location where_start, out_sec_start, out_start;
	short which_town_start;
	short flag_b;
	short town_data_size[200][5];
	short town_to_add_to[10];
	short flag_to_add_to_town[10][2];
	short flag_c;
	short out_data_size[100][2];
	BoeRect store_item_rects[3];
	short store_item_towns[3];
	short flag_e;
	short special_items[50];
	short special_item_special[50];
	short rating, uses_custom_graphics;
	short flag_f;
	monster_record_type scen_monsters[256];
	boat_record_type scen_boats[30];
	horse_record_type scen_horses[30];
	short flag_g;
	terrain_type_type ter_types[256];
	short scenario_timer_times[20];
	short scenario_timer_specs[20];
	short flag_h;
	special_node_type scen_specials[256];
	item_storage_shortcut_type storage_shortcuts[10];
	short flag_d;
	unsigned char scen_str_len[300];
	short flag_i;
	location last_out_edited;
	short last_town_edited;

} scenario_data_type;
static_assert(sizeof(scenario_data_type) == 41942);

// Game save file related structures
enum class flag_type : short {
	out = 5790,
	town = 1342,
	in_scenario = 100,
	not_in_scenario = 200,
	no_maps = 3422,
	have_maps = 5567
};
static_assert(sizeof(flag_type) == 2);

// for game
typedef struct {
	short personality;
	short town_num;
	short str1, str2;
} talk_save_type;
static_assert(sizeof(talk_save_type) == 8);

typedef struct {
	short active, attitude;
	unsigned char number;
	location m_loc;
	monster_record_type m_d;
	Boolean mobile;
	short summoned;
	creature_start_type monst_start;
} creature_data_type;
static_assert(sizeof(creature_data_type) == 142);

typedef struct {
	creature_data_type dudes[60];
	short which_town;
	short friendly;
} creature_list_type;
static_assert(sizeof(creature_list_type) == 8524);

typedef struct {
	short town_num, difficulty;
	town_record_type	town;
	char explored[64][64];
	Boolean	hostile;
	creature_list_type	monst;
	Boolean	in_boat;
	location p_loc;
}	current_town_type;
static_assert(sizeof(current_town_type) == 16136);

typedef struct {
	Boolean exists;
	short direction;
	out_wandering_type what_monst;
	location which_sector, m_loc;
} outdoor_creature_type;
static_assert(sizeof(outdoor_creature_type) == 30);

typedef struct {
	int age;
	short gold, food;
	unsigned char stuff_done[310][10], item_taken[200][8];
	short light_level;
	location outdoor_corner, i_w_c, p_loc, loc_in_sec;
	boat_record_type boats[30];
	horse_record_type horses[30];
	creature_list_type creature_save[4];
	short in_boat, in_horse;
	outdoor_creature_type out_c[10];
	item_record_type magic_store_items[5][10];
	short imprisoned_monst[4];
	char m_seen[256];
	char journal_str[50];
	short journal_day[50];
	short special_notes_str[140][2];
	talk_save_type talk_save[120];
	short direction, at_which_save_slot;
	char alchemy[20];
	Boolean can_find_town[200];
	short key_times[100];
	short party_event_timers[30];
	short global_or_town[30];
	short node_to_call[30];
	char spec_items[50], help_received[120];
	short m_killed[200];
	int total_m_killed, total_dam_done, total_xp_gained, total_dam_taken;
	char scen_name[256];
} party_record_type;
static_assert(sizeof(party_record_type) == 46400);

typedef struct {
	char town_maps[100][8][64];
} stored_town_maps_type;
static_assert(sizeof(stored_town_maps_type) == 51200);

typedef struct {
	short main_status;
	char name[20];
	short skills[30];
	short max_health, cur_health, max_sp, cur_sp, experience, skill_pts, level;
	short status[15];
	item_record_type items[24];
	Boolean equip[24];
	Boolean priest_spells[62], mage_spells[62];
	short which_graphic, weap_poisoned;
	Boolean advan[15], traits[15];
	short race, exp_adj, direction;
} pc_record_type;
static_assert(sizeof(pc_record_type) == 1898);

typedef struct {
	unsigned char setup[4][64][64];
} setup_save_type;
static_assert(sizeof(setup_save_type) == 16384);

typedef struct {
	item_record_type items[NUM_TOWN_ITEMS];
} town_item_list;
static_assert(sizeof(town_item_list) == 7590);

typedef struct {
	item_record_type items[NUM_TOWN_ITEMS];
} stored_items_list_type;
static_assert(sizeof(stored_items_list_type) == 7590);

typedef struct {
	char outdoor_maps[100][6][48];
} stored_outdoor_maps_type;
static_assert(sizeof(stored_outdoor_maps_type) == 28800);