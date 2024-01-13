if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set_directory_properties(PROPERTIES MY_VS_WARNING_LEVEL 4)
	add_compile_options(/WX)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	add_compile_options(-Wextra -Wpedantic -Werror ${BOE_WNO_UNSAFE_BUFFER_USAGE})
elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
	add_compile_options(-Wextra -Wpedantic -Werror)
endif()

add_library(boe STATIC
	adventurers.cpp
	dump_game_structures.cpp
	endian_adjust.cpp
	game_file_structures.cpp
	item.cpp
	item_source.cpp
	pc.cpp
	registration.cpp
	savedata_serialization.cpp
	scenario.cpp
	scenario_extra.cpp
	spell.cpp
	utility.cpp
)