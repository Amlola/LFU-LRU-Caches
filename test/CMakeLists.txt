cmake_minimum_required(VERSION 3.10)

project(Test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PERFECT_INCLUDE_DIR ../perfect_cache/include)
set(LFU_INCLUDE_DIR ../lfu_cache/include)
set(LRU_INCLUDE_DIR ../lru_cache/include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

add_executable(${PROJECT_NAME} test.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${LFU_INCLUDE_DIR} ${PERFECT_INCLUDE_DIR} ${LRU_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} gtest)