

# Add source to this project's executable.
add_executable (Blades-of-Exile WIN32
	"blades.cpp"
	"graphics.cpp"
	"newgraph.cpp"
	"fileio.cpp"
	"actions.cpp"
	"text.cpp"
	"party.cpp"
	"exlsound.cpp"
	"items.cpp"
	"fields.cpp"
	"town.cpp"
	"startup.cpp"
	"dlogtool.cpp"
	"dlgutils.cpp"
	"infodlgs.cpp"
	"graphutl.cpp"
	"global.cpp"
	"gutils.cpp"
	"monster.cpp"
	"locutils.cpp"
	"itemdata.cpp"
	"combat.cpp"
	"townspec.cpp"
	"specials.cpp"
	"BLADES.RC"
	)

target_link_libraries(Blades-of-Exile Winmm.lib)

# TODO: Add tests and install targets if needed.
