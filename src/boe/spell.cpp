#include "boe/spell.hpp"
#include <array>

namespace
{
	const std::array<short, 62> c_spell_level{
		1,1,1,1,1,1,1,1,1,1,
		2,2,2,2,2,2,2,2,2,2,
		3,3,3,3,3,3,3,3,3,3,
		4,4,4,4,4,4,4,4,
		5,5,5,5,5,5,5,5,
		6,6,6,6,6,6,6,6,
		7,7,7,7,7,7,7,7
	};
	const std::array< std::array<short, 62>, 2> c_spell_cost{ {
		{1,1,1,1,1,2,50,2,1,3, 2,3,2,2,2,2,4,4,2,6, 3,3,5,3,3,5,6,4,6,4,
			4,5,4,8,30,-1,8,6, 5,8,8,6,9,10,6,6, 7,6,8,7,12,10,12,20, 12,8,20,10,14,10,50,10},
		{1,1,1,2,1,1,3,5,50,1, 2,2,2,2,3,5,8,6,4,2, 3,4,3,3,3,10,5,3,4,6,
			5,5,5,15,6,5,5,8, 6,7,25,8,10,12,12,6, 8,7,8,8,14,17,8,7, 10,10,35,10,12,12,30,10}
	} };
	const std::array< std::array<short, 62>, 2> c_spell_w_cast{ {
		{0,1,1,1,1,1,3,4,1,2, 1,1,1,1,1,1,4,1,4,1, 2,1,1,0,1,1,4,1,1,0,
			1,1,1,2,4,1,1,1, 2,1,1,2,4,4,1,1, 1,1,1,1,4,4,1,5, 1,4,1,4,4,4,4,1},
		{1,0,0,1,3,1,1,3,2,1, 1,0,1,0,1,4,2,1,1,0, 0,0,1,2,0,3,1,0,0,1,
			0,1,1,3,4,1,0,0, 1,0,3,1,1,4,2,4, 0,0,0,3,4,1,1,1, 0,1,3,1,4,1,5,0}
	} };
	const std::array c_mage_s_name{
		"Light","Spark","Minor Haste","Strength","Scare",
		"Flame Cloud","Identify","Scry Monster","Goo","True Sight",
		"Minor Poison","Flame","Slow","Dumbfound","Envenom",
		"Stinking Cloud","Summon Beast","Conflagration","Dispel Field","Sleep Cloud",
		"Unlock","Haste","Fireball","Long Light","Fear",
		"Wall of Force","Weak Summoning","Flame Arrows","Web","Resist Magic",
		"Poison","Ice Bolt","Slow Group","Magic Map",
		"Capture Soul","Simulacrum","Venom Arrows","Wall of Ice",
		"Stealth","Major Haste","Fire Storm","D. Barrier",
		"Fire Barrier","Summoning","Shockstorm","Spray Fields",
		"Major Poison","Group Fear","Kill","Paralysis",
		"Daemon","Antimagic Cloud","MindDuel","Flight",
		"Shockwave","M. Blessing","Mass Paralysis","Protection",
		"Major Summon","Force Barrier","Quickfire","Death Arrows"
	};
	const std::array c_priest_s_name{
		"Minor Bless","Minor Heal","Weaken Poison","Turn Undead","Location",
		"Sanctuary","Symbiosis","Minor Manna","Ritual - Sanctify","Stumble",
		"Bless","Cure Poison","Curse","Light","Wound",
		"Summon Spirit","Move Mountains","Charm Foe","Disease","Awaken",
		"Heal","Light Heal All","Holy Scourge","Detect Life","Cure Paralysis",
		"Manna","Forcefield","Cure Disease","Restore Mind","Smite",
		"Cure Party","Curse All","Dispel Undead","Remove Curse",
		"Sticks to Snakes","Martyr's Shield","Cleanse","Firewalk",
		"Bless Party","Major Heal","Raise Dead","Flamestrike",
		"Mass Sanctuary","Summon Host","Shatter","Dispel Fields",
		"Heal All","Revive","Hyperactivity","Destone",
		"Guardian","Mass Charm","Protective Circle","Pestilence",
		"Revive All","Ravage Spirit","Resurrect","Divine Thud",
		"Avatar","Wall of Blades","Word of Recall","Major Cleansing"
	};
	const std::array c_alch_names{
		"Weak Curing Potion (1)","Weak Healing Potion (1)","Weak Poison (1)",
		"Weak Speed Potion (3)","Medium Poison (3)",
		"Medium Heal Potion (4)","Strong Curing (5)","Medium Speed Potion (5)",
		"Graymold Salve (7)","Weak Energy Potion (9)",
		"Potion of Clarity (9)","Strong Poison (10)","Strong Heal Potion (12)","Killer Poison (12)",
		"Resurrection Balm (9)","Medium Energy Ptn. (14)","Knowledge Brew (19)"	,
		"Strong Strength (10)","Bliss (16)","Strong Power (20)"
	};
	/* Alternative names as found in the PC editor
		"Weak Curing Potion (1)","Weak Healing Potion (1)","Weak Poison (1)",
		"Weak Speed Potion (3)","Medium Poison (3)",
		"Medium Heal Potion (4)","Strong Curing (5)","Medium Speed Potion (5)",
		"Graymold Salve (7)","Weak Power Potion (9)",
		"Potion of Clarity (9)","Strong Poison (10)","Strong Heal Potion (12)","Killer Poison (12)",
		"Resurrection Balm (9)","Medium Power Potion (14)","Knowledge Brew (19)",
		"Strong Strength (10)","Bliss (18)","Strong Power (20)"
	*/
	const std::array c_alch_names_short{
		"Weak Curing Potion","Weak Healing Potion","Weak Poison",
		"Weak Speed Potion","Medium Poison",
		"Medium Heal Potion","Strong Curing","Medium Speed Potion",
		"Graymold Salve","Weak Energy Potion",
		"Potion of Clarity","Strong Poison","Strong Heal Potion","Killer Poison",
		"Resurrection Bal","Medium Energy Ptn.","Knowledge Brew"	,
		"Strong Strength","Bliss","Strong Power"
	};
}

short spell_level(short id)
{
	return c_spell_level.at(static_cast<size_t>(id));
}

short spell_cost(short type, short id)
{
	return c_spell_cost.at(static_cast<size_t>(type)).at(static_cast<size_t>(id));
}

short mage_spell_cost(short id)
{
	return spell_cost(0, id);
}

short priest_spell_cost(short id)
{
	return spell_cost(1, id);
}

short spell_w_cast(short type, short id)
{
	return c_spell_w_cast.at(static_cast<size_t>(type)).at(static_cast<size_t>(id));
}


const char* mage_spell_name(short id)
{
	return c_mage_s_name.at(static_cast<size_t>(id));
}

const char* priest_spell_name(short id)
{
	return c_priest_s_name.at(static_cast<size_t>(id));
}

const char* alchemy_name(short id)
{
	return c_alch_names.at(static_cast<size_t>(id));
}

const char* alchemy_name_short(short id)
{
	return c_alch_names_short.at(static_cast<size_t>(id));
}
