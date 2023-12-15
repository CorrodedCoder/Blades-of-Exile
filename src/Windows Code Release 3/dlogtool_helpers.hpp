#pragma once

// Stopgap mechanism to reduce the amount of casting to known places only for now

static inline void draw_dialog_graphic_bmp(HBITMAP hBitmap, RECT rect, short which_g, Boolean do_frame)
{
	draw_dialog_graphic(hBitmap, rect, which_g, do_frame, 1);
}

static inline void draw_dialog_graphic_wnd(HWND hDlg, RECT rect, short which_g, Boolean do_frame)
{
	draw_dialog_graphic(hDlg, rect, which_g, do_frame, 0);
}
