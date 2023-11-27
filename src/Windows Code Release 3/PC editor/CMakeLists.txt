

# Add source to this project's executable.
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
	BLADPCED.RC
	)

target_link_libraries(PC-editor Winmm.lib)
