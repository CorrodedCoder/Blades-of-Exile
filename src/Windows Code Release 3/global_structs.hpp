#pragma once

#include "boe/game_file_structures.hpp"

typedef struct {
	char town_strs[180][256];
	} piles_of_stuff_dumping_type;
typedef struct {
	char talk_strs[170][256];
	} piles_of_stuff_dumping_type3;
typedef struct {
	char out_strs[9][256];
	} outdoor_strs_type;
typedef struct {
	outdoor_strs_type outdoor_text[2][2];
	} piles_of_stuff_dumping_type4;
typedef struct {
char scen_strs[160][256];
	} piles_of_stuff_dumping_type5;

typedef struct {unsigned char pattern[9][9];} effect_pat_type;
