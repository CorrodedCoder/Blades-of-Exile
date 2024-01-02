if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set_directory_properties(PROPERTIES MY_VS_WARNING_LEVEL 4)
	add_compile_options(/WX)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	add_compile_options(
		-Wextra -Wpedantic -Werror ${BOE_WNO_UNSAFE_BUFFER_USAGE}
	)
endif()

add_library(boe STATIC
	adventurers.cpp
	dump_game_structures.cpp
	endian_adjust.cpp
	item.cpp
	pc.cpp
	savedata_serialization.cpp
	scenario.cpp
	utility.cpp
)