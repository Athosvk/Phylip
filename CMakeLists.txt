cmake_minimum_required( VERSION 3.11 )

project( Phylip )

add_subdirectory( external/raylib )
add_subdirectory( external/yaml-cpp )

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples

if( MSVC )
	SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
endif()

set( CUSTOM_LIBS
	raylib
	yaml-cpp
	)

set ( INC
		external/eigen/
		external/yaml-cpp/include
		external/raygui/src/
	)

set( SRC
	src/main.cpp
	src/scene_parser.cpp
	src/mtransform.cpp
	src/sphere_primitive.cpp
	src/cloth_mesh.cpp
	src/scene.cpp
	src/scene.cpp
	src/options.cpp
	src/renderer.cpp
	src/cloth_simulator.cpp
	src/spring_constraint.cpp
	src/attachment_constraint.cpp
	src/gui_wrapper.cpp
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

include_directories(${INC})
add_executable( ${PROJECT_NAME} ${SRC})
target_link_libraries( ${PROJECT_NAME} ${LIBS} ${CUSTOM_LIBS})
target_compile_options( ${PROJECT_NAME} PRIVATE ${CXX_OPTIONS})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

if( UNIX )
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E
        create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/resources)
else()
    file(TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/resources" _dstDir)
    file(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/resources" _srcDir)
    execute_process(COMMAND cmd.exe /c mklink /J "${_dstDir}" "${_srcDir}")
endif()
