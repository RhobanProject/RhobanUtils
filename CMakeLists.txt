cmake_minimum_required(VERSION 2.8)
project(RhobanUtils)

find_package(catkin REQUIRED COMPONENTS
  jsoncpp
  model)

catkin_package(
  INCLUDE_DIRS .
  LIBRARIES RhobanUtils
  CATKIN_DEPENDS jsoncpp model
)

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
  moves
  history
  threading
  trajectory
  nominal
  random
)

#Include Sources sub sources
foreach (DIRECTORY ${SOURCES_DIRECTORIES})
    message("-- Sources: Adding ${DIRECTORY}/")
    include (${DIRECTORY}/Sources.cmake)
    set (PREFIXED_SOURCES)
    foreach (SOURCE ${SOURCES})
        set (PREFIXED_SOURCES ${PREFIXED_SOURCES} ${DIRECTORY}/${SOURCE})
    endforeach (SOURCE)
    set (ALL_SOURCES ${ALL_SOURCES} ${PREFIXED_SOURCES})
endforeach (DIRECTORY)

add_library (RhobanUtils ${ALL_SOURCES})
target_link_libraries(RhobanUtils ${catkin_LIBRARIES})