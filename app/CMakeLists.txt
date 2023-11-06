cmake_minimum_required(VERSION 3.0)
project(ATOM)

# Find and include Ogre
find_package(OGRE 14.1.2 REQUIRED COMPONENTS Bites RTShaderSystem Terrain)
find_package(ZLIB)
# Set the source files for your project
set(SOURCES
    main.cpp
)


# Create an executable from the source files
add_executable(YourExecutableName ${SOURCES})

target_include_directories(YourExecutableName PRIVATE ${OGRE_INCLUDE_DIRS})


# Link Ogre libraries to your executable
target_link_libraries(YourExecutableName PRIVATE ${OGRE_LIBRARIES}) 

# Set the output directory for the binary
set_target_properties(YourExecutableName PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)

file(COPY ${OGRE_CONFIG_DIR}/plugins.cfg ${OGRE_CONFIG_DIR}/resources.cfg
     DESTINATION ${CMAKE_BINARY_DIR})