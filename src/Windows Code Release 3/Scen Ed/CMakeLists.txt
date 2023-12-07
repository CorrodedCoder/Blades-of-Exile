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
	../endian_adjust.cpp
	BLSCENED.RC
	)

target_link_libraries(Scen-Ed Winmm.lib)
