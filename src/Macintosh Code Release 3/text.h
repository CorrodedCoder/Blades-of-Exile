short text_pc_has_abil_equip(short pc_num,short abil);
void put_pc_screen();
void place_buy_button(short position,short pc_num,short item_num);
void put_item_screen(short screen_num,short suppress_buttons);
void place_item_bottom_buttons();
Rect get_party_template_rect(short pc_num,short mode);
void set_stat_window(short new_stat);
void place_item_button(short which_button_to_put,short which_slot,short which_button_position,short extra_val);
short first_active_pc();
void refresh_stat_areas(short mode);
short total_encumberance(short pc_num);
short get_tnl(pc_record_type *pc);
void draw_pc_effects(short pc);
void print_party_stats() ;
short do_look(location space);
short town_boat_there(location where);
short out_boat_there(location where);
short town_horse_there(location where);
short out_horse_there(location where);
void notify_out_combat_began(out_wandering_type encounter,short *nums) ;
void get_m_name(char *str,unsigned char num);
void get_ter_name(char *str,unsigned char num);
void print_monst_name(unsigned char m_type);
void print_monst_attacks(unsigned char m_type,short target);
void damaged_message(short damage,short type);
void print_monster_going(char *combat_str,unsigned char m_num,short ap);
void monst_spell_note(unsigned char number,short which_mess);
void monst_cast_spell_note(unsigned char number,short spell,short type);
void monst_breathe_note(unsigned char number);
void monst_damaged_mes(short which_m,short how_much,short how_much_spec);
void monst_killed_mes(short which_m);
void print_nums(short a,short b,short c);
short print_terrain(location space);
void add_string_to_buf(const char *string);
void init_buf();
void print_buf () ;
void restart_printing();
void restore_mode();
void through_sending();
void Display_String(Str255 str);
Rect coord_to_rect(short i,short j);
void make_cursor_sword() ;
void c2p(Str255 str) ;
void p2c(Str255 str);
void get_str(Str255 str,short i, short j);
short string_length(char *str);
void char_win_draw_string(GrafPtr dest_window,Rect dest_rect,const char *str,short mode,short line_height);
void win_draw_string(GrafPtr dest_window,Rect dest_rect,Str255 str,short mode,short line_height);
short calc_day();
Boolean day_reached(unsigned char which_day, unsigned char which_event);
void display_string(char *str);
void Draw_Some_Item (GWorldPtr src_gworld, Rect src_rect, GWorldPtr targ_gworld, location target, char masked, short main_win);
void rect_draw_some_item (GWorldPtr src_gworld, Rect src_rect,  GWorldPtr targ_gworld, Rect targ_rect,  char masked, short main_win);
Boolean is_ident(item_record_type item);
Boolean is_magic(item_record_type item);
Boolean is_contained(item_record_type item);
Boolean is_cursed(item_record_type item);
Boolean is_property(item_record_type item);
Rect get_custom_rect (short which_rect);

