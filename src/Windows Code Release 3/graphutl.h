#pragma once

#include <variant>

using PaintDrawDestination = std::variant<HBITMAP, HDC, std::monostate>;
using RectDrawDestination = std::variant<HBITMAP, HDC>;

void inflict_palette();
void reset_palette();
BYTE * GetDibBitsAddr(BYTE * lpDib);
HBITMAP ReadDib(const char * name,HDC hdc);
HBITMAP load_pict(short pict_num, HDC model_hdc);
void rect_draw_some_item(HBITMAP src,RECT src_rect, RectDrawDestination dest,RECT dest_rect,short trans, short main_win);
void fry_dc(HWND hwnd,HDC dc);
void DisposeGWorld(HBITMAP bitmap);
void paint_pattern(PaintDrawDestination dest,short which_mode,RECT dest_rect,short which_pattern);
