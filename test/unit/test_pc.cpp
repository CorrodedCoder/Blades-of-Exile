#include <catch2/catch_test_macros.hpp>
#include <algorithm>

#include "boe/pc.hpp"

TEST_CASE("heal_pc", "[pc]")
{
	SECTION("Healing zero has no effect") {
		pc_record_type pc{};
		pc.main_status = 1; pc.cur_health = 4; pc.max_health = 8;
		const pc_record_type before{ pc };
		heal_pc(pc, 0);
		REQUIRE(before == pc);
	}
	SECTION("Healing when already at max health has no effect") {
		pc_record_type pc{};
		pc.main_status = 1; pc.cur_health = 8; pc.max_health = 8;
		const pc_record_type before{ pc };
		heal_pc(pc, 5);
		REQUIRE(before == pc);
	}
	SECTION("Healing when at more than max health has no effect") {
		pc_record_type pc{};
		pc.main_status = 1; pc.cur_health = 9; pc.max_health = 8;
		const pc_record_type before{ pc };
		heal_pc(pc, 1);
		REQUIRE(before == pc);
	}
	SECTION("Healing by two increases the amount correctly") {
		pc_record_type pc{};
		pc.main_status = 1; pc.cur_health = 4; pc.max_health = 8;
		heal_pc(pc, 2);
		pc_record_type expected{};
		expected.main_status = 1; expected.cur_health = 6; expected.max_health = 8;
		REQUIRE(expected == pc);
	}
	SECTION("Healing by two does not increases the amount if status is not 1") {
		pc_record_type pc{};
		pc.cur_health = 4; pc.max_health = 8;
		heal_pc(pc, 2);
		pc_record_type expected{};
		expected.cur_health = 4; expected.max_health = 8;
		REQUIRE(expected == pc);
	}
	SECTION("Healing by more than max increases the amount only up to max") {
		pc_record_type pc{};
		pc.main_status = 1; pc.cur_health = 4; pc.max_health = 8;
		heal_pc(pc, 6);
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
			heal_pc(pc, 7);
			pc_record_type expected{};
			expected.main_status = 1;
			expected.max_health = static_cast<short>(8 + index);
			expected.cur_health = std::min(static_cast<short>(7 + 4 + index), pc.max_health);
			REQUIRE(expected == pc);
		}
	}
}
