cmake_minimum_required(VERSION 3.12)
project(ultima VERSION 0.1.0)

set(CMAKE_C_STANDARD 11)

message("CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")

file(GLOB_RECURSE ultima_SRC
        include/*.h
        src/*.c)

add_subdirectory(lib/llvm-project/llvm buildllvm)

SET(OBJS
        ${CMAKE_CURRENT_SOURCE_DIR}/build/buildllvm/lib/Target/X86/TargetInfo/LLVMX86Info.dir/Debug/X86TargetInfo.obj
        ${CMAKE_CURRENT_SOURCE_DIR}/build/buildllvm/lib/Target/X86/LLVMX86CodeGen.dir/Debug/X86TargetMachine.obj
        ${CMAKE_CURRENT_SOURCE_DIR}/build/buildllvm/lib/Target/X86/MCTargetDesc/LLVMX86Desc.dir/Debug/X86MCTargetDesc.obj)

add_executable(ultima ${OBJS} ${ultima_SRC})

set(ULTIMA_BUILD_LIBRARIES
        ultima
        LLVM-C
        LLVMAnalysis
        LLVMBitWriter
        LLVMCore
        LLVMExecutionEngine
        LLVMTarget
        LLVMJITLink)
        #X86TargetInfo)

target_link_libraries(${ULTIMA_BUILD_LIBRARIES})

set(includeList
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/cstorage/include
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/llvm-project/llvm/include
        ${CMAKE_CURRENT_SOURCE_DIR}/build/buildllvm/include)

target_include_directories(ultima PUBLIC ${includeList})

#cmake_minimum_required(VERSION 3.12)
#project(ultima VERSION 0.1.0)
#
#set(CMAKE_C_STANDARD 11)
#
#file(GLOB_RECURSE ultima_SRC
#        include/*.h
#        src/*.c)
#
#message("CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
#
#add_subdirectory(lib/llvm-project/llvm buildllvm)
#
#find_package(LLVM REQUIRED CONFIG)
#add_definitions(${LLVM_DEFINITIONS})
#include_directories(${LLVM_INCLUDE_DIRS})
#
#message(STATUS "LLVM VERSION : ${LLVM_PACKAGE_VERSION}")
#message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
#
#add_executable(ultima ${ultima_SRC})
#
#llvm_map_components_to_libnames(llvm_libs support core irreader)
#
#set(includeList
#        ${CMAKE_CURRENT_SOURCE_DIR}/include
#        ${CMAKE_CURRENT_SOURCE_DIR}/lib/cstorage/include
#        ${CMAKE_CURRENT_SOURCE_DIR}/lib/llvm-project/llvm/include
#        ${CMAKE_CURRENT_SOURCE_DIR}/build/buildllvm/include)
#
#target_include_directories(ultima PUBLIC ${includeList})
#
#target_link_libraries(ultima llvm_libs)