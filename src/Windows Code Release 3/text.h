#pragma once

#include <string_view>
#include <format>

short text_pc_has_abil_equip(short pc_num,short abil);
void put_pc_screen();
void put_item_screen(short screen_num,short suppress_buttons);
RECT get_party_template_rect(short pc_num,short mode);
void set_stat_window(short new_stat);
short first_active_pc();
void refresh_stat_areas(short mode);
void draw_pc_effects_dc(short pc,HDC dest_dc);
void print_party_stats() ;
short do_look(location space);
short town_boat_there(location where);
short out_boat_there(location where);
short town_horse_there(location where);
short out_horse_there(location where);
void notify_out_combat_began(out_wandering_type encounter,short *nums);
void print_monst_name(unsigned char m_type);
void print_monst_attacks(unsigned char m_type,short target);
void damaged_message(short damage,short type);
std::string format_monster_going(unsigned char m_num,short ap);
void monst_spell_note(unsigned char number,short which_mess);
void monst_cast_spell_note(unsigned char number,short spell,short type);
void monst_breathe_note(unsigned char number);
void monst_damaged_mes(short which_m,short how_much,short how_much_spec);
void monst_killed_mes(short which_m);
void print_nums(short a,short b,short c);
void add_string_to_buf(std::string_view str);
void init_buf();
void print_buf () ;
void through_sending();
RECT coord_to_rect(short i,short j);
constexpr void c2p(char* str) {};
void get_str(char *str,short i, short j);
short string_length(const char *str,HDC hdc);
void win_draw_string(HDC dest_window,RECT dest_rect, std::string_view str,short mode,short line_height);
short calc_day();
Boolean day_reached(unsigned char which_day, unsigned char which_event);
void Draw_Some_Item (HBITMAP src_gworld, RECT src_rect, HBITMAP targ_gworld, location target, char masked, short main_win);
void WinBlackDrawString(std::string_view str,short x,short y);
void WinDrawString(std::string_view str,short x,short y);
void FlushEvents(short mode);
void ExitToShell();
void ClipRect(RECT *rect);
void beep();
void SysBeep(short a);
void GetIndString(char *str,short i, short j) ;
void StringToNum(char *the_string,long *number_given);
void force_reprint();
void MeasureText(short str_len,char *str, short *len_array,HDC hdc);
void MoveTo(short x, short y);
void MoveToDrawString(const char *string,HDC hdc);
RECT get_custom_rect (short which_rect);

template <class... Types>
static inline void add_string_to_buf(const std::format_string<Types...> Fmt, Types&&... Args)
{
	add_string_to_buf(std::vformat(Fmt.get(), std::make_format_args(Args...)));
}
