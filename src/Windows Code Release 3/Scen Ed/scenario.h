monster_record_type return_monster_template(unsigned char store);
item_record_type convert_item (short_item_record_type s_item) ;
void init_scenario();
short edit_ter_type(short which_ter);
void edit_ter_type_event_filter (short item_hit);
Boolean save_ter_info();
void put_ter_info_in_dlog();
void put_monst_info_in_dlog();
Boolean save_monst_info();
void edit_monst_type_event_filter (short item_hit);
short edit_monst_type(short which_monst);
void put_monst_abils_in_dlog();
Boolean save_monst_abils();
void edit_monst_abil_event_filter (short item_hit);
monster_record_type edit_monst_abil(monster_record_type starting_record,short parent_num);
void put_item_info_in_dlog();
Boolean save_item_info();
void edit_item_type_event_filter (short item_hit);
short edit_item_type(short which_item);
void put_item_abils_in_dlog();
Boolean save_item_abils();
void edit_item_abil_event_filter (short item_hit);
item_record_type edit_item_abil(item_record_type starting_record,short parent_num);
void edit_spec_item(short which_item);
void edit_spec_item_event_filter (short spec_item_hit);
Boolean save_spec_item();
void put_spec_item_in_dlog();
void put_save_rects_in_dlog();
Boolean save_save_rects();
void edit_save_rects_event_filter (short save_rects_hit);
void edit_save_rects();
void edit_horses();
void edit_horses_event_filter (short item_hit);
void put_horses_in_dlog();
Boolean save_horses();
Boolean save_add_town();
void put_add_town_in_dlog();
void edit_add_town_event_filter (short item_hit);
void edit_add_town();
void edit_item_placement();
void edit_item_placement_event_filter (short item_hit);
void put_item_placement_in_dlog();
Boolean save_item_placement();
void edit_scen_details_event_filter (short item_hit);
void edit_scen_details();
short edit_make_scen_2(short *val_array);
void edit_make_scen_2_event_filter (short item_hit);
void put_make_scen_2_in_dlog();
short edit_make_scen_1(char *filename,char *title,short *grass);
void edit_make_scen_1_event_filter (short item_hit);
void put_make_scen_1_in_dlog();
short get_password();
void give_password_filter (short item_hit);
void edit_scenario_events();
void edit_scenario_events_event_filter (short item_hit);
void put_scenario_events_in_dlog();
Boolean save_scenario_events();
short enter_password();
void build_scenario();
void set_starting_loc_filter (short item_hit);
void set_starting_loc();
void edit_boats_event_filter (short item_hit);
void edit_boats();

void user_password_filter (short item_hit);
short wd_to_pwd(char *str);
