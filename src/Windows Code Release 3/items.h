#pragma once

#include <span>
#include <string_view>

Boolean give_to_pc(short pc_num,const item_record_type& item,short  print_result);
Boolean give_to_party(const item_record_type& item,short print_result);
Boolean forced_give(short item_num,short abil);
Boolean GTP(short item_num);
Boolean silent_GTP(short item_num);
void give_gold(short amount,Boolean print_result);
Boolean take_gold(short amount,Boolean print_result);
Boolean party_take_abil(short abil);
short pc_carry_weight(const pc_record_type& pc);
void give_food(short amount,Boolean print_result);
short take_food(short amount,Boolean print_result);
short pc_ok_to_buy(short pc_num,short cost,const item_record_type& item);
void take_item(short pc_num,short which_item);
void remove_charge(short pc_num,short which_item);
void enchant_weapon(short pc_num,short item_hit,short enchant_type,short new_val);
void equip_item(short pc_num,short item_num);
void drop_item(short pc_num,short item_num,location where_drop);
Boolean place_item(const item_record_type& item,location where,Boolean forced);
void destroy_an_item();
void give_thing(short pc_num, short item_num);
void combine_things(short pc_num);
short dist_from_party(location where);
void set_item_flag(item_record_type *item);
short get_item(location place,short pc_num,bool check_container);
void put_item_graphics();

void make_town_hostile();
void display_item_event_filter (short item_hit);
void fancy_choice_dialog_event_filter (short item_hit);
short fancy_choice_dialog(short which_dlog,short parent);
void select_pc_event_filter (short item_hit);
short char_select_pc(short active_only,short free_inv_only,const char *title);
short select_pc(short active_only,short free_inv_only);
void get_num_of_items_event_filter (short item_hit);
short get_num_of_items(short max_num);
short choice_dialog(short pic,short num);
void create_modeless_dialog(short which_dlog);
void make_cursor_watch() ;
void place_glands(location where,unsigned char m_type);
void reset_item_max();
void place_treasure(location where,short level,short loot,short mode);
item_record_type return_treasure(short loot,short level,short mode);
void frame_button(RECT button_rect);
void refresh_store_items();
void get_text_response_event_filter (short item_hit);
void get_text_response(short dlg,char *str,short parent_num);
short custom_choice_dialog(const std::span<const std::string_view, 6>& strs,short pic_num,short buttons[3]) ;
short custom_choice_dialog(const std::array<std::array<char, 256>, 6>& strs, short pic_num, short buttons[3]);
Boolean party_check_class(short item_class,short mode);
