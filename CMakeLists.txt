cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

project(connection)

set (${PROJECT_NAME}_VERSION_MAJOR 00)
set (${PROJECT_NAME}_VERSION_MINOR 02)
set (${PROJECT_NAME}_VERSION_PATCH 09)

find_package(Threads REQUIRED)

## Compiler flags
set(CXX_COVERAGE_COMPILE_FLAGS "-std=c++98")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${CXX_COVERAGE_COMPILE_FLAGS}")
#set(C_COVERAGE_COMPILE_FLAGS "-std=c99")
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} ${C_COVERAGE_COMPILE_FLAGS}")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

if (CMAKE_BUILD_TYPE STREQUAL Debug)
	message("++ MAKE IN DEBUG MODE")
elseif (CMAKE_BUILD_TYPE STREQUAL Release)
	message("++ MAKE IN RELEASE MODE")
endif (CMAKE_BUILD_TYPE STREQUAL Debug)

## Sources
set (SRC_DIR src)
file (GLOB SOURCES ${SRC_DIR}/*.c*)
file (GLOB HEADERS ${SRC_DIR}/*.h)

## Configuration
configure_file (
    "${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME}Config.h.in"
    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.h"
)

# binary_dir
include_directories("${PROJECT_BINARY_DIR}")

## Target
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} 

)

