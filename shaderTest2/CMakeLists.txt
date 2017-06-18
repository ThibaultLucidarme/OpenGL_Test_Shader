cmake_minimum_required(VERSION 3.5)
cmake_policy(SET CMP0048 NEW)


if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt." )
endif()

set(PROJECT_NAME shaderTest CACHE PATH "Project name")


# If you add the CACHE part to your set command, it will be visible in CCMake. So, e.g.
# set(VAR <value> CACHE PATH "Some details about VAR")
# The types of variable which appear in CCMake are FILEPATH, PATH, STRING and BOOL.
set(PROJECT_VERSION_MAJOR 1 CACHE PATH "Project Major version")
set(PROJECT_VERSION_MINOR 0 CACHE PATH "Project Minor version")
set(PROJECT_VERSION_PATCH 0 CACHE PATH "Project Patch version")

project( ${PROJECT_NAME} LANGUAGES CXX VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})
set(CMAKE_BUILD_TYPE Debug)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(WERROR_FLAG "-Werror")
if( NOT CMAKE_COMPILER_IS_GNUCXX)
  # Windows does not accept -Werror
  set(WERROR_FLAG "-W4")
endif()
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pedantic -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O4 ${WERROR_FLAG}")

#set (CMAKE_CXX_FLAGS "-Wall -Werror -pedantic -g -std=c++11 -pg") 
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
if ( EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/.version.in)
  configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/.version.in"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/version.hpp"
    )
endif()

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
set(CMAKE_PREFIX_PATH /usr/local/build/SFML-2.3/)
find_package(SFML 2 REQUIRED system window graphics network audio)
include_directories(${SFML_INCLUDE_DIR})
if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${PROJECT_NAME} ${SFML_LIBRARIES})
  add_definitions(
    -lsfml-graphics
    -lsfml-window
    -lsfml-system
  )
endif()

