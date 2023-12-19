if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set_directory_properties(PROPERTIES MY_VS_WARNING_LEVEL 4)
	add_compile_options(/WX)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	add_compile_options(
		-Wextra -Wpedantic -Werror
		# Disable selective warnings from -Wall
		# These should be addressed ASAP
		-Wno-old-style-cast
		${BOE_WNO_UNSAFE_BUFFER_USAGE}
	)
endif()

add_subdirectory("Dump Scenario")
add_subdirectory("Dump Savegame")
