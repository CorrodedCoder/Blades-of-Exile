void init_current_terrain();
void init_screen_locs();
Boolean handle_action(POINT the_point, UINT wparam, LONG lparam );
void flash_rect(RECT to_flash);
void swap_terrain();
void set_new_terrain(unsigned char selected_terrain);
Boolean handle_syskeystroke(UINT wParam,LONG lParam,short *handled);
Boolean handle_keystroke(UINT wParam,LONG lParam);
void get_wandering_monst();
void get_town_info();
void edit_monst(short mode,short x,short y);
void get_sign_resource();
void set_info_strings();
preset_item_type edit_item(preset_item_type item);
void modify_lists();
void set_up_lights();
void shy_change_circle_terrain(location center,short radius,unsigned char terrain_type,short probability);
void change_circle_terrain(location center,short radius,unsigned char terrain_type,short probability);
void change_rect_terrain(BoeRect r,unsigned char terrain_type,short probability,Boolean hollow);
void frill_up_terrain();
void unfrill_terrain();
void set_terrain(location l,unsigned char terrain_type);
Boolean fix_rubble(location l);
Boolean fix_cave(location l);
Boolean fix_mountain(location l);
Boolean fix_hill(location l);
Boolean fix_water(location l);
Boolean out_fix_rubble(location l);
Boolean out_fix_cave(location l);
Boolean out_fix_mountain(location l);
Boolean out_fix_hill(location l);
Boolean out_fix_water(location l);
void adjust_space(location l);
unsigned char coord_to_ter(short x,short y);
short get_obscurity(short x,short y);
short can_see(location p1,location p2,short mode);
Boolean place_item(location spot_hit,short which_item,short property,short always,short odds); 
void place_items_in_town();
void set_up_start_screen();
void set_up_main_screen();
void start_town_edit();
void start_out_edit();
void shut_down_menus(short mode);
void start_terrain_editing();
void start_monster_editing(short just_redo_text);
void start_item_editing(short just_redo_text);
void start_special_item_editing();
void start_string_editing(short mode,short just_redo_text);
void start_special_editing(short mode,short just_redo_text);
void town_entry(location spot_hit);
void start_dialogue_editing(short restoring);
Boolean monst_on_space(location loc,short m_num);
void place_edit_special(location loc);
void set_special(location spot_hit);
Boolean save_check(short which_dlog);
void update_item_menu();
