#pragma once

#include "game_file_structures.hpp"

#include <string>
#include <ostream>
#include <iterator>
#include <format>

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const char& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const unsigned char& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const short& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const int& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const unsigned int& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const long& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const unsigned long& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const char* t, size_t size);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const unsigned char* t, size_t size);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const status& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const location& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const out_wandering_type& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const outdoor_record_type& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const item_record_type& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const scen_item_data_type& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const BoeRect& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const horse_record_type& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const boat_record_type& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const special_node_type& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const item_storage_shortcut_type& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const terrain_type_type& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const monster_record_type& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const scenario_data_type& t);

void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const talk_save_type& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const outdoor_creature_type& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const creature_start_type& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const creature_data_type& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const creature_list_type& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const party_record_type& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const setup_save_type& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const pc_record_type& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const preset_item_type& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const preset_field_type& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const wandering_type& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const town_record_type& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const current_town_type& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const big_tr_type& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const town_item_list& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const stored_items_list_type& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const stored_town_maps_type& t);
void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const stored_outdoor_maps_type& t);

template< typename T, size_t N >
requires std::is_same<T, char>::value
static inline void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const T(&arr)[N])
{
	dump_to(strm, parent, item, arr, N);
}

template< typename T, size_t N >
	requires std::is_same<T, char>::value
static inline void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const std::array<T, N>& arr)
{
	dump_to(strm, parent, item, arr, N);
}

template< typename T, size_t N >
	requires std::is_same<T, unsigned char>::value
static inline void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const T(&arr)[N])
{
	dump_to(strm, parent, item, arr, N);
}

template< typename T, size_t N >
	requires std::is_same<T, unsigned char>::value
static inline void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const std::array<T, N>& arr)
{
	dump_to(strm, parent, item, arr, N);
}

template< typename T, size_t N >
static inline void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const T(&arr)[N])
{
	for (size_t index = 0; index < N; ++index)
	{
		dump_to(strm, parent, std::format("{}[{}]", item, index), arr[index]);
	}
}

template< typename T, size_t N >
static inline void dump_to(std::ostream_iterator<char>& strm, std::string_view parent, std::string_view item, const std::array<T, N>& arr)
{
	for (size_t index = 0; index < N; ++index)
	{
		dump_to(strm, parent, std::format("{}[{}]", item, index), arr[index]);
	}
}

