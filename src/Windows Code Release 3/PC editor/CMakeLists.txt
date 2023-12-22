add_executable (PC-editor WIN32
	bladpced.cpp
	graphics.cpp
	graphutl.cpp
	edsound.cpp
	editors.cpp
	dlogtool.cpp
	edfileio.cpp
	edaction.cpp
	global.cpp
	../../boe/pc.cpp
	../../boe/utility.cpp

	BLADPCED.RC
	)

target_link_libraries(PC-editor Winmm.lib)

add_custom_command(
        TARGET PC-editor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
               "${CMAKE_CURRENT_SOURCE_DIR}/../Scen Ed"
               "${CMAKE_CURRENT_BINARY_DIR}/blscened")
