#pragma once

#include <Windows.h>
#include <vector>
#include <string_view>

DWORD GetDibInfoHeaderSize(const BYTE* lpDib);
const BYTE* GetDibBitsAddr(const BYTE* lpDib);
HPALETTE CreatePaletteFromDib(const BYTE* lpDib);
std::vector<BYTE> LoadDibData(const char * name);
