void draw_one_terrain_spot (short i,short j,short terrain_to_draw,short dest);
void draw_monsters();
void play_see_monster_str(unsigned char m);
void draw_pcs(location center,short mode);
void draw_items();
void draw_outd_boats(location center);
void draw_town_boat(location center) ;
void draw_fields();
void draw_spec_items();
void draw_sfx();
void draw_one_field(unsigned char flag,short source_x,short short_y);
void draw_one_spec_item(unsigned char flag,short source_x,short short_y);
void draw_party_symbol(short mode,location center);
RECT get_terrain_template_rect (unsigned char type_wanted);
RECT return_item_rect(short wanted);
RECT get_monster_rect (unsigned char type_wanted,short mode) ;
RECT get_monster_template_rect (unsigned char type_wanted,short mode,short which_part) ;
RECT get_item_template_rect (short type_wanted);
unsigned char get_t_t(char x,char y);  // returns terrain type at where
bool is_wall(unsigned char ter_type);
bool is_not_wall(unsigned char ter_type);
bool is_ground(unsigned char ter_type);
void make_town_trim(short mode);
void make_out_trim();
char add_trim_to_array(location where,unsigned char ter_type);
Boolean pt_in_rect(location loc,BoeRect rect);
void check_if_monst_seen(unsigned char m_num);
void adjust_monst_menu();
void frame_space(location where,short mode,short width,short height);
