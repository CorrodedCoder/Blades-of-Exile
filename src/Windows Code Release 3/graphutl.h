void inflict_palette();
void reset_palette();
void init_palette(BYTE * lpDib);
DWORD GetDibInfoHeaderSize(BYTE * lpDib);
WORD GetDibHeight(BYTE * lpDib);
BYTE * GetDibBitsAddr(BYTE * lpDib);
HBITMAP ReadDib(const char * name,HDC hdc);
WORD GetDibWidth(BYTE *);
void rect_draw_some_item(HBITMAP src,RECT src_rect,HBITMAP dest,RECT dest_rect,short trans, short main_win);
void fry_dc(HWND hwnd,HDC dc);
void DisposeGWorld(HBITMAP bitmap);
Boolean Button();
HBITMAP load_pict(short pict_num,HDC model_hdc);
void paint_pattern(HBITMAP dest,short which_mode,RECT dest_rect,short which_pattern);

