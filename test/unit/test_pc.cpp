#include <catch2/catch_test_macros.hpp>
#include <algorithm>

#include "boe/pc.hpp"


TEST_CASE("pc_heal", "[pc]")
{
	SECTION("Healing zero has no effect") {
		pc_record_type pc{};
		pc.main_status = 1; pc.cur_health = 4; pc.max_health = 8;
		const pc_record_type before{ pc };
		pc_heal(pc, 0);
		REQUIRE(before == pc);
	}
	SECTION("Healing when already at max health has no effect") {
		pc_record_type pc{};
		pc.main_status = 1; pc.cur_health = 8; pc.max_health = 8;
		const pc_record_type before{ pc };
		pc_heal(pc, 5);
		REQUIRE(before == pc);
	}
	SECTION("Healing when at more than max health has no effect") {
		pc_record_type pc{};
		pc.main_status = 1; pc.cur_health = 9; pc.max_health = 8;
		const pc_record_type before{ pc };
		pc_heal(pc, 1);
		REQUIRE(before == pc);
	}
	SECTION("Healing by two increases the amount correctly") {
		pc_record_type pc{};
		pc.main_status = 1; pc.cur_health = 4; pc.max_health = 8;
		pc_heal(pc, 2);
		pc_record_type expected{};
		expected.main_status = 1; expected.cur_health = 6; expected.max_health = 8;
		REQUIRE(expected == pc);
	}
	SECTION("Healing by two does not increases the amount if status is not 1") {
		pc_record_type pc{};
		pc.cur_health = 4; pc.max_health = 8;
		pc_heal(pc, 2);
		pc_record_type expected{};
		expected.cur_health = 4; expected.max_health = 8;
		REQUIRE(expected == pc);
	}
	SECTION("Healing by more than max increases the amount only up to max") {
		pc_record_type pc{};
		pc.main_status = 1; pc.cur_health = 4; pc.max_health = 8;
		pc_heal(pc, 6);
		pc_record_type expected{};
		expected.main_status = 1; expected.cur_health = 8; expected.max_health = 8;
		REQUIRE(expected == pc);
	}
	SECTION("Healing for pc with different levels of health") {
		for (size_t index = 0; index < 6; ++index)
		{
			pc_record_type pc{};
			pc.main_status = 1;
			pc.cur_health = static_cast<short>(4 + index);
			pc.max_health = static_cast<short>(8 + index);
			pc_heal(pc, 7);
			pc_record_type expected{};
			expected.main_status = 1;
			expected.max_health = static_cast<short>(8 + index);
			expected.cur_health = std::min(static_cast<short>(7 + 4 + index), pc.max_health);
			REQUIRE(expected == pc);
		}
	}
}

TEST_CASE("pc_cure", "[pc]")
{
	SECTION("Curing zero has no effect") {
		pc_record_type pc{};
		pc.main_status = 1; pc.status[2] = 2;
		const pc_record_type before{ pc };
		// Existing logic returns true on zero change
		REQUIRE(pc_cure(pc, 0));
		REQUIRE(before == pc);
	}
	SECTION("Curing when already at zero has no effect") {
		pc_record_type pc{};
		pc.main_status = 1; pc.status[2] = 0;
		const pc_record_type before{ pc };
		// Existing logic returns true on zero change
		REQUIRE(pc_cure(pc, 5));
		REQUIRE(before == pc);
	}
	SECTION("Curing by two decreases the amount correctly") {
		pc_record_type pc{};
		pc.main_status = 1; pc.status[2] = 5;
		REQUIRE(pc_cure(pc, 2));
		pc_record_type expected{};
		expected.main_status = 1; expected.status[2] = 3;
		REQUIRE(expected == pc);
	}
	SECTION("Curing by two does not decrease the amount if status is not 1") {
		pc_record_type pc{};
		pc.main_status = 0; pc.status[2] = 5;
		pc_record_type before{pc};
		REQUIRE(!pc_cure(pc, 2));
		REQUIRE(before == pc);
	}
	SECTION("Curing by more than total decreases the amount only down to zero") {
		pc_record_type pc{};
		pc.main_status = 1; pc.status[2] = 5;
		REQUIRE(pc_cure(pc, 9));
		pc_record_type expected{};
		expected.main_status = 1; expected.status[2] = 0;
		REQUIRE(expected == pc);
	}
	SECTION("Curing for pc with different levels of illness") {
		for (size_t index = 0; index < 6; ++index)
		{
			pc_record_type pc{};
			pc.main_status = 1;
			pc.status[2] = static_cast<short>(4 + index);
			REQUIRE(pc_cure(pc, 7));
			pc_record_type expected{};
			expected.main_status = 1;
			expected.status[2] = std::max(static_cast<short>(4 + index - 7), static_cast<short>(0));
			REQUIRE(expected == pc);
		}
	}
}

TEST_CASE("pc_restore_sp", "[pc]")
{
	SECTION("Restoring zero spellpoints has no effect") {
		pc_record_type pc{};
		pc.main_status = 1; pc.cur_sp = 4; pc.max_sp = 8;
		const pc_record_type before{ pc };
		pc_restore_sp(pc, 0);
		REQUIRE(before == pc);
	}
	SECTION("Restoring spellpoints when already at max spellpoints has no effect") {
		pc_record_type pc{};
		pc.main_status = 1; pc.cur_sp = 8; pc.max_sp = 8;
		const pc_record_type before{ pc };
		pc_restore_sp(pc, 5);
		REQUIRE(before == pc);
	}
	SECTION("Restoring spellpoints when at more than max spellpoints has no effect") {
		pc_record_type pc{};
		pc.main_status = 1; pc.cur_sp = 9; pc.max_sp = 8;
		const pc_record_type before{ pc };
		pc_restore_sp(pc, 1);
		REQUIRE(before == pc);
	}
	SECTION("Restoring spellpoints by two increases the amount correctly") {
		pc_record_type pc{};
		pc.main_status = 1; pc.cur_sp = 4; pc.max_sp = 8;
		pc_restore_sp(pc, 2);
		pc_record_type expected{};
		expected.main_status = 1; expected.cur_sp = 6; expected.max_sp = 8;
		REQUIRE(expected == pc);
	}
	SECTION("Restoring spellpoints by two does not increases the amount even if status is not 1") {
		pc_record_type pc{};
		pc.cur_sp = 4; pc.max_sp = 8;
		pc_restore_sp(pc, 2);
		pc_record_type expected{};
		expected.cur_sp = 6; expected.max_sp = 8;
		REQUIRE(expected == pc);
	}
	SECTION("Restoring spellpoints by more than max increases the amount only up to max") {
		pc_record_type pc{};
		pc.main_status = 1; pc.cur_sp = 4; pc.max_sp = 8;
		pc_restore_sp(pc, 6);
		pc_record_type expected{};
		expected.main_status = 1; expected.cur_sp = 8; expected.max_sp = 8;
		REQUIRE(expected == pc);
	}
	SECTION("Restoring spellpoints for pc with different levels of spellpoints") {
		for (size_t index = 0; index < 6; ++index)
		{
			pc_record_type pc{};
			pc.main_status = 1;
			pc.cur_sp = static_cast<short>(4 + index);
			pc.max_sp = static_cast<short>(8 + index);
			pc_restore_sp(pc, 7);
			pc_record_type expected{};
			expected.main_status = 1;
			expected.max_sp = static_cast<short>(8 + index);
			expected.cur_sp = std::min(static_cast<short>(7 + 4 + index), pc.max_sp);
			REQUIRE(expected == pc);
		}
	}
}

const pc_record_type c_pc_blank{
	.name={'\n'},
	.skills = {1, 1, 1},
	.max_health = 6,
	.cur_health = 6,
	.skill_pts = 60,
	.level = 1,
	.priest_spells = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	.mage_spells = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	.weap_poisoned = 24,
	.exp_adj = 100,
};

const std::array c_debug_names{
	"Gunther",
	"Yanni",
	"Mandolin",
	"Pete",
	"Vraiment",
	"Goo"
};

const pc_record_type c_pc_debug{
	.main_status = 1,
	.skills = {20,20,20,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8},
	.max_health = 60,
	.cur_health = 60,
	.max_sp = 90,
	.cur_sp = 90,
	.skill_pts = 60,
	.level = 1,
	.priest_spells = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	.mage_spells = { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	.weap_poisoned = 16,
	.exp_adj = 100,
};


const std::array<pc_record_type, 6> c_pc_prefab{ {
	{
		.main_status = 1,
		.name = "Jenneke",
		.skills = {8, 6, 2, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2},
		.max_health = 22,
		.cur_health = 22,
		.level = 1,
		.priest_spells = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		.mage_spells = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		.which_graphic = 3,
		.traits = {0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0},
		.exp_adj = 100,
	},
	{
		.main_status = 1,
		.name = "Thissa",
		.skills = {8,7,2, 0,0,6,3,0,3, 0,0,0,0,0, 0,0,0,2,0},
		.max_health = 24,
		.cur_health = 24,
		.level = 1,
		.priest_spells = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		.mage_spells = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		.which_graphic = 32,
		.traits = {1,0,0,0,0,1,0,0,0,0, 1,0,0,0,0},
		.race = 2,
		.exp_adj = 100,
	},
	{
		.main_status = 1,
		.name = "Frrrrrr",
		.skills = {8,6,2, 3,3,0,0,2,0, 0,0,0,0,0, 4,4,0,2,1},
		.max_health = 24,
		.cur_health = 24,
		.level = 1,
		.priest_spells = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		.mage_spells = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		.which_graphic = 29,
		.traits = {0,0,0,1,0,0,0,0,0,0, 0,0,1,0,0},
		.race = 1,
		.exp_adj = 100,
	},
	{
		.main_status = 1,
		.name = "Adrianna",
		.skills = {3,2,6, 2,0,0,2,0,0, 3,0,3,0,1, 0,0,0,0,0},
		.max_health = 16,
		.cur_health = 16,
		.max_sp = 20,
		.cur_sp = 20,
		.level = 1,
		.priest_spells = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		.mage_spells = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		.which_graphic = 16,
		.traits = {0,1,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		.race = 0,
		.exp_adj = 100,
	},
	{
		.main_status = 1,
		.name = "Feodoric",
		.skills = {2,2,6, 3,0,0,2,0,0,  2,1,4,0,0, 0,0,0,0,1},
		.max_health = 16,
		.cur_health = 16,
		.max_sp = 20,
		.cur_sp = 20,
		.level = 1,
		.priest_spells = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		.mage_spells = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		.which_graphic = 23,
		.traits = {0,0,0,0,1,0,1,1,0,0, 0,0,0,0,1},
		.race = 0,
		.exp_adj = 100,
	},
	{
		.main_status = 1,
		.name = "Michael",
		.skills = {2,2,6, 0,2,0,2,0,1, 0,3,3,2,0, 0,0,0,0,0},
		.max_health = 18,
		.cur_health = 18,
		.max_sp = 21,
		.cur_sp = 21,
		.level = 1,
		.priest_spells = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		.mage_spells = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		.which_graphic = 14,
		.traits = {0,1,0,0,0,0,0,0,0,0, 0,0,0,0,0},
		.race = 0,
		.exp_adj = 100,
	},
} };


TEST_CASE("pc_setup_blank", "[pc]")
{
	pc_record_type pc{};
	pc_setup_blank(pc);
	REQUIRE(c_pc_blank == pc);
}


TEST_CASE("pc_setup_debug", "[pc]")
{
	for (size_t index = 0; index < 6; ++index)
	{
		INFO("Checking debug at index: " << index);
		pc_record_type pc{};
		pc_setup_debug(pc, static_cast<short>(index));
		pc_record_type expected{ c_pc_debug };
		std::ranges::copy(std::string_view(c_debug_names.at(index)), expected.name);
		expected.which_graphic = static_cast<short>(index + 4);
		REQUIRE(expected == pc);
	}
}


TEST_CASE("pc_setup_prefab", "[pc]")
{
	for( size_t index = 0; index < std::size(c_pc_prefab); ++index )
	{
		INFO("Checking prefab at index: " << index);
		pc_record_type pc{};
		pc_setup_prefab(pc, static_cast<short>(index));
		REQUIRE(c_pc_prefab[index] == pc);
	}
}

/*
short pc_encumberance(const pc_record_type& pc)
{
	short store = 0, i, what_val;

	for (i = 0; i < 24; i++)
		if (pc.equip[i] == BOE_TRUE) {
			what_val = pc.items[i].awkward;
			store += what_val;
		}
	return store;
}
*/

TEST_CASE("pc_encumberance", "[pc]")
{
	SECTION("No encumberance") {
		pc_record_type pc{};
		REQUIRE(pc_encumberance(pc) == 0);
	}
	SECTION("Awkward items not equipped should have no encumberance") {
		pc_record_type pc{};
		for (auto& item : pc.items) { item.awkward = 1; }
		REQUIRE(pc_encumberance(pc) == 0);
	}
	SECTION("Equipped non awkward items should have no encumberance") {
		pc_record_type pc{};
		for (auto& equipped : pc.equip) { equipped = BOE_TRUE; }
		REQUIRE(pc_encumberance(pc) == 0);
	}
	SECTION("Equipped awkward items of should have encumberance") {
		pc_record_type pc{};
		for (auto& equipped : pc.equip) { equipped = BOE_TRUE; }
		for (size_t index = 0; index < std::size(pc.items); ++index)
		{
			INFO("Checking at index: " << index);
			pc.items[index].awkward = 1;
			CHECK(pc_encumberance(pc) == static_cast<short>(index + 1));
		}
	}
	SECTION("Equipped awkward items of different values should have encumberance") {
		pc_record_type pc{};
		pc.equip[0] = BOE_TRUE; pc.items[0].awkward = 1;
		REQUIRE(pc_encumberance(pc) == 1);
		pc.equip[4] = BOE_TRUE; pc.items[4].awkward = 3;
		REQUIRE(pc_encumberance(pc) == 4);
		pc.equip[7] = BOE_FALSE; pc.items[7].awkward = 5;
		REQUIRE(pc_encumberance(pc) == 4);
		pc.equip[9] = BOE_TRUE; pc.items[9].awkward = 0;
		REQUIRE(pc_encumberance(pc) == 4);
		pc.equip[11] = BOE_TRUE; pc.items[11].awkward = 4;
		REQUIRE(pc_encumberance(pc) == 8);
		pc.equip[11] = BOE_FALSE; pc.items[11].awkward = 4;
		REQUIRE(pc_encumberance(pc) == 4);
	}
}

const std::array<std::array<short, 15>, 3> c_tnl_single_trait{ {
	{0x6e, 0x78, 0x6c, 0x6e, 0x68, 0x6a, 0x6e, 0x6b, 0x70, 0x73, 0x5a, 0x5c, 0x5c, 0x50, 0x5c,},
	{0x7b, 0x86, 0x78, 0x7b, 0x74, 0x76, 0x7b, 0x77, 0x7d, 0x80, 0x64, 0x67, 0x67, 0x59, 0x67,},
	{0x84, 0x90, 0x81, 0x84, 0x7c, 0x7f, 0x84, 0x80, 0x86, 0x8a, 0x6c, 0x6e, 0x6e, 0x60, 0x6e,},
} };

TEST_CASE("pc_get_tnl", "[pc]")
{
	SECTION("Bulk tests") {
		pc_record_type pc{};
		for (short race = 0; race < 3; ++race)
		{
			pc.race = race;
			for (size_t trait = 0; trait < std::size(pc.traits); ++trait)
			{
				pc.traits[trait] = BOE_TRUE;
				REQUIRE(pc_get_tnl(pc) == c_tnl_single_trait.at(race).at(trait));
				pc.traits[trait] = BOE_FALSE;
			}
		}
	}
}
