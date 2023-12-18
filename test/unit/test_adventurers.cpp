#include <catch2/catch_test_macros.hpp>


#include "boe/adventurers.hpp"


TEST_CASE("Cave lore present", "[adventurers]")
{
	Adventurers adventurers{};
	REQUIRE(!cave_lore_present(adventurers));
	for (auto& adventurer : adventurers) { adventurer.main_status = 1; }
	REQUIRE(!cave_lore_present(adventurers));
	for (auto& adventurer : adventurers) { adventurer.main_status = 1; adventurer.traits[4] = 1; }
	REQUIRE(cave_lore_present(adventurers));
	for (auto& adventurer : adventurers) { adventurer.main_status = 0; }
	REQUIRE(!cave_lore_present(adventurers));
}
