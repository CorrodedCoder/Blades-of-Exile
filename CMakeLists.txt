# CMakeList.txt : CMake project for Blades-of-Exile, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.27.2)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Only set the cxx_standard if it is not set by someone else
if (NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 23)
endif()

set(CMAKE_CXX_EXTENSIONS OFF)


project (
	"Blades-of-Exile"
	VERSION 0.0.1
	HOMEPAGE_URL "https://github.com/CorrodedCoder/Blades-of-Exile"
	LANGUAGES CXX
	)

include(cmake/PreventInSourceBuilds.cmake)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)

add_subdirectory(src)
