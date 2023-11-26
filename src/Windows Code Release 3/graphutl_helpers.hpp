#pragma once

// Stopgap mechanism to reduce the amount of casting to known places only for now

static inline void rect_draw_some_item_bmp(HBITMAP src, RECT src_rect, HBITMAP dest, RECT dest_rect, short trans, short main_win)
{
	rect_draw_some_item(src, src_rect, dest, dest_rect, trans, main_win);
}

static inline void rect_draw_some_item_wnd(HBITMAP src,RECT src_rect,HWND dest,RECT dest_rect, short trans, short main_win)
{
	rect_draw_some_item(src, src_rect, reinterpret_cast<HBITMAP>(dest), dest_rect, trans, main_win);
}

static inline void rect_draw_some_item_dc(HBITMAP src,RECT src_rect,HDC dest,RECT dest_rect, short trans, short main_win)
{
	rect_draw_some_item(src, src_rect, reinterpret_cast<HBITMAP>(dest), dest_rect, trans, main_win);
}

static inline void rect_draw_some_item_either(HBITMAP src,RECT src_rect,short win_or_gworld, HWND hWnd, HDC hDC,RECT dest_rect, short trans, short main_win)
{
	if(win_or_gworld == 1)
	{
		rect_draw_some_item_wnd(src, src_rect, hWnd, dest_rect, trans, main_win);
	}
	else
	{
		rect_draw_some_item_dc(src, src_rect, hDC, dest_rect, trans, main_win);
	}
}
