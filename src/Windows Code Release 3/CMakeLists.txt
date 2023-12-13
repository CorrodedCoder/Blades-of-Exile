
if (MSVC)

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
	specials.cpp
	endian_adjust.cpp
	savedata_serialization.cpp
	
	BLADES.RC
	GAMEDLOG.RC
	GAMESTR.RC
	)

target_compile_definitions(Blades-of-Exile PRIVATE BOE_DEFAULT_VOLUME=32)

target_link_libraries(Blades-of-Exile Winmm.lib)


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

endif()

add_subdirectory("Dump Scenario")
add_subdirectory("Dump Savegame")
