
include_directories("../inc" "../inc/CBS" "../inc/PIBT")
file(GLOB SOURCES "mapfdp-driver.cpp" "../src/*.cpp" "../src/CBS/*.cpp" "../src/PIBT/*.cpp")
add_executable(mapfdp-lns ${SOURCES})

include_directories( ${Boost_INCLUDE_DIRS} )
target_link_libraries(mapfdp-lns ${Boost_LIBRARIES} Eigen3::Eigen)
