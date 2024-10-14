cmake_minimum_required(VERSION 3.10)

project(Benchmark)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

set(PERFECT_INCLUDE_DIR ../perfect_cache/include)
set(LFU_INCLUDE_DIR ../lfu_cache/include)
set(LRU_INCLUDE_DIR ../lru_cache/include)

set(BASE_CACHE_INCLUDE_DIR ../unified_cache)

add_executable(bench_lfu source/lfu-perfect.cpp)

target_include_directories(bench_lfu PRIVATE ${LFU_INCLUDE_DIR} ${PERFECT_INCLUDE_DIR} ${BASE_CACHE_INCLUDE_DIR})

add_executable(bench_lru source/lru-perfect.cpp)

target_include_directories(bench_lru PRIVATE ${LRU_INCLUDE_DIR} ${PERFECT_INCLUDE_DIR} ${BASE_CACHE_INCLUDE_DIR})