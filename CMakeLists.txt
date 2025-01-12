cmake_minimum_required(VERSION 3.10)

project(HeliOS)

set(SOURCE_DIR src)

include_directories(HeliOS
    ${SOURCE_DIR}/cpu
    ${SOURCE_DIR}/kernel
    ${SOURCE_DIR}/memory
    ${SOURCE_DIR}/process
)

file(GLOB SOURCE_FILES 
    "${SOURCE_DIR}/*.h"
    "${SOURCE_DIR}/*.c"
)

add_executable(HeliOS ${SOURCE_FILES})