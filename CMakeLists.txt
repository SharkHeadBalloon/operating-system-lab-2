cmake_minimum_required(VERSION 2.8.12)
project(Current_project)
set(CMAKE_CXX_COMPILER "g++")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build)

# project os_2
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_executable(producer ${SOURCE_DIR}/producer.cpp ${SOURCE_DIR}/os_2.cpp)
add_executable(consumer ${SOURCE_DIR}/consumer.cpp ${SOURCE_DIR}/os_2.cpp)
add_executable(main ${SOURCE_DIR}/main.cpp ${SOURCE_DIR}/os_2.cpp)
