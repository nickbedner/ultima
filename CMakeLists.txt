cmake_minimum_required(VERSION 3.12)
project(Ultima VERSION 0.1.0)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
set(ignoreMe "${CMAKE_CXX_COMPILER}")
set(CMAKE_CXX_COMPILER_WORKS 1)
set(CMAKE_LINKER "C:/llvm-project/build/bin/lld-link.exe")
message("CMAKE_LINKER=${CMAKE_LINKER}")

file(GLOB_RECURSE ultima_SRC
        include/*.h
        src/*.c)

message("CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")

if(CMAKE_BUILD_TYPE MATCHES Release)
        include(CheckIPOSupported)
        check_ipo_supported(RESULT supported OUTPUT error)
        if(supported)
                message(STATUS "IPO / LTO enabled")
                set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
        else()
                message(STATUS "IPO / LTO not supported: <${error}>")
        endif()
endif()

add_executable(Ultima ${ultima_SRC})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mLLVM")

set(ULTIMA_BUILD_LIBRARIES Ultima)

add_custom_command (TARGET Ultima POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "C:/llvm-project/build/bin/LLVM-C.dll" ${CMAKE_CURRENT_BINARY_DIR}/LLVM-C.dll)

target_link_libraries(${ULTIMA_BUILD_LIBRARIES}
        "C:/llvm-project/build/lib/LLVM-C.lib"
        "C:/llvm-project/build/lib/LLVMAnalysis.lib"
        "C:/llvm-project/build/lib/LLVMBitWriter.lib"
        "C:/llvm-project/build/lib/LLVMCore.lib"
        "C:/llvm-project/build/lib/LLVMExecutionEngine.lib"
        "C:/llvm-project/build/lib/LLVMTarget.lib"
        "C:/llvm-project/build/lib/LLVMJITLink.lib")

set(includeList
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/cstorage/include
        "C:/llvm-project/build/lib/clang/16.0.0/include"
        "C:/llvm-project/build/include",
        "C:/llvm-project/llvm/include")

target_include_directories(Ultima PUBLIC ${includeList})
