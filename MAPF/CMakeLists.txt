
include_directories("../inc" "../inc/CBS" "../inc/PIBT")
file(GLOB SOURCES "mapf-driver.cpp" "../src/*.cpp" "../src/CBS/*.cpp" "../src/PIBT/*.cpp")
add_executable(mapf-lns ${SOURCES})

include_directories( ${Boost_INCLUDE_DIRS} )
target_link_libraries(mapf-lns ${Boost_LIBRARIES} Eigen3::Eigen)
