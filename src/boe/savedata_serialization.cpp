#include "boe/savedata_serialization.hpp"
#include <format>
#include <cassert>

typedef struct {
	char expl[96][96];
}	out_info_type;

boe_error::boe_error(short error_code)
	: error_code_(error_code)
	, what_(std::format("BOE ERROR #{}", error_code))
{
}

const char* boe_error::what() const noexcept
{
	return what_.c_str();
}



static void stream_read_type(std::istream& file, auto& type)
{
	assert((file.exceptions() & std::ios_base::failbit) == std::ios_base::failbit);
	file.read(reinterpret_cast<char*>(&type), sizeof(type));
}

template< typename T >
static T stream_read_type(std::istream& file)
{
	assert((file.exceptions() & std::ios_base::failbit) == std::ios_base::failbit);
	T type;
	file.read(reinterpret_cast<char*>(&type), sizeof(type));
	return type;
}

static void stream_write_type(auto& file, const auto& type)
{
	assert((file.exceptions() & std::ios_base::failbit) == std::ios_base::failbit);
	file.write(reinterpret_cast<const char*>(&type), sizeof(type));
}


static void xor_type(auto& type, char xor_value)
{
	for (size_t index = 0; index < sizeof(type); ++index)
	{
		reinterpret_cast<char*>(&type)[index] ^= xor_value;
	}
}


template< char K, typename T>
struct xor_wrap_detail
{
	T& t_;
	xor_wrap_detail(T & t)
		: t_(t)
	{
		xor_type(t_, K);
	}
	T& get() const
	{
		return t_;
	}
	~xor_wrap_detail()
	{
		xor_type(t_, K);
	}
};

template<char K, typename T>
xor_wrap_detail<K, T> xor_wrap(T& t)
{
	return xor_wrap_detail<K, T>(t);
}


static bool savedata_read_flag(std::istream& file_id, flag_type value_true, flag_type value_false)
{
	const auto flag = stream_read_type<flag_type>(file_id);
	if ((flag != value_true) && (flag != value_false)) { // OK Exile II save file?
		throw boe_error(1063);
	}
	return flag == value_true;
}


void savedata_read_all(
	std::istream& file_id,
	bool & town_restore,
	bool & in_scen,
	party_record_type & party,
	setup_save_type & setup_save,
	std::array<pc_record_type, 6> & adven,
	unsigned char (&out_e)[96][96],
	current_town_type& c_town,
	big_tr_type& t_d,
	town_item_list& t_i,
	stored_items_list_type (&stored_items)[3],
	stored_town_maps_type& town_maps,
	stored_town_maps_type& town_maps2,
	stored_outdoor_maps_type& o_maps,
	unsigned char(&sfx)[64][64],
	unsigned char (&misc_i)[64][64]
	)
{
	// Eventually this will be hidden by an interface so this kind of
	// ugliness will go away.
	if ((file_id.exceptions() & std::ios_base::failbit) != std::ios_base::failbit)
	{
		throw std::runtime_error("Internal error: called without the stream first being set to throw on failure");
	}

	town_restore = savedata_read_flag(file_id, flag_type::town, flag_type::out);
	in_scen = savedata_read_flag(file_id, flag_type::in_scenario, flag_type::not_in_scenario);
	const bool maps_there = savedata_read_flag(file_id, flag_type::have_maps, flag_type::no_maps);

	// LOAD PARTY
	stream_read_type(file_id, party);
	xor_type(party, 0x5C);

	// LOAD SETUP
	stream_read_type(file_id, setup_save);

	// LOAD PCS
	stream_read_type(file_id, adven);
	xor_type(adven, 0x6B);

	if (in_scen) {

		// LOAD OUTDOOR MAP
		static_assert(sizeof(out_info_type) == sizeof(out_e));
		stream_read_type(file_id, out_e);

		// LOAD TOWN 
		if (town_restore) {
			stream_read_type(file_id, c_town);
			stream_read_type(file_id, t_d);
			stream_read_type(file_id, t_i);
		}

		// LOAD STORED ITEMS
		stream_read_type(file_id, stored_items);

		// LOAD SAVED MAPS
		if (maps_there) {
			stream_read_type(file_id, town_maps);
			stream_read_type(file_id, town_maps2);
			stream_read_type(file_id, o_maps);
		}

		// LOAD SFX & MISC_I
		static_assert(sizeof(sfx) == 64 * 64);
		stream_read_type(file_id, sfx);
		static_assert(sizeof(misc_i) == 64 * 64);
		stream_read_type(file_id, misc_i);
	}
}


void savedata_write_all(
	std::ostream& file_id,
	bool town_save,
	bool in_startup_mode,
	bool save_maps,
	party_record_type& party,
	const setup_save_type& setup_save,
	std::array<pc_record_type, 6>& adven,
	const unsigned char(&out_e)[96][96],
	const current_town_type& c_town,
	const big_tr_type& t_d,
	const town_item_list& t_i,
	const stored_items_list_type(&stored_items)[3],
	const stored_town_maps_type& town_maps,
	const stored_town_maps_type& town_maps2,
	const stored_outdoor_maps_type& o_maps,
	const unsigned char(&sfx)[64][64],
	const unsigned char(&misc_i)[64][64]
	)
{
	stream_write_type(file_id, town_save ? flag_type::town : flag_type::out);
	stream_write_type(file_id, in_startup_mode ? flag_type::not_in_scenario : flag_type::in_scenario);
	stream_write_type(file_id, save_maps ? flag_type::have_maps : flag_type::no_maps);

	// SAVE PARTY
	stream_write_type(file_id, xor_wrap<0x5C>(party).get());

	// SAVE SETUP
	stream_write_type(file_id, setup_save);

	// SAVE PCS	
	stream_write_type(file_id, xor_wrap<0x6B>(adven).get());

	if (!in_startup_mode) {
		// SAVE OUT DATA
		static_assert(sizeof(out_info_type) == sizeof(out_e));
		stream_write_type(file_id, out_e);

		if (town_save) {
			stream_write_type(file_id, c_town);
			stream_write_type(file_id, t_d);
			stream_write_type(file_id, t_i);
		}

		// Save stored items 
		stream_write_type(file_id, stored_items);

		// If saving maps, save maps
		if (save_maps) {
			stream_write_type(file_id, town_maps);
			stream_write_type(file_id, town_maps2);
			stream_write_type(file_id, o_maps);
		}

		// SAVE SFX and MISC_I
		static_assert(sizeof(sfx) == 64 * 64);
		stream_write_type(file_id, sfx);
		static_assert(sizeof(misc_i) == 64 * 64);
		stream_write_type(file_id, misc_i);
	}
}
