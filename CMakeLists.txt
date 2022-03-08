cmake_minimum_required( VERSION 3.1 )

project( Phylip )

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(ALLOW_DUPLICATE_CUSTOM_TARGETS TRUE)

include_directories(
    src/
    )

if( MSVC )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
endif()

set( COMMON_LIBS
	)

set( SRC
	src/main.cpp
	)

if (UNIX)
	set (
		COMMON_LIBS
		dl
		pthread
		)
else()
	set (
		COMMON_LIBS
		)
endif()

add_executable( Phylip ${SRC})
target_link_libraries( Phylip ${LIBS})
target_compile_options( Phylip PRIVATE ${CXX_OPTIONS})
set_property(TARGET Phylip PROPERTY CXX_STANDARD 17)
