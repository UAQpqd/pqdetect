cmake_minimum_required(VERSION 3.5)

find_package(Boost REQUIRED)
find_package(OpenCL REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(diffEvolution main.cpp)
target_link_libraries(diffEvolution OpenCL ${Boost_LIBRARIES})