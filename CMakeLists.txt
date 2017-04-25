cmake_minimum_required(VERSION 3.6)
project(BarberProblem)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES hw2.cpp)
add_executable(BarberProblem ${SOURCE_FILES})

set_target_properties(BarberProblem PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(BarberProblem pthread)