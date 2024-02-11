cmake_minimum_required(VERSION 3.0)
project(glad)

# Add the source files
set(SOURCES
    "include/glad/gl.h"
    "src/gl.c"
)



# Create a static library
add_library(glad STATIC ${SOURCES})

# Include directories
target_include_directories(glad PUBLIC "include")