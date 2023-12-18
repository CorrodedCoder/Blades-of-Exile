add_executable(tests
	../../src/boe/pc.cpp
	../../src/boe/adventurers.cpp
	test_adventurers.cpp
)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)

catch_discover_tests(tests)