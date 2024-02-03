#pragma once

#include <Windows.h>

DWORD GetDibInfoHeaderSize(const BYTE* lpDib);
const BYTE* GetDibBitsAddr(const BYTE* lpDib);
HPALETTE CreatePaletteFromDib(const BYTE* lpDib);
