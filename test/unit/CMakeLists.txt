if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set_directory_properties(PROPERTIES MY_VS_WARNING_LEVEL 4)
	add_compile_options(/WX)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	add_compile_options(
		-Wextra -Wpedantic -Werror ${BOE_WNO_UNSAFE_BUFFER_USAGE} -Wno-missing-field-initializers
	)
endif()


add_executable(tests
	test_pc.cpp
	test_adventurers.cpp
)
target_link_libraries(tests PRIVATE boe Catch2::Catch2WithMain)

catch_discover_tests(tests)