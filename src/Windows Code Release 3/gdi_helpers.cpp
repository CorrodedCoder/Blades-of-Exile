#include "gdi_helpers.hpp"

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
