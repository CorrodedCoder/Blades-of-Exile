# https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0 # or a later release
)

FetchContent_MakeAvailable(Catch2)

# Disable selective warnings for the Catch2 framework when compiling with Clang-cl
# TODO: Look for a better way to disable the no-unsafe-buffer-usage here as it will
# also disable it for child projects.
if (MSVC AND CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	target_compile_options(Catch2 PRIVATE
		-Wno-implicit-int-float-conversion
		-Wno-switch-enum
		-Wno-nonportable-system-include-path
		-Wno-double-promotion
		-Wno-covered-switch-default
	)
	target_compile_options(Catch2 PUBLIC
		-Wno-sign-conversion
		${BOE_WNO_UNSAFE_BUFFER_USAGE}
	)
endif()


include(CTest)
include(Catch)

add_subdirectory(unit)
