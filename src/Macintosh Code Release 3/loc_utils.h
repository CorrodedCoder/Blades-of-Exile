void set_terrain_blocked();
short dist(location p1,location p2);
short ex_sqrt(short val);
short vdist(location p1,location p2) ;
Boolean adjacent(location p1,location p2);
Boolean point_onscreen(location center,location check);
short set_direction (location old_pt, location new_pt);
location global_to_local(location global);
location local_to_global(location local);
Boolean loc_off_world(location p1);
Boolean loc_off_act_area(location p1);
location get_cur_loc();
Boolean is_lava(short x,short y);
short can_see(location p1,location p2,short mode);
short get_obscurity(short x,short y);
unsigned char coord_to_ter(short x,short y);
Boolean is_container(location loc);
void update_explored(location dest);
bool is_blocked(location to_check);
bool is_not_blocked(location to_check);
Boolean monst_on_space(location loc,short m_num);
short monst_there(location where) ;
Boolean monst_can_be_there(location loc,short m_num);
Boolean monst_adjacent(location loc,short m_num);
Boolean monst_can_see(short m_num,location l);
Boolean party_can_see_monst(short m_num);
Boolean can_see_monst(location l,short m_num);
Boolean outd_is_blocked(location to_check);
Boolean special_which_blocks_monst(location to_check);
Boolean is_special(location to_check);
Boolean outd_is_special(location to_check);
bool impassable(unsigned char terrain_to_check);
short get_blockage(unsigned char terrain_type);
short light_radius();
Boolean pt_in_light(location from_where,location to_where) ;// Assumes, of course, in town or combat
Boolean combat_pt_in_light(location to_where);
Boolean party_sees_a_monst(); // Returns true is a hostile monster is in sight.
short party_can_see(location where);
location push_loc(location from_where,location to_where);
bool spot_impassable(short i,short  j);
void swap_ter(short i,short j,unsigned char ter1,unsigned char ter2);
void alter_space(short i,short j,unsigned char ter);
