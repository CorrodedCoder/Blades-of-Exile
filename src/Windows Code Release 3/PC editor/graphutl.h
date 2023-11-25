void init_palette(BYTE * lpDib);
void extract_given_palette(BYTE * lpDib);
void inflict_palette();
void reset_palette();
DWORD GetDibInfoHeaderSize(BYTE * lpDib);
WORD GetDibWidth(BYTE * lpDib);
WORD GetDibHeight(BYTE * lpDib);
BYTE * GetDibBitsAddr(BYTE * lpDib);
HBITMAP ReadDib(char * name,HDC hdc) ;
HBITMAP load_pict(short pict_num,HDC model_hdc);
void rect_draw_some_item(HBITMAP src,RECT src_rect,HBITMAP dest,RECT dest_rect,
	short trans, short main_win) ;
	void fry_dc(HWND hwnd,HDC dc) ;
	void DisposeGWorld(HBITMAP bitmap);
void SectRect(RECT *a, RECT *b, RECT *c); 
Boolean Button();
void paint_pattern(HBITMAP dest,short which_mode,RECT dest_rect,short which_pattern);