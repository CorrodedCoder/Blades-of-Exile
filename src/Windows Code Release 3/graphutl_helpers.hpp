#pragma once

#include <variant>

using DialogDrawDestination = std::variant<HWND, HBITMAP>;

// Stopgap mechanism to reduce the amount of casting to known places only for now

static inline void rect_draw_some_item_bmp(HBITMAP src, RECT src_rect, HBITMAP dest, RECT dest_rect, short trans, short main_win)
{
	rect_draw_some_item(src, src_rect, dest, dest_rect, trans, main_win);
}

static inline void rect_draw_some_item_dc(HBITMAP src,RECT src_rect,HDC dest,RECT dest_rect, short trans, short main_win)
{
	rect_draw_some_item(src, src_rect, dest, dest_rect, trans, main_win);
}

// which_mode is 0 ... dest is a bitmap
// is 1 ... ignore dest ... paint on mainPtr
// is 2 ... dest is a dialog, use the dialog pattern
// both pattern gworlds are 192 x 256
static inline void paint_pattern_bmp(HBITMAP dest, RECT dest_rect, short which_pattern)
{
	paint_pattern(dest, 0, dest_rect, which_pattern);
}

static inline void paint_pattern_main(RECT dest_rect, short which_pattern)
{
	paint_pattern(std::monostate{}, 1, dest_rect, which_pattern);
}

static inline void paint_pattern_dc(HDC dest, RECT dest_rect, short which_pattern)
{
	paint_pattern(dest, 2, dest_rect, which_pattern);
}
