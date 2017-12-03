cmake_minimum_required(VERSION 2.8)
project(rhoban_utils)

find_package(catkin REQUIRED COMPONENTS
  jsoncpp
  eigen
)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES rhoban_utils
  CATKIN_DEPENDS jsoncpp eigen
)

option (RHOBAN_UTILS_BUILD_APPS "Build Rhoban utils apps" OFF)

include_directories(include ${catkin_INCLUDE_DIRS})

#Enable C++11
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")

#Enable Weird option linker
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

#List Sources sub source directories
set(SOURCES_DIRECTORIES
  src/rhoban_utils
  src/rhoban_utils/control
  src/rhoban_utils/history
  src/rhoban_utils/logging
  src/rhoban_utils/nominal
# TODO refactor: treat this
#  timing
#  geometry -> move to rhoban_geometry
#  spline
#  sockets
#  history
#  threading
#  trajectory
#  astar -> move to rhoban_graphs
#  graph -> move to rhoban_graphs
#  random
)

#Include Sources sub sources
foreach (DIRECTORY ${SOURCES_DIRECTORIES})
    message(STATUS "-- Sources: Adding ${DIRECTORY}/")
    include (${DIRECTORY}/Sources.cmake)
    set (PREFIXED_SOURCES)
    foreach (SOURCE ${SOURCES})
        set (PREFIXED_SOURCES ${PREFIXED_SOURCES} ${DIRECTORY}/${SOURCE})
    endforeach (SOURCE)
    set (ALL_SOURCES ${ALL_SOURCES} ${PREFIXED_SOURCES})
endforeach (DIRECTORY)

add_library (rhoban_utils ${ALL_SOURCES})
target_link_libraries(rhoban_utils ${catkin_LIBRARIES})

# TODO refactor: move to rhoban_graphs
#if (RHOBANUTILS_BUILD_APPS)
#    add_executable (astarTest astar/main.cpp)
#    target_link_libraries (astarTest rhoban_utils)
#    
#    add_executable (graphTest graph/main.cpp)
#    target_link_libraries (graphTest rhoban_utils)
#endif ()

