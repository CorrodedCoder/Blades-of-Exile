#include <catch2/catch_test_macros.hpp>

#include "boe/adventurers.hpp"


TEST_CASE("cave_lore_present", "[adventurers]")
{
	{
		Adventurers adventurers{};
		REQUIRE(!cave_lore_present(adventurers));
	}
	{
		Adventurers adventurers{};
		for (auto& adventurer : adventurers)
		{
			adventurer.main_status = 1;
			REQUIRE(!cave_lore_present(adventurers));
			adventurer.main_status = 0;
		}
	}
	{
		Adventurers adventurers{};
		for (auto& adventurer : adventurers)
		{
			adventurer.main_status = 1;
			adventurer.traits[4] = 1;
			REQUIRE(cave_lore_present(adventurers));
			adventurer.main_status = 0;
			adventurer.traits[4] = 0;
		}
	}
	{
		Adventurers adventurers{};
		for (auto& adventurer : adventurers)
		{
			adventurer.traits[4] = 1;
			REQUIRE(!cave_lore_present(adventurers));
			adventurer.traits[4] = 0;
		}
	}
}

TEST_CASE("woodsman_present", "[adventurers]")
{
	{
		Adventurers adventurers{};
		REQUIRE(!woodsman_present(adventurers));
	}
	{
		Adventurers adventurers{};
		for (auto& adventurer : adventurers)
		{
			adventurer.main_status = 1;
			REQUIRE(!woodsman_present(adventurers));
			adventurer.main_status = 0;
		}
	}
	{
		Adventurers adventurers{};
		for (auto& adventurer : adventurers)
		{
			adventurer.main_status = 1;
			adventurer.traits[5] = 1;
			REQUIRE(woodsman_present(adventurers));
			adventurer.main_status = 0;
			adventurer.traits[5] = 0;
		}
	}
	{
		Adventurers adventurers{};
		for (auto& adventurer : adventurers)
		{
			adventurer.traits[5] = 1;
			REQUIRE(!woodsman_present(adventurers));
			adventurer.traits[5] = 0;
		}
	}
}

TEST_CASE("mage_lore_total", "[adventurers]")
{
	{
		Adventurers adventurers{};
		REQUIRE(mage_lore_total(adventurers) == 0);
	}
	{
		Adventurers adventurers{};
		for (auto& adventurer : adventurers)
		{
			adventurer.main_status = 1;
			REQUIRE(mage_lore_total(adventurers) == 0);
			adventurer.main_status = 0;
		}
	}
	{
		Adventurers adventurers{};
		for (auto& adventurer : adventurers)
		{
			adventurer.main_status = 1;
			adventurer.skills[11] = 1;
			REQUIRE(mage_lore_total(adventurers) == 1);
			adventurer.main_status = 0;
			adventurer.skills[11] = 0;
		}
	}
	{
		Adventurers adventurers{};
		for (auto& adventurer : adventurers) { adventurer.skills[11] = 1; }
		REQUIRE(mage_lore_total(adventurers) == 0);
	}
	{
		Adventurers adventurers{};
		for (auto& adventurer : adventurers) { adventurer.main_status = 1; adventurer.skills[11] = 1; }
		REQUIRE(mage_lore_total(adventurers) == 6);
	}
	{
		Adventurers adventurers{};
		for (auto& adventurer : adventurers) { adventurer.main_status = 1; adventurer.skills[11] = 2; }
		REQUIRE(mage_lore_total(adventurers) == 12);
	}
}

TEST_CASE("adventurers_heal", "[adventurers]")
{
	SECTION("Healing zero has no effect") {
		Adventurers adventurers{};
		for (auto& pc : adventurers) { pc.main_status = 1; pc.cur_health = 4; pc.max_health = 8; }
		const Adventurers before{ adventurers };
		adventurers_heal(adventurers, 0);
		REQUIRE(before == adventurers);
	}
	SECTION("Healing when already at max health has no effect") {
		Adventurers adventurers{};
		for (auto& pc : adventurers) { pc.main_status = 1; pc.cur_health = 8; pc.max_health = 8; }
		const Adventurers before{ adventurers };
		adventurers_heal(adventurers, 5);
		REQUIRE(before == adventurers);
	}
	SECTION("Healing when at more than max health has no effect") {
		Adventurers adventurers{};
		for (auto& pc : adventurers) { pc.main_status = 1; pc.cur_health = 9; pc.max_health = 8; }
		const Adventurers before{ adventurers };
		adventurers_heal(adventurers, 1);
		REQUIRE(before == adventurers);
	}
	SECTION("Healing by two increases the amount correctly") {
		Adventurers adventurers{};
		for (auto& pc : adventurers) { pc.main_status = 1; pc.cur_health = 4; pc.max_health = 8; }
		adventurers_heal(adventurers, 2);
		Adventurers expected{};
		for (auto& pc : expected) { pc.main_status = 1; pc.cur_health = 6; pc.max_health = 8; }
		REQUIRE(expected == adventurers);
	}
	SECTION("Healing by two does not increases the amount if status is not 1") {
		Adventurers adventurers{};
		for (auto& pc : adventurers) { pc.cur_health = 4; pc.max_health = 8; }
		adventurers_heal(adventurers, 2);
		Adventurers expected{};
		for (auto& pc : expected) { pc.cur_health = 4; pc.max_health = 8; }
		REQUIRE(expected == adventurers);
	}
	SECTION("Healing by more than max increases the amount only up to max") {
		Adventurers adventurers{};
		for (auto& pc : adventurers) { pc.main_status = 1; pc.cur_health = 4; pc.max_health = 8; }
		adventurers_heal(adventurers, 6);
		Adventurers expected{};
		for (auto& pc : expected) { pc.main_status = 1; pc.cur_health = 8; pc.max_health = 8; }
		REQUIRE(expected == adventurers);
	}
	SECTION("Healing for adventurers with different levels of health") {
		Adventurers adventurers{};
		for (size_t index = 0; index < std::size(adventurers); ++index)
		{
			auto& pc = adventurers[index];
			pc.main_status = 1;
			pc.cur_health = static_cast<short>(4 + index);
			pc.max_health = static_cast<short>(8 + index);
		}
		adventurers_heal(adventurers, 7);
		Adventurers expected{};
		for (size_t index = 0; index < std::size(expected); ++index)
		{
			auto& pc = expected[index];
			pc.main_status = 1;
			pc.max_health = static_cast<short>(8 + index);
			pc.cur_health = std::min(static_cast<short>(7 + 4 + index), pc.max_health);
		}
		REQUIRE(expected == adventurers);
	}
}

TEST_CASE("adventurers_cure", "[adventurers]")
{
	SECTION("Curing when only one person in the party needs it") {
		Adventurers adventurers{};
		for (auto& pc : adventurers) { pc.main_status = 1; }
		adventurers[2].status[2] = 4;
		REQUIRE(adventurers_cure(adventurers, 5));
		REQUIRE(adventurers[2].status[2] == 0);
	}
	SECTION("Curing when party members have no status has no effect") {
		Adventurers adventurers{};
		for (auto& pc : adventurers) { pc.main_status = 0; }
		adventurers[2].status[2] = 4;
		const Adventurers before{ adventurers };
		REQUIRE(!adventurers_cure(adventurers, 5));
		REQUIRE(before == adventurers);
	}
}

TEST_CASE("adventurers_restore_sp", "[adventurers]")
{
	SECTION("Restoring zero spellpoints has no effect") {
		Adventurers adventurers{};
		for (auto& pc : adventurers) { pc.main_status = 1; pc.cur_sp = 4; pc.max_sp = 8; }
		const Adventurers before{ adventurers };
		adventurers_restore_sp(adventurers, 0);
		REQUIRE(before == adventurers);
	}
	SECTION("Restoring spellpoints when already at max spellpoints has no effect") {
		Adventurers adventurers{};
		for (auto& pc : adventurers) { pc.main_status = 1; pc.cur_sp = 8; pc.max_sp = 8; }
		const Adventurers before{ adventurers };
		adventurers_restore_sp(adventurers, 5);
		REQUIRE(before == adventurers);
	}
	SECTION("Restoring spellpoints when at more than max spellpoints has no effect") {
		Adventurers adventurers{};
		for (auto& pc : adventurers) { pc.main_status = 1; pc.cur_sp = 9; pc.max_sp = 8; }
		const Adventurers before{ adventurers };
		adventurers_restore_sp(adventurers, 1);
		REQUIRE(before == adventurers);
	}
	SECTION("Restoring spellpoints by two increases the amount correctly") {
		Adventurers adventurers{};
		for (auto& pc : adventurers) { pc.main_status = 1; pc.cur_sp = 4; pc.max_sp = 8; }
		adventurers_restore_sp(adventurers, 2);
		Adventurers expected{};
		for (auto& pc : expected) { pc.main_status = 1; pc.cur_sp = 6; pc.max_sp = 8; }
		REQUIRE(expected == adventurers);
	}
	SECTION("Restoring spellpoints by two does not increases the amount if status is not 1") {
		Adventurers adventurers{};
		for (auto& pc : adventurers) { pc.cur_sp = 4; pc.max_sp = 8; }
		adventurers_restore_sp(adventurers, 2);
		Adventurers expected{};
		for (auto& pc : expected) { pc.cur_sp = 4; pc.max_sp = 8; }
		REQUIRE(expected == adventurers);
	}
	SECTION("Restoring spellpoints by more than max increases the amount only up to max") {
		Adventurers adventurers{};
		for (auto& pc : adventurers) { pc.main_status = 1; pc.cur_sp = 4; pc.max_sp = 8; }
		adventurers_restore_sp(adventurers, 6);
		Adventurers expected{};
		for (auto& pc : expected) { pc.main_status = 1; pc.cur_sp = 8; pc.max_sp = 8; }
		REQUIRE(expected == adventurers);
	}
	SECTION("Restoring spellpoints for adventurers with different levels of spellpoints") {
		Adventurers adventurers{};
		for (size_t index = 0; index < std::size(adventurers); ++index)
		{
			auto& pc = adventurers[index];
			pc.main_status = 1;
			pc.cur_sp = static_cast<short>(4 + index);
			pc.max_sp = static_cast<short>(8 + index);
		}
		adventurers_restore_sp(adventurers, 7);
		Adventurers expected{};
		for (size_t index = 0; index < std::size(expected); ++index)
		{
			auto& pc = expected[index];
			pc.main_status = 1;
			pc.max_sp = static_cast<short>(8 + index);
			pc.cur_sp = std::min(static_cast<short>(7 + 4 + index), pc.max_sp);
		}
		REQUIRE(expected == adventurers);
	}
}
