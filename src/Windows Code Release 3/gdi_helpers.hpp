#pragma once

#include <Windows.h>
#include <vector>
#include <string_view>
#include <span>

std::span<const BYTE> GetDibBitsAddr(std::span<const BYTE> data);
HPALETTE CreatePaletteFromDib(std::span<const BYTE> data);
std::vector<BYTE> LoadDibData(const char * name);
