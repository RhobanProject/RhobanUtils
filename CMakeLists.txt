cmake_minimum_required(VERSION 2.8)
project(RhobanUtils)

find_package(catkin REQUIRED COMPONENTS
  jsoncpp Eigen
  )

find_package(model)

catkin_package(
  INCLUDE_DIRS .
  LIBRARIES RhobanUtils
  CATKIN_DEPENDS jsoncpp
)

option (RHOBANUTILS_BUILD_APPS "Build Rhoban utils apps" OFF)

include_directories(. ${catkin_INCLUDE_DIRS})

#Enable C++11
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")

#Enable Weird option linker
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

#List Sources sub source directories
set(SOURCES_DIRECTORIES
  .
  timing
  geometry
  spline
  sockets
  logging
  history
  threading
  trajectory
  astar
  graph
  nominal
  random
)

if (model_FOUND)
    include_directories(${model_INCLUDE_DIRS})
    set (SOURCES_DIRECTORIES
        ${SOURCES_DIRECTORIES}
        moves
    )
endif ()

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

add_library (RhobanUtils ${ALL_SOURCES})
target_link_libraries(RhobanUtils ${catkin_LIBRARIES})

if (RHOBANUTILS_BUILD_APPS)
    add_executable (astarTest astar/main.cpp)
    target_link_libraries (astarTest RhobanUtils)

    add_executable (graphTest graph/main.cpp)
    target_link_libraries (graphTest RhobanUtils)
endif ()
