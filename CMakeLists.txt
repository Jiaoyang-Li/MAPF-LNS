# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
project(lns)
cmake_minimum_required (VERSION 2.6)
set (CMAKE_CXX_STANDARD 14)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE "RELEASE")
ENDIF()

# Find Boost
find_package(Boost REQUIRED COMPONENTS program_options system filesystem)

# Find Eigen3 for PIBT
find_package (Eigen3 3.3 REQUIRED NO_MODULE)

add_subdirectory ("MAPF")
add_subdirectory ("MAPF-DP")