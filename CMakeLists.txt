cmake_minimum_required(VERSION 3.24)

project(MultiArrowCharge VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CommonLibSSE CONFIG REQUIRED)

add_commonlibsse_plugin(${PROJECT_NAME}
    AUTHOR "MultiArrowCharge"
    SOURCES src/main.cpp)
