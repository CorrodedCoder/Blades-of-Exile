void file_initialize();
void load_file();
void save_file(short mode);
void change_rect_terrain(BoeRect r,unsigned char terrain_type,short probability,Boolean hollow);
void load_town(short town_num,short mode,short extra,char *str);
void shift_universe_left();
void shift_universe_right();
void shift_universe_up();
void shift_universe_down();
void position_party(short out_x,short out_y,short pc_pos_x,short pc_pos_y);
void position_party(const location& out, const location& pc_pos);
void build_outdoors();
void save_outdoor_maps();
void add_outdoor_maps();
void fix_boats();
void load_outdoors(short to_create_x, short to_create_y, short targ_x, short targ_y,
	short mode,short extra,char *str);
void get_reg_data();
long do_waterfall(long flag);
short init_data(short flag);
void start_data_dump();
void end_data_dump();
void set_terrain(location l, unsigned char terrain_type);
void oops_error(short error);
void set_up_ter_pics();
Boolean load_scenario();
short onm(char x_sector,char y_sector);
void build_scen_headers();
Boolean load_scenario_header(char *filename,short header_entry);
void load_spec_graphics();
Boolean check_p (short pword);
void build_scen_file_name (char *file_n);
short town_s(short flag);
short out_s(short flag);
short get_buf_ptr(short flag);
void outdoor_alert(),town_alert(),reg_alert();
void build_data_file(short mode);
void build_scen_ed_name (char *file_n);
Boolean load_blades_data();
