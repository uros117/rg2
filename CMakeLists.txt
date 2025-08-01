cmake_minimum_required( VERSION 3.1 )

project( RG2 )

# Check if submodules are initialized
if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/glfw/CMakeLists.txt")
    message(FATAL_ERROR "GLFW submodule not found. Please run 'git submodule update --init --recursive'")
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/imgui/imgui.h")
    message(FATAL_ERROR "ImGui submodule not found. Please run 'git submodule update --init --recursive'")
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/stb/stb_image.h")
    message(FATAL_ERROR "STB submodule not found. Please run 'git submodule update --init --recursive'")
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/glm/glm/glm.hpp")
    message(FATAL_ERROR "GLM submodule not found. Please run 'git submodule update --init --recursive'")
endif()

include_directories(include imgui glm .)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

find_package( OpenGL REQUIRED )

include_directories( ${OPENGL_INCLUDE_DIRS} )

#set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
#set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory( glfw glfw )

if( MSVC )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
endif()

file( GLOB SOURCES 
    src/*.cpp 
    src/*.c 
    imgui/*.cpp 
    imgui/backends/imgui_impl_opengl3.cpp
    imgui/backends/imgui_impl_glfw.cpp
    )
     
add_executable( RG2 WIN32 ${SOURCES} )
target_link_libraries( RG2 ${OPENGL_LIBRARIES} glfw )
if( MSVC )
    if(${CMAKE_VERSION} VERSION_LESS "3.6.0") 
	    message( "\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set 'RG2' as StartUp Project in Visual Studio.\n" )
    else()
        set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT GLFW-CMake-starter )
    endif()
endif()
