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
			adventurer.main_status = status_type::Normal;
			REQUIRE(!cave_lore_present(adventurers));
			adventurer.main_status = status_type::Absent;
		}
	}
	{
		Adventurers adventurers{};
		for (auto& adventurer : adventurers)
		{
			adventurer.main_status = status_type::Normal;
			adventurer.traits[trait::CaveLore] = BOE_TRUE;
			REQUIRE(cave_lore_present(adventurers));
			adventurer.main_status = status_type::Absent;
			adventurer.traits[trait::CaveLore] = BOE_FALSE;
		}
	}
	{
		Adventurers adventurers{};
		for (auto& adventurer : adventurers)
		{
			adventurer.traits[trait::CaveLore] = BOE_TRUE;
			REQUIRE(!cave_lore_present(adventurers));
			adventurer.traits[trait::CaveLore] = BOE_FALSE;
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
			adventurer.main_status = status_type::Normal;
			REQUIRE(!woodsman_present(adventurers));
			adventurer.main_status = status_type::Absent;
		}
	}
	{
		Adventurers adventurers{};
		for (auto& adventurer : adventurers)
		{
			adventurer.main_status = status_type::Normal;
			adventurer.traits[trait::Woodsman] = BOE_TRUE;
			REQUIRE(woodsman_present(adventurers));
			adventurer.main_status = status_type::Absent;
			adventurer.traits[trait::Woodsman] = BOE_FALSE;
		}
	}
	{
		Adventurers adventurers{};
		for (auto& adventurer : adventurers)
		{
			adventurer.traits[trait::Woodsman] = BOE_TRUE;
			REQUIRE(!woodsman_present(adventurers));
			adventurer.traits[trait::Woodsman] = BOE_FALSE;
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
			adventurer.main_status = status_type::Normal;
			REQUIRE(mage_lore_total(adventurers) == 0);
			adventurer.main_status = status_type::Absent;
		}
	}
	{
		Adventurers adventurers{};
		for (auto& adventurer : adventurers)
		{
			adventurer.main_status = status_type::Normal;
			adventurer.skills[skill::MageLore] = 1;
			REQUIRE(mage_lore_total(adventurers) == 1);
			adventurer.main_status = status_type::Absent;
			adventurer.skills[skill::MageLore] = 0;
		}
	}
	{
		Adventurers adventurers{};
		for (auto& adventurer : adventurers) { adventurer.skills[skill::MageLore] = 1; }
		REQUIRE(mage_lore_total(adventurers) == 0);
	}
	{
		Adventurers adventurers{};
		for (auto& adventurer : adventurers) { adventurer.main_status = status_type::Normal; adventurer.skills[skill::MageLore] = 1; }
		REQUIRE(mage_lore_total(adventurers) == 6);
	}
	{
		Adventurers adventurers{};
		for (auto& adventurer : adventurers) { adventurer.main_status = status_type::Normal; adventurer.skills[skill::MageLore] = 2; }
		REQUIRE(mage_lore_total(adventurers) == 12);
	}
}

TEST_CASE("adventurers_heal", "[adventurers]")
{
	SECTION("Healing zero has no effect") {
		Adventurers adventurers{};
		for (auto& pc : adventurers) { pc.main_status = status_type::Normal; pc.cur_health = 4; pc.max_health = 8; }
		const Adventurers before{ adventurers };
		adventurers_heal(adventurers, 0);
		REQUIRE(before == adventurers);
	}
	SECTION("Healing when already at max health has no effect") {
		Adventurers adventurers{};
		for (auto& pc : adventurers) { pc.main_status = status_type::Normal; pc.cur_health = 8; pc.max_health = 8; }
		const Adventurers before{ adventurers };
		adventurers_heal(adventurers, 5);
		REQUIRE(before == adventurers);
	}
	SECTION("Healing when at more than max health has no effect") {
		Adventurers adventurers{};
		for (auto& pc : adventurers) { pc.main_status = status_type::Normal; pc.cur_health = 9; pc.max_health = 8; }
		const Adventurers before{ adventurers };
		adventurers_heal(adventurers, 1);
		REQUIRE(before == adventurers);
	}
	SECTION("Healing by two increases the amount correctly") {
		Adventurers adventurers{};
		for (auto& pc : adventurers) { pc.main_status = status_type::Normal; pc.cur_health = 4; pc.max_health = 8; }
		adventurers_heal(adventurers, 2);
		Adventurers expected{};
		for (auto& pc : expected) { pc.main_status = status_type::Normal; pc.cur_health = 6; pc.max_health = 8; }
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
		for (auto& pc : adventurers) { pc.main_status = status_type::Normal; pc.cur_health = 4; pc.max_health = 8; }
		adventurers_heal(adventurers, 6);
		Adventurers expected{};
		for (auto& pc : expected) { pc.main_status = status_type::Normal; pc.cur_health = 8; pc.max_health = 8; }
		REQUIRE(expected == adventurers);
	}
	SECTION("Healing for adventurers with different levels of health") {
		Adventurers adventurers{};
		for (size_t index = 0; index < std::size(adventurers); ++index)
		{
			auto& pc = adventurers[index];
			pc.main_status = status_type::Normal;
			pc.cur_health = static_cast<short>(4 + index);
			pc.max_health = static_cast<short>(8 + index);
		}
		adventurers_heal(adventurers, 7);
		Adventurers expected{};
		for (size_t index = 0; index < std::size(expected); ++index)
		{
			auto& pc = expected[index];
			pc.main_status = status_type::Normal;
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
		for (auto& pc : adventurers) { pc.main_status = status_type::Normal; }
		adventurers[2].gaffect(affect::Poisoned) = 4;
		REQUIRE(adventurers_cure(adventurers, 5));
		REQUIRE(adventurers[2].gaffect(affect::Poisoned) == 0);
	}
	SECTION("Curing when party members have no status has no effect") {
		Adventurers adventurers{};
		for (auto& pc : adventurers) { pc.main_status = status_type::Absent; }
		adventurers[2].gaffect(affect::Poisoned) = 4;
		const Adventurers before{ adventurers };
		REQUIRE(!adventurers_cure(adventurers, 5));
		REQUIRE(before == adventurers);
	}
}

TEST_CASE("adventurers_restore_sp", "[adventurers]")
{
	SECTION("Restoring zero spellpoints has no effect") {
		Adventurers adventurers{};
		for (auto& pc : adventurers) { pc.main_status = status_type::Normal; pc.cur_sp = 4; pc.max_sp = 8; }
		const Adventurers before{ adventurers };
		adventurers_restore_sp(adventurers, 0);
		REQUIRE(before == adventurers);
	}
	SECTION("Restoring spellpoints when already at max spellpoints has no effect") {
		Adventurers adventurers{};
		for (auto& pc : adventurers) { pc.main_status = status_type::Normal; pc.cur_sp = 8; pc.max_sp = 8; }
		const Adventurers before{ adventurers };
		adventurers_restore_sp(adventurers, 5);
		REQUIRE(before == adventurers);
	}
	SECTION("Restoring spellpoints when at more than max spellpoints has no effect") {
		Adventurers adventurers{};
		for (auto& pc : adventurers) { pc.main_status = status_type::Normal; pc.cur_sp = 9; pc.max_sp = 8; }
		const Adventurers before{ adventurers };
		adventurers_restore_sp(adventurers, 1);
		REQUIRE(before == adventurers);
	}
	SECTION("Restoring spellpoints by two increases the amount correctly") {
		Adventurers adventurers{};
		for (auto& pc : adventurers) { pc.main_status = status_type::Normal; pc.cur_sp = 4; pc.max_sp = 8; }
		adventurers_restore_sp(adventurers, 2);
		Adventurers expected{};
		for (auto& pc : expected) { pc.main_status = status_type::Normal; pc.cur_sp = 6; pc.max_sp = 8; }
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
		for (auto& pc : adventurers) { pc.main_status = status_type::Normal; pc.cur_sp = 4; pc.max_sp = 8; }
		adventurers_restore_sp(adventurers, 6);
		Adventurers expected{};
		for (auto& pc : expected) { pc.main_status = status_type::Normal; pc.cur_sp = 8; pc.max_sp = 8; }
		REQUIRE(expected == adventurers);
	}
	SECTION("Restoring spellpoints for adventurers with different levels of spellpoints") {
		Adventurers adventurers{};
		for (size_t index = 0; index < std::size(adventurers); ++index)
		{
			auto& pc = adventurers[index];
			pc.main_status = status_type::Normal;
			pc.cur_sp = static_cast<short>(4 + index);
			pc.max_sp = static_cast<short>(8 + index);
		}
		adventurers_restore_sp(adventurers, 7);
		Adventurers expected{};
		for (size_t index = 0; index < std::size(expected); ++index)
		{
			auto& pc = expected[index];
			pc.main_status = status_type::Normal;
			pc.max_sp = static_cast<short>(8 + index);
			pc.cur_sp = std::min(static_cast<short>(7 + 4 + index), pc.max_sp);
		}
		REQUIRE(expected == adventurers);
	}
}


TEST_CASE("adventurers_count_normal", "[adventurers]")
{
	{
		Adventurers adventurers{};
		REQUIRE(0 == adventurers_count_normal(adventurers));
	}
	{
		Adventurers adventurers{};
		short count = 0;
		for (auto& adventurer : adventurers)
		{
			++count;
			adventurer.main_status = status_type::Normal;
			REQUIRE(adventurers_count_normal(adventurers) == count);
		}
	}
	{
		Adventurers adventurers{};
		for (auto& adventurer : adventurers)
		{
			adventurer.main_status = status_type::Normal;
			REQUIRE(adventurers_count_normal(adventurers) == 1);
			adventurer.main_status = status_type::Absent;
		}
	}
}
