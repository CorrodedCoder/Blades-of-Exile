#pragma once

#include "game_file_structures.hpp"

#include <string>
#include <ostream>

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
