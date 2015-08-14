cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME shaderTest)
set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)
set(CMAKE_BUILD_TYPE Debug)
project( ${PROJECT_NAME} )

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set (CMAKE_CXX_FLAGS "-Wall -Werror -pedantic -g -std=c++11 -pg") 
ADD_DEFINITIONS(
    -lGL
    -lGLU
    -lsfml-graphics
    -lsfml-window
    -lsfml-system
    -DGL_GLEXT_PROTOTYPES 
)

# Versioning
# Replace @VARIABLES@ with CMake values
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/.version.in"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/version.hpp"
  )

# Source
file(GLOB_RECURSE  
	Project_SOURCES
       ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    )
add_executable( ${PROJECT_NAME} ${Project_SOURCES} )


########################### Libraries ##########################################

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
if (OPENGL_FOUND)
  target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
  target_link_libraries(${PROJECT_NAME} m)  # if using maths.h
endif()

# SFML
# SFML unknown to CMAKE by default -> need to copy FindSFML.cmake to /usr/shared/cmake-2.8/Modules
#find_package(SFML 1.6 REQUIRED system window graphics network audio)
find_package(SFML 2 REQUIRED system window graphics network audio)
include_directories(${SFML_INCLUDE_DIR})
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
endif()
