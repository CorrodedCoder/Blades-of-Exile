#include "dump_scenario.hpp"
#include "../dump_game_structures.hpp"
#include "../endian_adjust.hpp"

#include <format>
#include <vector>
#include <bit>

bool dump_scenario(std::ostream_iterator<char> strm, std::istream & input)
{
	std::format_to(strm, "scenario_data_type: {:x} {:d}\n", sizeof(scenario_data_type), sizeof(scenario_data_type));
	std::format_to(strm, "scen_item_data_type: {:x} {:d}\n", sizeof(scen_item_data_type), sizeof(scen_item_data_type));
	std::format_to(strm, "scenario_both: {:x} {:d}\n", sizeof(scenario_data_type) + sizeof(scen_item_data_type), sizeof(scenario_data_type) + sizeof(scen_item_data_type));
	std::format_to(strm, "outdoor_record_type: {:x} {:d}\n", sizeof(outdoor_record_type), sizeof(outdoor_record_type));
	std::endian scenario_endian{ std::endian::native };
	scenario_data_type scenario;
	input.read(reinterpret_cast<char*>(&scenario), sizeof(scenario));
	if ((scenario.flag1 == 10) && (scenario.flag2 == 20)
		&& (scenario.flag3 == 30)
		&& (scenario.flag4 == 40))
	{
		std::format_to(strm, "scenario is big endian\n");
		scenario_endian = std::endian::big;
	}
	else if ((scenario.flag1 == 20) && (scenario.flag2 == 40)
		&& (scenario.flag3 == 60)
		&& (scenario.flag4 == 80))
	{
		std::format_to(strm, "scenario is little endian\n");
		scenario_endian = std::endian::little;
	}
	else
	{
		std::format_to(strm, "scenario flags not recognised");
		return false;
	}
	if (std::endian::native != scenario_endian)
	{
		endian_adjust(scenario);
	}
	dump_to(strm, "", "scenario_data_type", scenario);

	scen_item_data_type scenario_items;
	input.read(reinterpret_cast<char*>(&scenario_items), sizeof(scenario_items));
	if (std::endian::native != scenario_endian)
	{
		endian_adjust(scenario_items);
	}
	dump_to(strm, "", "scen_item_data_type", scenario_items);

	// Note that the original code only checks for 270
	std::vector<char> storage;
	for (size_t index = 0; index < 300; ++index)
	{
		const auto len = scenario.scen_str_len[index];
		storage.clear();
		storage.resize(len + 1, '\0');
		input.read(storage.data(), len);
		std::format_to(strm, "scen_str[{}]: ({}) \"{}\"\n", index, len, storage.data());
	}

	for (size_t outdoors_index = 0; outdoors_index < (scenario.out_width * scenario.out_height); ++outdoors_index)
	{
		std::format_to(strm, "Start position: {} {:x}\n", outdoors_index, static_cast<std::size_t>(input.tellg()));
		outdoor_record_type outdoors;
		input.read(reinterpret_cast<char*>(&outdoors), sizeof(outdoors));
		if (std::endian::native != scenario_endian)
		{
			endian_adjust(outdoors);
		}
		dump_to(strm, "", "outdoor_record_type", outdoors);
		const auto string_start_pos{ input.tellg() };
		std::vector<char> storage;
		for (size_t index = 0; index < std::size(outdoors.strlens); ++index)
		{
			const auto len = outdoors.strlens[index];
			storage.clear();
			storage.resize(len + 1, '\0');
			std::format_to(strm, "Current position: {:x}\n", static_cast<std::size_t>(input.tellg()));
			input.read(storage.data(), (unsigned int)len);
			std::format_to(strm, "scen_str[{}]: ({}) \"{}\"\n", index, len, storage.data());
		}
		const auto string_end_pos{ input.tellg() };
		std::format_to(strm, "String section size: {} {:x}\n", static_cast<std::size_t>(string_end_pos - string_start_pos), static_cast<std::size_t>(string_end_pos - string_start_pos));
		std::format_to(strm, "scenario.out_data_size: {} {:x}\n", scenario.out_data_size[outdoors_index][1], scenario.out_data_size[outdoors_index][1]);
		std::format_to(strm, "End position: {} {:x}\n", outdoors_index, static_cast<std::size_t>(string_end_pos));
	}
	std::format_to(strm, "Final position: {:x}\n", static_cast<std::size_t>(input.tellg()));
	return true;
}
