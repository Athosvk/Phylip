cmake_minimum_required( VERSION 3.11 )

project( Phylip )

add_subdirectory( external/raylib )

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples

if( MSVC )
	SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
endif()

set( CUSTOM_LIBS
	raylib
	)

set( SRC
	src/main.cpp
	)

if (UNIX)
	set (
		LIBS
		dl
		pthread
		)
else()
	set (
		LIBS
		)
endif()

add_executable( ${PROJECT_NAME} ${SRC})
target_link_libraries( ${PROJECT_NAME} ${LIBS} ${CUSTOM_LIBS})
target_compile_options( ${PROJECT_NAME} PRIVATE ${CXX_OPTIONS})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
