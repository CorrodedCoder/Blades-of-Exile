#pragma once

short spell_level(short id);
short spell_cost(short type, short id);
#define mage_spell_cost(ID) spell_cost(0, ID)
#define priest_spell_cost(ID) spell_cost(1, ID)
short spell_w_cast(short type, short id);

const char* mage_spell_name(short id);
const char* priest_spell_name(short id);
const char* alchemy_name_short(short id);
const char* alchemy_name(short id);
