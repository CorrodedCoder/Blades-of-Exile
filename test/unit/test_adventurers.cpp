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
