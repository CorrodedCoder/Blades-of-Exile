add_executable (DumpSavegame
	dump_savegame.cpp
	dump_savegame_cli.cpp
)

target_link_libraries(DumpSavegame
	boe
)