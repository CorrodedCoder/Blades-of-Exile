#define xmin(a,b)	((a) < (b) ?  (a) : (b))

#include <Windows.h>
#include <windowsx.h>
#include <commdlg.h>
#include <format>
#include <string_view>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "global.h"
#include "edsound.h"
#include "../graphutl.h"
#include "../graphutl_helpers.hpp"
#include "../gdi_helpers.hpp"

extern HWND mainPtr;
extern HPALETTE hpal;
extern PALETTEENTRY ape[256];
extern HDC main_dc,main_dc2,main_dc3;
extern short ulx,uly;
extern HBITMAP mixed_gworld,dialog_pattern_gworld,pattern_gworld,status_pattern_gworld;
extern Boolean suppress_load_file_name;

extern char scen_strs[160][256];
extern char scen_strs2[110][256];
extern char talk_strs[170][256];
extern char town_strs[180][256];
extern char file_path_name[256];
HPALETTE opening_palette;

BOOL pal_ok = FALSE;
Boolean syscolors_stored = FALSE;
	int elements[5] = {COLOR_ACTIVEBORDER,COLOR_ACTIVECAPTION,
			COLOR_WINDOWFRAME,COLOR_SCROLLBAR,COLOR_BTNFACE};
	COLORREF store_element_colors[5];
short dlog_pat_placed = 0;
short current_pattern = -1;
HPALETTE syspal = NULL;

static void init_palette(BYTE * lpDib)
{
 	if (pal_ok == TRUE)
		return;
	pal_ok = TRUE;

	hpal = CreatePaletteFromDib(lpDib);
	HDC hdc = ::GetDC(mainPtr);
	::GetSystemPaletteEntries(hdc,0,255,ape);
	inflict_palette();
	::ReleaseDC(mainPtr,hdc);
#ifdef REENABLE_SENDMESSAGE_CALLS
	::SendMessage(HWND_BROADCAST, WM_SYSCOLORCHANGE, 0, 0);
#else
	::PostMessage(HWND_BROADCAST, WM_SYSCOLORCHANGE, 0, 0);
#endif
}

// extracts and inflicts palette from given dib. WARNING ...
// does NOT do any deleting or cleanup
static void extract_given_palette(const BYTE * lpDib)
{
	opening_palette = CreatePaletteFromDib(lpDib);
	HDC hdc = ::GetDC(mainPtr);
	//GetSystemPaletteEntries(hdc,0,255,(PALETTEENTRY FAR*) ape);
	//inflict_palette();
	::SelectPalette(main_dc,opening_palette,0);
	::RealizePalette(main_dc);
	::SetSystemPaletteUse(main_dc,SYSPAL_NOSTATIC);
	::UnrealizeObject(opening_palette);
	::RealizePalette(main_dc);
	::ReleaseDC(mainPtr,hdc);
	//SendMessage(HWND_BROADCAST,WM_SYSCOLORCHANGE,0,0);
}


void inflict_palette()
{
	COLORREF x = RGB(0,0,192);
	UINT c;
	COLORREF elem_color[5];

	if (syscolors_stored == FALSE) {
		store_element_colors[0] = GetSysColor(elements[0]);
		store_element_colors[1] = GetSysColor(elements[1]);
		store_element_colors[2] = GetSysColor(elements[2]);
		store_element_colors[3] = GetSysColor(elements[3]);
		store_element_colors[4] = GetSysColor(elements[4]);
		syscolors_stored = TRUE;
		}
	c = GetNearestPaletteIndex(hpal,x);
	elem_color[0] = RGB(192,192,192);
	elem_color[1] = RGB(128,128,128);
	elem_color[2] = RGB(0,0,0);//192,192,192);
	elem_color[3] = RGB(128,128,128);
	elem_color[4] = RGB(192,192,192);
	SetSysColors(5,elements,elem_color);
// 	int elements[5] = {COLOR_ACTIVEBORDER,COLOR_ACTIVECAPTION,
//			COLOR_WINDOWFRAME,COLOR_SCROLLBAR,COLOR_BTNFACE};

	SelectPalette(main_dc,hpal,0);
	RealizePalette(main_dc);
	SetSystemPaletteUse(main_dc,SYSPAL_NOSTATIC);
	UnrealizeObject(hpal);
	RealizePalette(main_dc);
#ifdef REENABLE_SENDMESSAGE_CALLS
	SendMessage(HWND_BROADCAST, WM_SYSCOLORCHANGE, 0, 0);
#else
	PostMessage(HWND_BROADCAST, WM_SYSCOLORCHANGE, 0, 0);
#endif
	/*
	store_element_colors[0] = GetSysColor(elements[0]);
	store_element_colors[1] = GetSysColor(elements[1]);
	store_element_colors[2] = GetSysColor(elements[2]);
	c = GetNearestPaletteIndex(hpal,x);
	elem_color[0] = RGB(255,0,0);
	elem_color[1] = RGB(255,0,0);
	elem_color[2] = RGB(255,0,0);
	SetSysColors(3,elements,elem_color);
*/
	}

void reset_palette()
{
	LOGPALETTE *plgpl = NULL;
	LOCALHANDLE l;
	short i;

	if (syspal == NULL) {
			l = LocalAlloc(LHND,
		sizeof(LOGPALETTE) + 255 * sizeof(PALETTEENTRY));
	plgpl = (LOGPALETTE*) LocalLock(l);

	plgpl->palNumEntries = (WORD) (255);
	plgpl->palVersion = 0x300;

	for (i = 0; i < 255; i++) {
				plgpl->palPalEntry[i].peRed = ape[i].peRed;
				plgpl->palPalEntry[i].peGreen = ape[i].peGreen;
				plgpl->palPalEntry[i].peBlue = ape[i].peBlue;
				plgpl->palPalEntry[i].peFlags = PC_NOCOLLAPSE;
			}

		syspal = CreatePalette(plgpl);
		}
	SetSystemPaletteUse(main_dc,SYSPAL_STATIC);
	UnrealizeObject(hpal);
	SelectPalette(main_dc,syspal,0);
	RealizePalette(main_dc);
#ifdef REENABLE_SENDMESSAGE_CALLS
	SendMessage(HWND_BROADCAST, WM_SYSCOLORCHANGE, 0, 0);
#else
	PostMessage(HWND_BROADCAST, WM_SYSCOLORCHANGE, 0, 0);
#endif
	SetSysColors(5,elements,store_element_colors);


}

HBITMAP ReadDib(const char * name,HDC hdc)
{
	BITMAPFILEHEADER bmfh;
	BYTE * lpDib;
	DWORD dwDibSize, dwOffset, dwHeaderSize;
	int hFile;
	WORD wDibRead;
	const BYTE * lpDibBits;
	HBITMAP bmap;
	OFSTRUCT store;
	char real_name[256] = "";

	if (suppress_load_file_name == FALSE)
	{
		short last_slash = -1;
		for (short i = 0; i < 256; i++)
			if ((file_path_name[i] == 92) || (file_path_name[i] == '/'))
				last_slash = i;
		if (last_slash < 0)
		{
			strcpy(real_name,name);
		}
		else
		{
			strcpy(real_name, file_path_name);
			strcpy(real_name + last_slash + 1, name);
		}
	}
	else
	{
		strcpy(real_name, name);
	}

	{
		// CC: Temporary kludge for long filenames as OpenFile can only handle up to 128 characters
		char real_name_short[256] = "";
		(void)GetShortPathName(real_name, real_name_short, sizeof(real_name_short));
		hFile = OpenFile(real_name_short, &store, OF_READ | OF_SHARE_DENY_WRITE /* | OF_SEARCH */);
	}

	if (hFile == HFILE_ERROR)
		return NULL;
//	if (-1 == (hFile = _lopen(name,OF_READ|OF_SHARE_DENY_WRITE)))
//		return NULL;
	if (_lread(hFile, (LPSTR) &bmfh, sizeof(BITMAPFILEHEADER))
	!= sizeof(BITMAPFILEHEADER)) {
		_lclose(hFile);
		return NULL;
		}

//	if (bmfh.bfType != * (WORD *) "BM") {
//		_lclose(hFile);
//		return NULL;
//		}
	dwDibSize = bmfh.bfSize - sizeof(BITMAPFILEHEADER);
	lpDib = (BYTE *) GlobalAllocPtr(GMEM_MOVEABLE, dwDibSize);
	if (lpDib == NULL){
		_lclose(hFile);
		return NULL;
		}
	dwOffset = 0;
	while (dwDibSize > 0) {
		wDibRead = (WORD) xmin(32768ul,dwDibSize);
		if (wDibRead != _lread(hFile, (LPSTR) (lpDib + dwOffset),
			wDibRead)) {
				_lclose(hFile);
				GlobalFreePtr(lpDib);
				return NULL;
				}
			dwDibSize -= wDibRead;
			dwOffset += wDibRead;
		}
	_lclose(hFile);
	dwHeaderSize = GetDibInfoHeaderSize(lpDib);
	if ((dwHeaderSize < 12) || ((dwHeaderSize > 12) && (dwHeaderSize < 16))) {
		GlobalFreePtr(lpDib);
		return NULL;
		}

	init_palette(lpDib);
	SelectPalette(hdc,hpal,0);

	if ((name[0] == 'S') && (name[1] == 'T') && (name[5] == '.'))  {
		extract_given_palette(lpDib);
		}
	lpDibBits = GetDibBitsAddr(lpDib);
	bmap = CreateDIBitmap(hdc,(LPBITMAPINFOHEADER) lpDib, CBM_INIT,
		(LPSTR) lpDibBits, (LPBITMAPINFO) lpDib, DIB_RGB_COLORS);
	GlobalFreePtr(lpDib);
	return bmap;
}


static std::string_view lookup_picname(short id)
{
	std::string_view res;
	switch (id)
	{
	case 700: case 701: case 702: res = "STATAREA"; break;
	case 703: res = "TEXTBAR"; break;
	case 704: res = "BUTTONS"; break;
	case 705: res = "TERSCRN"; break;
	case 800: res = "TER1"; break;
	case 801: res = "TER2"; break;
	case 802: res = "TER3"; break;
	case 803: res = "TER4"; break;
	case 804: res = "TER5"; break;
	case 805: res = "TER6"; break;
	case 820: res = "TERANIM"; break;
	case 821: res = "FIELDS"; break;
	case 830: res = "STARTUP"; break;
	case 850: res = "DLOGPICS"; break;
	case 851: res = "SCENPICS"; break;
	case 860: res = "TALKPORT"; break;
	case 880: res = "MISSILES"; break;
	case 900: res = "TINYOBJ"; break;
	case 901: res = "OBJECTS"; break;
	case 902: res = "PCS"; break;
	case 905: res = "PCS"; break;
	case 903: case 904: res = "MIXED"; break;
	case 906: res = "EDBTNS"; break;
	case 910: case 911: case 912: res = "BIGSCEN"; break;
	case 1100: case 1200: res = "MONST1"; break;
	case 1101: case 1201: res = "MONST2"; break;
	case 1102: case 1202: res = "MONST3"; break;
	case 1103: case 1203: res = "MONST4"; break;
	case 1104: case 1204: res = "MONST5"; break;
	case 1105: case 1205: res = "MONST6"; break;
	case 1106: case 1206: res = "MONST7"; break;
	case 1107: case 1207: res = "MONST8"; break;
	case 1108: case 1208: res = "MONST9"; break;
	case 1109: case 1209: res = "MONST10"; break;
	case 1400: res = "STSCICON"; break;
	case 1401: res = "HELPPICS"; break;
	case 1402: res = "APPIC"; break;
	case 1500: case 1501: case 1502: case 1503: case 1504: case 1505: case 1506: case 1507:
		res = "BIGMAPS"; break;
	case 2000: res = "DLOGBTNS"; break;
	case 3000: res = "START"; break;
	case 3001: res = "SPIDLOGO"; break;
	case 3002: res = "EDSTART"; break;
	case 5000: res = "E3EDTITL"; break;
	default:
		throw std::runtime_error(std::format("Unable to find picture for id={}", id));
		break;
	}
	return res;
}

HBITMAP load_pict(short pict_num, HDC model_hdc)
{
	return ReadDib(std::format("{}.BMP", lookup_picname(pict_num)).c_str(), model_hdc);
}

void rect_draw_some_item(HBITMAP src,RECT src_rect, RectDrawDestination dest,RECT dest_rect,
	short trans, short main_win) {
	HDC hdcMem,hdcMem2,hdcMem3,destDC;
	HBITMAP transbmp;
	COLORREF white = RGB(255,255,255),oldcolor;
	HBRUSH hbrush;
	HGDIOBJ old_brush;
	COLORREF x = RGB(17,17,17);
	UINT c;
	HGDIOBJ store,store2;
	Boolean dlog_draw = FALSE;

	main_dc2 = CreateCompatibleDC(main_dc);
	SetMapMode(main_dc2,GetMapMode(main_dc));
	SelectPalette(main_dc2,hpal,0);
	main_dc3 = CreateCompatibleDC(main_dc);
	SetMapMode(main_dc3,GetMapMode(main_dc));
	SelectPalette(main_dc3,hpal,0);
	SetStretchBltMode(main_dc2,STRETCH_DELETESCANS);
	SetStretchBltMode(main_dc3,STRETCH_DELETESCANS);

	if (main_win == 2) {
		destDC = std::get<HDC>(dest);
		main_win = 1;
		dlog_draw = TRUE;
		hdcMem = CreateCompatibleDC(destDC);
		SelectObject(hdcMem, src);
		SetMapMode(hdcMem,GetMapMode(destDC));
		SelectPalette(hdcMem,hpal,0);
		SetStretchBltMode(hdcMem,STRETCH_DELETESCANS);
		}
		else {
			destDC = main_dc;
			hdcMem = main_dc2;
			store = SelectObject(hdcMem,src);
			}


	SetStretchBltMode(destDC,STRETCH_DELETESCANS);
	SetStretchBltMode(hdcMem,STRETCH_DELETESCANS);
	if (trans != 1) {
		if (main_win == 0) { // Not transparent, into bitmap
			hdcMem2 = main_dc3;
			store2 = SelectObject(hdcMem2, std::get<HBITMAP>(dest));
			/*CreateCompatibleDC(hdcMem);
			SelectObject(hdcMem2, dest);
			SetMapMode(hdcMem2,GetMapMode(mainPtr));
			SelectPalette(hdcMem2,hpal,0);  */

			SetStretchBltMode(hdcMem2,STRETCH_DELETESCANS);

				StretchBlt(hdcMem2,dest_rect.left,dest_rect.top,dest_rect.right - dest_rect.left,
				dest_rect.bottom - dest_rect.top,
				hdcMem,src_rect.left,src_rect.top,src_rect.right - src_rect.left,
				src_rect.bottom - src_rect.top,(trans >= 0) ? SRCCOPY : SRCAND);
			SelectObject(hdcMem2,store2);
			//DeleteDC(hdcMem2);
			}

		else { // Not transparent, onto screen
		if (trans == 2) {
			c = GetNearestPaletteIndex(hpal,x);
			hbrush = CreateSolidBrush(PALETTEINDEX(c));
			old_brush = SelectObject(destDC,hbrush);
			//SelectObject(hdcMem,hbrush);
			}
		if (dlog_draw == FALSE)
			SetViewportOrgEx(destDC,ulx,uly,nullptr);

		SetStretchBltMode(destDC,STRETCH_DELETESCANS);
		SetStretchBltMode(hdcMem,STRETCH_DELETESCANS);
		StretchBlt(destDC,dest_rect.left,dest_rect.top,dest_rect.right - dest_rect.left,
			dest_rect.bottom - dest_rect.top,
			hdcMem,src_rect.left,src_rect.top,src_rect.right - src_rect.left,
			src_rect.bottom - src_rect.top,(trans == 0) ? SRCCOPY : MERGECOPY);
			if (trans == 2) {
				SelectObject(destDC,old_brush);
				if (DeleteObject(hbrush) == 0)
					play_sound(1);
				}
		if (dlog_draw == FALSE)
			SetViewportOrgEx(destDC,0,0,nullptr);


		}
		} // end of non-transparent draws
		else {
		if (main_win == 0) {
			hdcMem3 = CreateCompatibleDC(hdcMem);
			SelectObject(hdcMem3, std::get<HBITMAP>(dest));
			SetMapMode(hdcMem3,GetMapMode(hdcMem));
			SelectPalette(hdcMem3,hpal,0);
			transbmp = CreateBitmap(src_rect.right - src_rect.left,
						src_rect.bottom - src_rect.top,1,1,NULL);
		hdcMem2 = CreateCompatibleDC(destDC);
		SelectObject(hdcMem2, transbmp);
		oldcolor = SetBkColor(hdcMem, white);
		StretchBlt(hdcMem2,0,0,dest_rect.right - dest_rect.left,
			dest_rect.bottom - dest_rect.top,
			hdcMem,src_rect.left,src_rect.top,src_rect.right - src_rect.left,
			src_rect.bottom - src_rect.top,SRCCOPY);
		SetBkColor(hdcMem, oldcolor);

		StretchBlt(hdcMem3,dest_rect.left,dest_rect.top,dest_rect.right - dest_rect.left,
			dest_rect.bottom - dest_rect.top,
			hdcMem,src_rect.left,src_rect.top,src_rect.right - src_rect.left,
			src_rect.bottom - src_rect.top,SRCINVERT);
		StretchBlt(hdcMem3,dest_rect.left,dest_rect.top,dest_rect.right - dest_rect.left,
			dest_rect.bottom - dest_rect.top,
			hdcMem2,0,0,src_rect.right - src_rect.left,
			src_rect.bottom - src_rect.top,SRCAND);
		StretchBlt(hdcMem3,dest_rect.left,dest_rect.top,dest_rect.right - dest_rect.left,
			dest_rect.bottom - dest_rect.top,
			hdcMem,src_rect.left,src_rect.top,src_rect.right - src_rect.left,
			src_rect.bottom - src_rect.top,SRCINVERT);
		DeleteDC(hdcMem3);
		DeleteDC(hdcMem2);

			DeleteObject(transbmp);
		}
		else {
			if (dlog_draw == FALSE)
				SetViewportOrgEx(destDC,ulx,uly,nullptr);
			transbmp = CreateBitmap(src_rect.right - src_rect.left,
						src_rect.bottom - src_rect.top,1,1,NULL);
			hdcMem2 = CreateCompatibleDC(destDC);
			SelectObject(hdcMem2, transbmp);
			oldcolor = SetBkColor(hdcMem, white);
			StretchBlt(hdcMem2,0,0,dest_rect.right - dest_rect.left,
				dest_rect.bottom - dest_rect.top,
				hdcMem,src_rect.left,src_rect.top,src_rect.right - src_rect.left,
				src_rect.bottom - src_rect.top,SRCCOPY);

			SetBkColor(hdcMem, oldcolor);

			StretchBlt(destDC,dest_rect.left,dest_rect.top,dest_rect.right - dest_rect.left,
				dest_rect.bottom - dest_rect.top,
				hdcMem,src_rect.left,src_rect.top,src_rect.right - src_rect.left,
				src_rect.bottom - src_rect.top,SRCINVERT);
			StretchBlt(destDC,dest_rect.left,dest_rect.top,dest_rect.right - dest_rect.left,
				dest_rect.bottom - dest_rect.top,
				hdcMem2,0,0,src_rect.right - src_rect.left,
				src_rect.bottom - src_rect.top,SRCAND);
			StretchBlt(destDC,dest_rect.left,dest_rect.top,dest_rect.right - dest_rect.left,
				dest_rect.bottom - dest_rect.top,
				hdcMem,src_rect.left,src_rect.top,src_rect.right - src_rect.left,
				src_rect.bottom - src_rect.top,SRCINVERT);
			if (dlog_draw == FALSE)
				SetViewportOrgEx(destDC,0,0,nullptr);
			DeleteDC(hdcMem2);

				DeleteObject(transbmp);


			}
			}
	if (dlog_draw == TRUE)
		DeleteDC(hdcMem);
		else SelectObject(hdcMem,store);
	DeleteDC(main_dc2);
	DeleteDC(main_dc3);
}

 void fry_dc(HWND hwnd,HDC dc) {
	if (ReleaseDC(hwnd,dc) == 0)
		PostQuitMessage(0);
 }

void DisposeGWorld(HBITMAP bitmap)
{
	DeleteObject(bitmap);
}


// which_mode is 0 ... dest is a bitmap
// is 1 ... ignore dest ... paint on mainPtr
// is 2 ... dest is a dialog, use the dialog pattern
// both pattern gworlds are 192 x 256
void paint_pattern(PaintDrawDestination dest,short which_mode,RECT dest_rect,short which_pattern)
{
	HBITMAP source_pat;
	RECT pattern_source = {32,168,96,232}, pat_dest_orig = {0,0,64,64},pat_dest;
	short i,j;

	RECT draw_from_orig = {0,0,192,256},draw_from,draw_to;
	short store_ulx,store_uly;

	if (which_mode == 1)
		OffsetRect(&dest_rect,ulx, uly);
		else if (which_mode == 3)
      	which_mode = 1;
	if (which_mode == 2) {
		source_pat = dialog_pattern_gworld;
		if (dlog_pat_placed == 0) {
			dlog_pat_placed = 1;
			OffsetRect(&pattern_source, 64 * 2,0);
			for (i = 0; i < 3; i++)
				for (j = 0; j < 4; j++) {
					pat_dest = pat_dest_orig;
					OffsetRect(&pat_dest,64 * i, 64 * j);
					rect_draw_some_item_bmp(mixed_gworld,pattern_source,
						dialog_pattern_gworld,pat_dest,0,0);
					}
			}
		}
		else {
			source_pat = pattern_gworld;
			if (current_pattern != which_pattern) {
				current_pattern = which_pattern;
				OffsetRect(&pattern_source, 64 * (which_pattern % 5),
					64 * (which_pattern / 5));
				for (i = 0; i < 3; i++)
					for (j = 0; j < 4; j++) {
						pat_dest = pat_dest_orig;
						OffsetRect(&pat_dest,64 * i, 64 * j);
						rect_draw_some_item_bmp(mixed_gworld,pattern_source,
							pattern_gworld,pat_dest,0,0);
						}
				}
			}

	// now patterns are loaded, so have fun
	// first nullify ul shifting
	store_ulx = ulx;
	store_uly = uly;
	ulx = uly = 0;
	for (i = 0; i < (dest_rect.right / 192) + 1; i++)
		for (j = 0; j < (dest_rect.bottom / 256) + 1; j++) {
			draw_to = draw_from_orig;
			OffsetRect(&draw_to,192 * i, 256 * j);
			IntersectRect(&draw_to,&draw_to,&dest_rect);
			if (draw_to.right != 0) {
				draw_from = draw_to;
				OffsetRect(&draw_from, -192 * i, -256 * j);
				switch (which_mode) {
					case 0:
						rect_draw_some_item_bmp(source_pat,draw_from,
							std::get<HBITMAP>(dest),draw_to,0,0); break;
					case 1:
						rect_draw_some_item_bmp(source_pat,draw_from,
							source_pat,draw_to,0,1); break;
					case 2:
						rect_draw_some_item_dc(source_pat,draw_from,
							std::get<HDC>(dest),draw_to,0,2); break;
					}
				}
			}
	ulx = store_ulx;
   uly = store_uly;
}
