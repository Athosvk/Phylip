cmake_minimum_required( VERSION 3.11 )

project( Phylip )

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set( GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only" FORCE)
set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW lib only" FORCE)
set( GLFW_BUILD_TESTS OFF CACHE BOOL "GLFW lib only" FORCE)
set( GLFW_INSTALL OFF CACHE BOOL "GLFW lib only" FORCE)

if( MSVC )
	SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
endif()

add_subdirectory( external/glfw )

set ( INC
		external/eigen/
		external/glad/include/
		external/glfw/include
		external/glm/
	)

set( SRC
	external/glad/src/glad.c
	src/main.cpp
	src/transform.cpp
	src/sphere_primitive.cpp
	src/shader.cpp
	src/shader_map.cpp
	src/camera.cpp
	src/scene.cpp
	src/options.cpp
	src/renderer.cpp
	)

if (UNIX)
	set (
		LIBS
		dl
		pthread
		glfw
		)
else()
	set (
		LIBS
		glfw
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
        create_symlink ${CMAKE_CURRENT_SOURCE_DIR}/shaders ${CMAKE_BINARY_DIR}/shaders)
else()
    file(TO_NATIVE_PATH "${CMAKE_CURRENT_BINARY_DIR}/shaders" _dstDir)
    file(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/shaders" _srcDir)
    execute_process(COMMAND cmd.exe /c mklink /J "${_dstDir}" "${_srcDir}")
endif()


