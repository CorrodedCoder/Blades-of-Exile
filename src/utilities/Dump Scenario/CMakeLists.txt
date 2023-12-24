add_executable (DumpScenario
	dump_scenario.cpp
	dump_scenario_cli.cpp
)

target_link_libraries(DumpScenario
	boe
)