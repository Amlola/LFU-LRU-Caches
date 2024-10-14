cmake_minimum_required(VERSION 3.10)
project(CacheProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(DEBUG OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(BASE_CACHE_INCLUDE_DIR unified_cache)

if (DEBUG)
    add_compile_definitions(DEBUG_CACHE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined")
endif()

function(add_cache CACHE_NAME)
    file(GLOB SRC "${CACHE_NAME}/source/*.cpp")

    add_executable(${CACHE_NAME} ${SRC})

    target_include_directories(${CACHE_NAME} PRIVATE ${CACHE_NAME}/include ${BASE_CACHE_INCLUDE_DIR})

    set_target_properties(${CACHE_NAME} PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
    )
endfunction()

add_cache("lru_cache")
add_cache("lfu_cache")
add_cache("perfect_cache")
