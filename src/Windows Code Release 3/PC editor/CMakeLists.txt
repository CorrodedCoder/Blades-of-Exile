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
	../gdi_helpers.cpp

	BLADPCED.RC
	)

target_link_libraries(PC-editor boe Winmm.lib)

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

	${CMAKE_SOURCE_DIR}/res/images/BLEDTITL.BMP
	${CMAKE_SOURCE_DIR}/res/images/PCEDBTNS.BMP
	#${CMAKE_SOURCE_DIR}/res/images/DLOGMAPS.BMP
	DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/blscened
	)
