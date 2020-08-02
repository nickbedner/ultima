cmake_minimum_required(VERSION 3.12)
project(Ultima VERSION 0.1.0)

set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE ultima_SRC
        include/*.h
        src/*.c)

add_executable(Ultima ${ultima_SRC})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-visibility -Wno-pragma-pack -Wno-deprecated-declarations")

message("CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")

set(includeList
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/cstorage/include
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/llvm-project/llvm/include)

target_include_directories(Ultima PUBLIC ${includeList})
