void start_shop_mode(short shop_type,short shop_min,short shop_max,short cost_adj,char *store_name);
void end_shop_mode();
void handle_shop_event(POINT p,Boolean right_button);
void handle_sale(short what_chosen,short cost);
void handle_info_request(short what_chosen);
void set_up_shop_array();
void start_talk_mode(short m_num,short personality,unsigned char monst_type,short store_face_pic);
void end_talk_mode();
void handle_talk_event(POINT p,Boolean right_button);
void handle_talk_spec(short ttype,char *place_string1,char *place_string2);
void store_responses();
void put_num_in_text(short num_to_put,HWND the_dialog,short item_num);
int get_text_item_num(short item_num,HWND the_dialog);
void do_sign(short town_num, short which_sign, short sign_type,location sign_loc);
void give_reg_info_event_filter (short item_hit);
void give_reg_info();
void do_registration_event_filter (short item_hit);
void do_registration();
void do_update_registration();
void do_talk(short personality, unsigned char m_num);
void edit_party_event_filter (short item_hit);
void put_party_stats();
void edit_party(short can_create,short can_cancel);
Boolean prefs_event_filter (short item_hit);
void pick_preferences()  ;


void sign_event_filter (short item_hit);
void tip_of_day_event_filter (short item_hit);
void tip_of_day();
void max_window(HWND window);

void put_scen_info();
void pick_a_scen_event_filter (short item_hit);
short pick_a_scen();
short pick_prefab_scen();
void pick_prefab_scen_event_filter (short item_hit);
void give_password_filter (short item_hit);
Boolean enter_password();
