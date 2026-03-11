cmake_minimum_required(VERSION 3.16)
project(Ultima VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB_RECURSE ultima_SRC
        include/*.h
        src/*.c)

add_executable(Ultima ${ultima_SRC})

# ---------------------------------------
# System LLVM configuration
# ---------------------------------------
# Allow user override, otherwise default to LLVM-dev
if(NOT DEFINED LLVM_DIR)
        set(LLVM_DIR "C:/LLVM-dev/lib/cmake/llvm" CACHE PATH "Path to LLVMConfig.cmake")
endif()

find_package(LLVM REQUIRED CONFIG)

target_include_directories(Ultima PRIVATE ${LLVM_INCLUDE_DIRS})
target_compile_definitions(Ultima PRIVATE ${LLVM_DEFINITIONS})

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

target_include_directories(Ultima PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/cstorage/include
)

target_link_libraries(Ultima PRIVATE LLVM-C)

# ---------------------------------------
# Copy LLVM-C.dll for Windows
# ---------------------------------------
if(WIN32)
        add_custom_command(TARGET Ultima POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:LLVM-C>
        $<TARGET_FILE_DIR:Ultima>
        )
endif()
