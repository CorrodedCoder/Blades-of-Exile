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

if (MSVC)
	# Initially limit warning level to 3.
	# Enable permissive so that we leave the "const" issues for later.
	# For now also disable the following which generate hundreds of warnings:
	# * 4101: local variable is never used.
	# * 4244: 'conversion' conversion from 'type1' to 'type2', possible loss of data
    add_compile_options(/W3 /permissive /wd4101 /wd4244)
	# The code defines it's own min and max functions so disable VC's versions for now.
	# Also disable complaints about not using MS's "secure" versions of CRT functions,
	# largely because they aren't portable and in any case we'll be migrating such
	# functionality to leverage the C++ STL in good time.
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)

	if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
		# Warnings which are going to be more involved to address either because of
		# the complexity, or the number of places it touches.
		add_compile_options(
			-Wno-writable-strings
			-Wno-extern-initializer
			-Wno-unused-variable
			-Wno-char-subscripts
			-Wno-misleading-indentation
			-Wno-unused-but-set-variable
			-Wno-constant-conversion
			-Wno-sometimes-uninitialized
		)
		# Warnings which either should be addressed ASAP or are trivial to address
		add_compile_options(
			-Wno-empty-body
			-Wno-unused-value
			-Wno-parentheses
			-Wno-missing-braces
			-Wno-tautological-overlap-compare
			-Wno-tautological-constant-compare
			-Wno-pointer-to-int-cast
		)
	endif()
endif()

add_subdirectory(src)
