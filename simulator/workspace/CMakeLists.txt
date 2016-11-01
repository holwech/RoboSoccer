cmake_minimum_required(VERSION 2.8.9)
project (RoSoExecution)

find_package(Threads REQUIRED)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

#additional libraries
set(LIBRARIES ${LIBRARIES} libkogmo_rtdb.so libmath.so libpolulu.so)
link_directories(${CMAKE_SOURCE_DIR}/libraries/libmath/lib)
link_directories(${CMAKE_SOURCE_DIR}/libraries/kogmo/lib)
link_directories(${CMAKE_SOURCE_DIR}/libraries/libpololurtdb/lib)


#include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/libmath/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/kogmo/include)
include_directories(${CMAKE_SOURCE_DIR}/libraries/libpololurtdb/include)


file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "include/*.h")


add_executable(RoSoExe ${SOURCES} ${HEADERS})
target_link_libraries(RoSoExe  ${LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(RoSoExe -lrt)

