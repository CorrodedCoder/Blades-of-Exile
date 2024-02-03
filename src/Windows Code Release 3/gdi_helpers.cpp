#include "gdi_helpers.hpp"
#include <vector>
#include <fstream>
#include <format>

DWORD GetDibInfoHeaderSize(const BYTE* lpDib)
{
	return ((const BITMAPINFOHEADER*)lpDib)->biSize;
}

const BYTE* GetDibBitsAddr(const BYTE* lpDib)
{
	DWORD dwNumColors, dwColorTableSize;
	WORD wBitCount;

	if (GetDibInfoHeaderSize(lpDib) == sizeof(BITMAPCOREHEADER))
	{
		wBitCount = ((BITMAPCOREHEADER*)lpDib)->bcBitCount;
		if (wBitCount != 24)
			dwNumColors = 1L << wBitCount;
		else
			dwNumColors = 0;
		dwColorTableSize = dwNumColors * sizeof(RGBTRIPLE);
	}
	else
	{
		wBitCount = ((BITMAPINFOHEADER*)lpDib)->biBitCount;
		if (GetDibInfoHeaderSize(lpDib) >= 36)
			dwNumColors = ((BITMAPINFOHEADER*)lpDib)->biClrUsed;
		if (dwNumColors == 0)
		{
			if (wBitCount != 24)
				dwNumColors = 1L << wBitCount;
			else
				dwNumColors = 0;
		}
		dwColorTableSize = dwNumColors * sizeof(RGBQUAD);
	}

	return lpDib + GetDibInfoHeaderSize(lpDib) + dwColorTableSize;
}

HPALETTE CreatePaletteFromDib(const BYTE* lpDib)
{
	short i;
	LOGPALETTE* plgpl = NULL;
	BITMAPINFO* store_info;
	BITMAPCOREINFO* store_core_info;
	RGBTRIPLE store_c[256];
	DWORD dwNumColors, dwColorTableSize;
	WORD wBitCount;

	if (GetDibInfoHeaderSize(lpDib) == sizeof(BITMAPCOREHEADER)) {
		wBitCount = ((BITMAPCOREHEADER*)lpDib)->bcBitCount;
		if (wBitCount != 24)
			dwNumColors = 1L << wBitCount;
		else
			dwNumColors = 0;
		dwColorTableSize = dwNumColors * sizeof(RGBTRIPLE);

		store_core_info = (BITMAPCOREINFO*)lpDib;
		for (i = 0; i < dwNumColors; i++) {
			store_c[i].rgbtRed = store_core_info->bmciColors[i].rgbtRed;
			store_c[i].rgbtGreen = store_core_info->bmciColors[i].rgbtGreen;
			store_c[i].rgbtBlue = store_core_info->bmciColors[i].rgbtBlue;
		}
	}
	else {
		wBitCount = ((BITMAPINFOHEADER*)lpDib)->biBitCount;
		if (GetDibInfoHeaderSize(lpDib) >= 36)
			dwNumColors = ((BITMAPINFOHEADER*)lpDib)->biClrUsed;
		if (dwNumColors == 0) {
			if (wBitCount != 24)
				dwNumColors = 1L << wBitCount;
			else
				dwNumColors = 0;
		}
		dwColorTableSize = dwNumColors * sizeof(RGBQUAD);

		store_info = (BITMAPINFO*)lpDib;
		for (i = 0; i < dwNumColors; i++)
		{
			store_c[i].rgbtRed = store_info->bmiColors[i].rgbRed;
			store_c[i].rgbtGreen = store_info->bmiColors[i].rgbGreen;
			store_c[i].rgbtBlue = store_info->bmiColors[i].rgbBlue;
		}
	}

	std::vector<BYTE> bytes(sizeof(LOGPALETTE) + (dwNumColors * sizeof(PALETTEENTRY)));
	plgpl = (LOGPALETTE*)&bytes[0];

	plgpl->palNumEntries = (WORD)(dwNumColors);
	plgpl->palVersion = 0x300;

	for (i = 0; i < dwNumColors; i++) {

		plgpl->palPalEntry[i].peRed = store_c[i].rgbtRed;
		plgpl->palPalEntry[i].peGreen = store_c[i].rgbtGreen;
		plgpl->palPalEntry[i].peBlue = store_c[i].rgbtBlue;
		plgpl->palPalEntry[i].peFlags = PC_NOCOLLAPSE;
	}

	return ::CreatePalette(plgpl);
}

std::vector<BYTE> LoadDibData(const char* name)
{
	std::ifstream input(name, std::ios_base::binary);
	input.exceptions(std::ios::failbit);

	BITMAPFILEHEADER bmfh;
	input.read(reinterpret_cast<char *>(&bmfh), sizeof(bmfh));

	std::vector<BYTE> dib(bmfh.bfSize - sizeof(BITMAPFILEHEADER));
	input.read(reinterpret_cast<char *>(&dib[0]), dib.size());

	const DWORD dwHeaderSize = GetDibInfoHeaderSize(&dib[0]);
	if ((dwHeaderSize < 12) || ((dwHeaderSize > 12) && (dwHeaderSize < 16)))
	{
		throw std::runtime_error(std::format("Bad header size ({}) for bitmap {}", dwHeaderSize, name));
	}
	return dib;
}
