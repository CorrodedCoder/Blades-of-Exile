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
	scened_globals.cpp

	BLSCENED.RC
	GAMEDLOG.RC
	STRINGS.RC
	)

target_link_libraries(Scen-Ed boe Winmm.lib)

file(COPY
	${CMAKE_SOURCE_DIR}/res/images/SPIDLOGO.BMP
	${CMAKE_SOURCE_DIR}/res/images/STATAREA.BMP
	${CMAKE_SOURCE_DIR}/res/images/TEXTBAR.BMP
	${CMAKE_SOURCE_DIR}/res/images/BUTTONS.BMP
	${CMAKE_SOURCE_DIR}/res/images/TERSCRN.BMP
	${CMAKE_SOURCE_DIR}/res/images/TER1.BMP
	${CMAKE_SOURCE_DIR}/res/images/TER2.BMP
	${CMAKE_SOURCE_DIR}/res/images/TER3.BMP
	${CMAKE_SOURCE_DIR}/res/images/TER4.BMP
	${CMAKE_SOURCE_DIR}/res/images/TER5.BMP
	${CMAKE_SOURCE_DIR}/res/images/TER6.BMP
	${CMAKE_SOURCE_DIR}/res/images/TERANIM.BMP
	${CMAKE_SOURCE_DIR}/res/images/FIELDS.BMP
	${CMAKE_SOURCE_DIR}/res/images/STARTUP.BMP
	${CMAKE_SOURCE_DIR}/res/images/DLOGPICS.BMP
	${CMAKE_SOURCE_DIR}/res/images/TALKPORT.BMP
	${CMAKE_SOURCE_DIR}/res/images/MISSILES.BMP
	${CMAKE_SOURCE_DIR}/res/images/TINYOBJ.BMP
	${CMAKE_SOURCE_DIR}/res/images/OBJECTS.BMP
	${CMAKE_SOURCE_DIR}/res/images/PCS.BMP
	${CMAKE_SOURCE_DIR}/res/images/MIXED.BMP
	${CMAKE_SOURCE_DIR}/res/images/MONST1.BMP
	${CMAKE_SOURCE_DIR}/res/images/MONST2.BMP
	${CMAKE_SOURCE_DIR}/res/images/MONST3.BMP
	${CMAKE_SOURCE_DIR}/res/images/MONST4.BMP
	${CMAKE_SOURCE_DIR}/res/images/MONST5.BMP
	${CMAKE_SOURCE_DIR}/res/images/MONST6.BMP
	${CMAKE_SOURCE_DIR}/res/images/MONST7.BMP
	${CMAKE_SOURCE_DIR}/res/images/MONST8.BMP
	${CMAKE_SOURCE_DIR}/res/images/MONST9.BMP
	${CMAKE_SOURCE_DIR}/res/images/MONST10.BMP
	${CMAKE_SOURCE_DIR}/res/images/STSCICON.BMP
	${CMAKE_SOURCE_DIR}/res/images/HELPPICS.BMP
	${CMAKE_SOURCE_DIR}/res/images/APPIC.BMP
	#${CMAKE_SOURCE_DIR}/res/images/BIGMAPS.BMP
	${CMAKE_SOURCE_DIR}/res/images/DLOGBTNS.BMP
	${CMAKE_SOURCE_DIR}/res/images/START.BMP

	${CMAKE_SOURCE_DIR}/res/images/EDSTART.BMP
	#${CMAKE_SOURCE_DIR}/res/images/E3EDTITL.BMP
	${CMAKE_SOURCE_DIR}/res/images/SCENPICS.BMP
	${CMAKE_SOURCE_DIR}/res/images/EDBTNS.BMP
	${CMAKE_SOURCE_DIR}/res/images/BIGSCEN.BMP

	"${CMAKE_CURRENT_SOURCE_DIR}/bladbase.exs"
	"${CMAKE_CURRENT_SOURCE_DIR}/bladbase_registered.exs"
	"${CMAKE_CURRENT_SOURCE_DIR}/bladdata.bld"
	"${CMAKE_CURRENT_SOURCE_DIR}/bladedit.hlp"

	DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
	)

add_custom_command(
        TARGET Scen-Ed POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
               "${CMAKE_CURRENT_SOURCE_DIR}/../BLADSCEN"
               "${CMAKE_CURRENT_BINARY_DIR}/bladscen")
