void force_town_enter(short which_town,location where_start);
void start_town_mode(short which_town, short entry_dir);
location end_town_mode(short switching_level,location destination);  // returns new party location
void handle_leave_town_specials(short town_number, short which_spec,location start_loc) ;
void handle_town_specials(short town_number, short entry_dir,location start_loc) ;
Boolean abil_exists(short abil) ;

void start_town_combat(short direction);
short end_town_combat();
void place_party(short direction);
void create_town_combat_terrain();
void create_out_combat_terrain(short type,short num_walls,short spec_code);
void elim_monst(unsigned char which,short spec_a,short spec_b);
void do_shop(short which,short min,short max,char *store_name);
void buy_food(short cost,short per,char *food_name);
void healing_shop();
void do_sell(short which);
void dump_gold(short print_mes);
void pick_lock(location where,short pc_num);
void bash_door(location where,short pc_num);
void erase_specials();
void erase_out_specials();
short get_town_spec_id(location where);
void clear_map();
void draw_map (HWND the_dialog, short the_item);
Boolean is_door(location destination);
void display_map();
void check_done();
Boolean quadrant_legal(short i, short j) ;
