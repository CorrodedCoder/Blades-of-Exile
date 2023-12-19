cmake_minimum_required (VERSION 3.25.1)

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

# set(BOE_WARNINGS_MAX ON)
# set(BOE_DEBUG_SYMBOLS ON)
# set(BOE_ADDRESS_SANITIZER ON)

include_directories(include)

if (MSVC)
	# Disable complaints about not using MS's "secure" versions of CRT functions,
	# largely because they aren't portable and in any case we'll be migrating such
	# functionality to leverage the C++ STL in good time.
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)

	if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
		# See the following for an explanation of this warning model:
		#   https://stackoverflow.com/a/76990741/5751627

		# Define a target property which will contain desired warning level
		# for a target (library or executable)
		define_property(TARGET PROPERTY MY_VS_WARNING_LEVEL
		  BRIEF_DOCS "Warning level"
		  FULL_DOCS "Warning level for Visual Studio compiler: 1, 2, ..."
		  INHERITED # The property will be initialized from the directory property.
		)
		# Define a **directory** property with the same name
		# It will be used via inheritance and provide default value for the target property.
		define_property(DIRECTORY PROPERTY MY_VS_WARNING_LEVEL
		  BRIEF_DOCS "Warning level"
		  FULL_DOCS "Warning level for Visual Studio compiler: 1, 2, ..."
		  INHERITED # The property will be initialized from the parent property.
		)

		if(BOE_WARNINGS_MAX)
			set_directory_properties(PROPERTIES MY_VS_WARNING_LEVEL 4)
		else()
			set_directory_properties(PROPERTIES MY_VS_WARNING_LEVEL 3)
			# This is our default settings for the builds
			# Warnings as errors
			add_compile_options(/WX /Os)
		endif()

		# Add compiler options which depends on the target property
		add_compile_options(/W$<TARGET_PROPERTY:MY_VS_WARNING_LEVEL>)

		if(BOE_DEBUG_SYMBOLS)
			# Enable debug symbols even in release builds
			set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
			set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DEBUG")
		endif()

		if(BOE_ADDRESS_SANITIZER)
			add_compile_options(/fsanitize=address)
		endif()
	elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		add_compile_options(
			# Support C++ exceptions when using Clang under MSVC, this is not the default.
			/EHsc
		)
	endif()
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	include(CheckCXXCompilerFlag)
	check_cxx_compiler_flag("-Wno-unsafe-buffer-usage" BOE_FLAG_HAS_NO_UNSAFE_BUFFER_USAGE)
	if(BOE_FLAG_HAS_NO_UNSAFE_BUFFER_USAGE)
		set(BOE_WNO_UNSAFE_BUFFER_USAGE -Wno-unsafe-buffer-usage)
	endif()

	add_compile_options(
		# Default warning level
		-Wall
		-ferror-limit=1000

		# It seems required to disable this warning, but not sure why
		# as it should know it's irrelevant to anyone specifying
		# a later C++ standard.
		-Wno-c++98-compat-pedantic
		-Wno-c++20-compat
	)
	if(BOE_WARNINGS_MAX)
		add_compile_options(-Wextra -Wpedantic)
	else()
		# Warnings as errors
		add_compile_options(-Werror)
	endif()
endif()

add_subdirectory(src)

option(ENABLE_TESTS "Enable tests" ON)
if (${ENABLE_TESTS})
    enable_testing()
	add_subdirectory(test)
endif()
