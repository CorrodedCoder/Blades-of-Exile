void cd_set_flag(short dlog_num,short item_num,short flag);
void cd_erase_rect(short dlog_num,RECT to_fry);
short cd_get_led_range(short dlog_num,short first_led,short last_led)    ;
void cd_hit_led_range(short dlog_num,short first_led,short last_led,short which_to_set);
void cd_set_led_range(short dlog_num,short first_led,short last_led,short which_to_set);
void cd_flip_led(short dlog_num,short item_num,short item_hit);                        
short cd_get_led(short dlog_num,short item_num);
void frame_di(HWND hDlg, short item_num, short val);
void cd_kill_dc(short which_slot,HDC hdc);
HDC cd_get_dlog_dc(short which_slot);
short cd_get_active(short dlog_num, short item_num);
short cd_create_dialog_parent_num(short dlog_num,short parent);
// Original code passed no parameter to cd_set_edit_focus
void cd_set_edit_focus(short which_dlg);
void cd_init_dialogs();
short cd_create_dialog(short dlog_num,HWND parent) ;
short cd_kill_dialog(short dlog_num,short parent_message);
short cd_process_click(HWND window,POINT the_point, UINT wparam, LONG lparam,short *item);
short cd_process_syskeystroke(HWND window,UINT wparam, LONG lparam,short *item);
short cd_process_keystroke(HWND window,UINT wparam, LONG lparam,short *item);
void cd_init_button(short dlog_num,short item_num, short button_num, short status);
void cd_attach_key(short dlog_num,short item_num,char key);
void cd_set_pict(short dlog_num, short item_num, short pict_num);
void cd_activate_item(short dlog_num, short item_num, short status);
void cd_get_item_text(short dlog_num, short item_num, char *str);
void cd_set_item_text(short dlog_num, short item_num, char *str);
void cd_set_item_num(short dlog_num, short item_num, short num);
void cd_set_led(short dlog_num,short item_num,short state);
void cd_text_frame(short dlog_num,short item_num,short frame);
void cd_add_label(short dlog_num, short item_num, char *label, short label_flag);
void cd_take_label(short dlog_num, short item_num);
void cd_key_label(short dlog_num, short item_num,short loc);
void cd_draw_item(short dlog_num,short item_num);
void cd_initial_draw(short dlog_num);
void cd_draw(short dlog_num);
void cd_redraw(HWND window);
void cd_frame_item(short dlog_num, short item_num, short width);
void cd_erase_item(short dlog_num, short item_num);
void cd_press_button(short dlog_num, short item_num);
short cd_get_indices(short dlg_num, short item_num, short *dlg_index, short *item_index);
short cd_get_dlg_index(short dlog_num);
short cd_find_dlog(HWND window, short *dlg_num, short *dlg_key);
short cd_get_item_id(short dlg_num, short item_num);
void center_window(HWND window);
RECT get_item_rect(HWND hDlg, short item_num);
void frame_dlog_rect(HWND hDlg, RECT rect, short val);
void draw_dialog_graphic(HWND hDlg, RECT rect, short which_g, Boolean do_frame,short win_or_gworld);

inline void draw_dialog_graphic_wnd(HWND hDlg, RECT rect, short which_g, Boolean do_frame)
{
	draw_dialog_graphic(hDlg, rect, which_g, do_frame, 0);
}

void showcursor(Boolean a);

void cd_get_text_edit_str(short dlog_num, char *str);
void cd_set_text_edit_str(short dlog_num, char *str);
void cdsin(short dlog_num, short item_num, short num); 
void csit(short dlog_num, short item_num, char *str);
void csp(short dlog_num, short item_num, short pict_num);
void ModalDialog();

RECT calc_rect(short i, short j);

void cd_retrieve_text_edit_str(short dlog_num, short item_num, char *str);
short cd_retrieve_text_edit_num(short dlog_num, short item_num) ;
void cd_set_text_edit_str(short dlog_num, short item_num, char *str) ;
void cd_set_text_edit_num(short dlog_num, short item_num, short num);
void InsetRect(RECT *rect,short x, short y);
