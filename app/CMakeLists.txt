cmake_minimum_required(VERSION 3.0)
project(ATOM)


message(==============${CMAKE_CURRENT_SOURCE_DIR})


set(SOURCES
        main.cpp
        ATOM/Core/Layers/Layer.cpp
        ATOM/Core/Layers/Layer.h
        ATOM/Core/Layers/LayerStack.h
        ATOM/Core/Layers/LayerStack.cpp
        ATOM/atompch.h
        ATOM/Core/macros.h
        ATOM/Core/Loging/Log.h
        ATOM/Core/Loging/Log.cpp
        ATOM/Application.h
        ATOM/Application.cpp
        ATOM/Core/Window.h
        Platform/Window/OpenGLWindow.h
        Platform/Window/OpenGLWindow.cpp
        ATOM/Core/Event/Mause.cpp
        ATOM/Core/Event/Keyboard.cpp
)



add_executable(executable ${SOURCES})

target_link_libraries(executable
    imgui
    glad
    glfw
    glm
    spdlog
 ) 


target_include_directories(executable PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)