if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set_directory_properties(PROPERTIES MY_VS_WARNING_LEVEL 4)
	add_compile_options(/WX)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	include(CheckCXXCompilerFlag)
	check_cxx_compiler_flag("-Wno-unsafe-buffer-usage" BOE_FLAG_HAS_NO_UNSAFE_BUFFER_USAGE)
	add_compile_options(
		-Wextra -Wpedantic -Werror
		# Disable selective warnings from -Wall
		# These should be addressed ASAP
		-Wno-old-style-cast
	)
	if(BOE_FLAG_HAS_NO_UNSAFE_BUFFER_USAGE)
		add_compile_options(-Wno-unsafe-buffer-usage)
	endif()
endif()

add_subdirectory("Dump Scenario")
add_subdirectory("Dump Savegame")
