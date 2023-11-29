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

# set(BOE_WARNINGS_MAX ON)

if (MSVC)
	# Disable complaints about not using MS's "secure" versions of CRT functions,
	# largely because they aren't portable and in any case we'll be migrating such
	# functionality to leverage the C++ STL in good time.
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)

	if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
		if(BOE_WARNINGS_MAX)
			add_compile_options(
				/W4
				# For now these are just creating too many hits for these to be practical
				/wd4100
				/wd4244
			)
		else()
			# This is our default settings for the builds
			add_compile_options(
				# Warnings as errors
				/WX
				# Limit warning level to 3.
				/W3 
				# For now also disable the following which generate hundreds of warnings:
				/wd4244
				/wd4018
				/wd4060
				/wd4065
				/wd4267
				/wd4302
				/wd4305
				/wd4309
				/wd4311
				/wd4390
				/wd4554
				/wd4804
			)
		endif()
	elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		if(BOE_WARNINGS_MAX)
			add_compile_options(-Wall -Wextra -Wpedantic)
		else()
			# This is our default settings for the builds
			add_compile_options(
				# default to all warnings
				-Wall

				# Warnings as errors
				-Werror

				# It seems required to disable this warning, but not sure why
				# as it should know it's irrelevant to anyone specifying
				# a later C++ standard.
				-Wno-c++98-compat-pedantic

				# Disable selective warnings from -Wall
				-Wno-missing-variable-declarations
				-Wno-unused-parameter
				-Wno-old-style-cast
				-Wno-cast-align
				-Wno-missing-prototypes
				-Wno-unreachable-code-break
				-Wno-implicit-int-conversion
				-Wno-sign-conversion
				-Wno-unused-but-set-variable
				-Wno-conditional-uninitialized
				-Wno-shadow
				-Wno-tautological-unsigned-zero-compare
				-Wno-sign-compare
				-Wno-unreachable-code
				-Wno-shorten-64-to-32
				-Wno-tautological-value-range-compare
				-Wno-unsafe-buffer-usage
				-Wno-implicit-fallthrough

				# Replace NULL with nullptr and we can remove the following item
				-Wno-zero-as-null-pointer-constant

				# Disable warnings that come out even without -Wall
				-Wno-extern-initializer
				-Wno-char-subscripts
				-Wno-misleading-indentation
				-Wno-constant-conversion
				-Wno-sometimes-uninitialized
				-Wno-empty-body
				-Wno-parentheses
				-Wno-tautological-overlap-compare
				-Wno-tautological-constant-compare
			)
		endif()
	endif()
endif()

add_subdirectory(src)
