Boolean give_to_pc(short pc_num,item_record_type  item, short print_result);
Boolean give_to_party(item_record_type item,short print_result);
void give_gold(short amount,Boolean print_result);
Boolean take_gold(short amount,Boolean print_result);
short pc_has_space(short pc_num);
void take_item(short pc_num,short which_item);
void fancy_choice_dialog_event_filter (short item_hit);
short fancy_choice_dialog(short which_dlog,short parent);
void select_pc_event_filter (short item_hit);
short char_select_pc(short active_only,short free_inv_only,char *title);
short select_pc(short active_only,short free_inv_only);
short choice_dialog(short pic,short num);
Boolean give_spec_items_event_filter (short item_hit);
void give_spec_items();
short party_total_level(); 
void display_traits_graphics();
void pick_race_abil_event_filter(short item_hit);
void pick_race_abil(pc_record_type *pc,short mode,short parent_num);
short get_tnl(pc_record_type *pc);
void reset_boats();
void combine_things(short pc_num);
