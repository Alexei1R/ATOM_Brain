
cmake_minimum_required(VERSION 3.0)
# Set the project name
project(glad)

# Add the source files
set(SOURCES
    "include/glad/glad.h"
    "include/KHR/khrplatform.h"
    "src/glad.c"
)

# Create a static library
add_library(${PROJECT_NAME} STATIC ${SOURCES})

# Set C language for the library
set_target_properties(${PROJECT_NAME} PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
)

# Set the output directory for the library
set_target_properties(${PROJECT_NAME} PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}"
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE "include")
