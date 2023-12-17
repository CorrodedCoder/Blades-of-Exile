add_executable (Scen-Ed WIN32
	blscened.cpp
	graphics.cpp
	edsound.cpp
	keydlgs.cpp
	graphutl.cpp
	tactions.cpp
	dlogtool.cpp
	scenario.cpp
	tfileio.cpp
	townout_stub.cpp
	global.cpp
	buttonmg.cpp
	../../boe/endian_adjust.cpp
	BLSCENED.RC
	GAMEDLOG.RC
	STRINGS.RC
	)

target_link_libraries(Scen-Ed Winmm.lib)

add_custom_command(
        TARGET Scen-Ed POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
               "${CMAKE_CURRENT_SOURCE_DIR}"
               "${CMAKE_CURRENT_BINARY_DIR}")

add_custom_command(
        TARGET Scen-Ed POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
               "${CMAKE_SOURCE_DIR}/src/Macintosh Code Release 3/Blades of Exile Scenarios"
               "${CMAKE_CURRENT_BINARY_DIR}/bladscen")
