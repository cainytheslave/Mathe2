cmake_minimum_required(VERSION 3.26.1)

project(MAIN)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME} CMyVector.cpp)

target_compile_options(${PROJECT_NAME} PRIVATE -O3)
