cmake_minimum_required(VERSION 3.26.1)

project(MAIN VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Catch2 3 REQUIRED)

list(APPEND targets
    #"src/CMyVector.cpp"
    #"src/CMyMatrix.cpp"
    #"src/CDGLSolver.cpp"
    #"src/CComplex.cpp"
    "src/CRandom.cpp"
    "tests/CRandomTest.cpp"
)

add_executable(${PROJECT_NAME} ${targets})

target_link_libraries(${PROJECT_NAME} PRIVATE Catch2::Catch2WithMain)
target_compile_options(${PROJECT_NAME} PRIVATE -O3)
