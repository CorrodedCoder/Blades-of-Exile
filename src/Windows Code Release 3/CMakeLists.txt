if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	if(BOE_WARNINGS_MAX)
		add_compile_options(
			# For now these are just creating too many hits for these to be practical
			/wd4100
			/wd4244
		)
	else()
		# For now disable the following which generate hundreds of warnings:
		add_compile_options(
			/wd4244
			/wd4018
			/wd4060
			/wd4065
			/wd4267
			/wd4302
			/wd4305
			/wd4309
			/wd4311
			/wd4390
			/wd4554
			/wd4804
		)
	endif()

elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	if(BOE_WARNINGS_MAX)
	else()
		add_compile_options(
			# Disable selective warnings from -Wall
			-Wno-missing-variable-declarations
			-Wno-unused-parameter
			-Wno-old-style-cast
			-Wno-cast-align
			-Wno-unreachable-code-break
			-Wno-implicit-int-conversion
			-Wno-sign-conversion
			-Wno-unused-but-set-variable
			-Wno-conditional-uninitialized
			-Wno-shadow
			-Wno-tautological-unsigned-zero-compare
			-Wno-sign-compare
			-Wno-unreachable-code
			-Wno-shorten-64-to-32
			-Wno-tautological-value-range-compare
			-Wno-unsafe-buffer-usage
			-Wno-implicit-fallthrough
			-Wno-switch-enum

			# Replace NULL with nullptr and we can remove the following item
			-Wno-zero-as-null-pointer-constant

			# Disable warnings that come out even without -Wall
			-Wno-extern-initializer
			-Wno-char-subscripts
			-Wno-misleading-indentation
			-Wno-constant-conversion
			-Wno-sometimes-uninitialized
			-Wno-empty-body
			-Wno-parentheses
			-Wno-tautological-overlap-compare
			-Wno-tautological-constant-compare
		)
	endif()
endif()

add_executable (Blades-of-Exile WIN32
	blades.cpp
	graphics.cpp
	newgraph.cpp
	fileio.cpp
	actions.cpp
	text.cpp
	party.cpp
	exlsound.cpp
	items.cpp
	fields.cpp
	town.cpp
	startup.cpp
	dlogtool.cpp
	dlgutils.cpp
	infodlgs.cpp
	graphutl.cpp
	global.cpp
	gutils.cpp
	monster.cpp
	locutils.cpp
	itemdata.cpp
	combat.cpp
	townspec.cpp
	scenario.cpp
	scenario_ext.cpp
	specials.cpp
	
	BLADES.RC
	GAMEDLOG.RC
	GAMESTR.RC
	)

target_compile_definitions(Blades-of-Exile PRIVATE BOE_DEFAULT_VOLUME=32)

target_link_libraries(Blades-of-Exile boe Winmm.lib)


add_custom_command(
        TARGET Blades-of-Exile POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
               "${CMAKE_CURRENT_SOURCE_DIR}/Scen Ed"
               "${CMAKE_CURRENT_BINARY_DIR}/blscened")

add_custom_command(
        TARGET Blades-of-Exile POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
               "${CMAKE_SOURCE_DIR}/src/Macintosh Code Release 3/Blades of Exile Scenarios"
               "${CMAKE_CURRENT_BINARY_DIR}/bladscen")

add_subdirectory("PC Editor")
add_subdirectory("Scen Ed")
