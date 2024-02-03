#include "gdi_helpers.hpp"
#include <vector>
#include <fstream>
#include <format>

namespace {

DWORD GetDibInfoHeaderSize(std::span<const BYTE> dib)
{
	return reinterpret_cast<const BITMAPINFOHEADER*>(dib.data())->biSize;
}

} // anonymous namespace

std::span<const BYTE> GetDibBitsAddr(std::span<const BYTE> dib)
{
	DWORD dwColorTableSize = 0;

	if (GetDibInfoHeaderSize(dib) == sizeof(BITMAPCOREHEADER))
	{
		auto core_header = reinterpret_cast<const BITMAPCOREHEADER*>(dib.data());
		const WORD wBitCount = core_header->bcBitCount;
		const DWORD dwNumColors = (wBitCount != 24) ? (1L << wBitCount) : 0;
		dwColorTableSize = dwNumColors * sizeof(RGBTRIPLE);
	}
	else
	{
		auto info_header = reinterpret_cast<const BITMAPINFOHEADER*>(dib.data());
		const WORD wBitCount = info_header->biBitCount;
		DWORD dwNumColors = 0;
		if (GetDibInfoHeaderSize(dib) >= 36)
		{
			dwNumColors = info_header->biClrUsed;
		}
		if (dwNumColors == 0)
		{
			dwNumColors = (wBitCount != 24) ? (1L << wBitCount) : 0;
		}
		dwColorTableSize = dwNumColors * sizeof(RGBQUAD);
	}

	return dib.subspan(GetDibInfoHeaderSize(dib) + dwColorTableSize);
}

HPALETTE CreatePaletteFromDib(std::span<const BYTE> dib)
{
	RGBTRIPLE store_c[256];
	DWORD dwColorTableSize = 0;
	DWORD dwNumColors = 0;

	if (GetDibInfoHeaderSize(dib) == sizeof(BITMAPCOREHEADER))
	{
		auto& core_header = *reinterpret_cast<const BITMAPCOREHEADER*>(dib.data());
		const WORD wBitCount = core_header.bcBitCount;
		dwNumColors = (wBitCount != 24) ? (1L << wBitCount) : 0;
		dwColorTableSize = dwNumColors * sizeof(RGBTRIPLE);

		auto& core_info = *reinterpret_cast<const BITMAPCOREINFO*>(dib.data());
		for (DWORD i = 0; i < dwNumColors; i++)
		{
			store_c[i].rgbtRed = core_info.bmciColors[i].rgbtRed;
			store_c[i].rgbtGreen = core_info.bmciColors[i].rgbtGreen;
			store_c[i].rgbtBlue = core_info.bmciColors[i].rgbtBlue;
		}
	}
	else
	{
		auto& info_header = *reinterpret_cast<const BITMAPINFOHEADER*>(dib.data());
		const WORD wBitCount = info_header.biBitCount;
		if (GetDibInfoHeaderSize(dib) >= 36)
		{
			dwNumColors = info_header.biClrUsed;
		}
		if (dwNumColors == 0)
		{
			dwNumColors = (wBitCount != 24) ? (1L << wBitCount) : 0;
		}
		dwColorTableSize = dwNumColors * sizeof(RGBQUAD);

		auto& bitmap_info = *reinterpret_cast<const BITMAPINFO*>(dib.data());
		for (DWORD i = 0; i < dwNumColors; i++)
		{
			store_c[i].rgbtRed = bitmap_info.bmiColors[i].rgbRed;
			store_c[i].rgbtGreen = bitmap_info.bmiColors[i].rgbGreen;
			store_c[i].rgbtBlue = bitmap_info.bmiColors[i].rgbBlue;
		}
	}

	std::vector<BYTE> bytes(sizeof(LOGPALETTE) + (dwNumColors * sizeof(PALETTEENTRY)));
	LOGPALETTE& lpal = *reinterpret_cast<LOGPALETTE*>(&bytes[0]);

	lpal.palNumEntries = (WORD)(dwNumColors);
	lpal.palVersion = 0x300;

	for (DWORD i = 0; i < dwNumColors; i++) {

		lpal.palPalEntry[i].peRed = store_c[i].rgbtRed;
		lpal.palPalEntry[i].peGreen = store_c[i].rgbtGreen;
		lpal.palPalEntry[i].peBlue = store_c[i].rgbtBlue;
		lpal.palPalEntry[i].peFlags = PC_NOCOLLAPSE;
	}

	return ::CreatePalette(&lpal);
}

std::vector<BYTE> LoadDibData(const char* name)
{
	std::ifstream input(name, std::ios_base::binary);
	input.exceptions(std::ios::failbit);

	BITMAPFILEHEADER bmfh;
	input.read(reinterpret_cast<char *>(&bmfh), sizeof(bmfh));

	std::vector<BYTE> dib(bmfh.bfSize - sizeof(BITMAPFILEHEADER));
	input.read(reinterpret_cast<char *>(&dib[0]), dib.size());

	const DWORD dwHeaderSize = GetDibInfoHeaderSize(dib);
	if ((dwHeaderSize < 12) || ((dwHeaderSize > 12) && (dwHeaderSize < 16)))
	{
		throw std::runtime_error(std::format("Bad header size ({}) for bitmap {}", dwHeaderSize, name));
	}
	return dib;
}
