
cmake_minimum_required(VERSION 3.15)

project(ATOM
        VERSION 0.0.1
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)  # Choose Release, Debug, etc.
endif()

add_compile_options(
        "-Wno-array-bounds"
        "-Wno-error=stringop-overflow"
)

add_subdirectory(libs)
add_subdirectory(app)