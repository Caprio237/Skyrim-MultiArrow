cmake_minimum_required(VERSION 3.24)

project(MultiArrowCharge VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(ENABLE_SKYRIM_SE ON CACHE BOOL "" FORCE)
set(ENABLE_SKYRIM_AE ON CACHE BOOL "" FORCE)
set(ENABLE_SKYRIM_VR OFF CACHE BOOL "" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)

add_subdirectory(CommonLibSSE-NG ${CMAKE_BINARY_DIR}/_deps/commonlib-build)

file(GLOB _commonlib_cmake_files "${CMAKE_CURRENT_SOURCE_DIR}/CommonLibSSE-NG/cmake/*")
message(STATUS "CommonLib cmake files: ${_commonlib_cmake_files}")
include("${CMAKE_CURRENT_SOURCE_DIR}/CommonLibSSE-NG/cmake/CommonLibSSE.cmake")

add_commonlibsse_plugin(${PROJECT_NAME}
    AUTHOR "MultiArrowCharge"
    SOURCES src/main.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE src)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/PCH.h)
