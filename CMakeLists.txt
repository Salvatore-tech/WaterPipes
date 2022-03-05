cmake_minimum_required(VERSION 3.21)
project(WaterPipes)
set(CMAKE_CXX_STANDARD 20)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(INCLUDE_FILES hash_table/api/HashTable.h)
set(SOURCE_FILES main.cpp hash_table/impl/HashTable.cpp ExecutionTimer.h)
add_executable(WaterPipes ${HEADER_FILES} ${SOURCE_FILES})

include(FindPkgConfig)
find_package(GraphNode REQUIRED)
include_directories(${GRAPHNODE_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${GRAPHNODE_LIBRARY})

include(FindPkgConfig)
find_package(IOGraph REQUIRED)
include_directories(${IOGRAPH_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${IOGRAPH_LIBRARY})

include(FindPkgConfig)
find_package(HashingStrategy REQUIRED)
include_directories(${HASHING_STRATEGY_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${HASHING_STRATEGY_LIBRARY})
