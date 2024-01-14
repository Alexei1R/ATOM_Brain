cmake_minimum_required(VERSION 3.0)
project(ATOM)
find_package(OpenCV REQUIRED )


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
        ATOM/Core/Editor/ImGuiLayer.cpp
        ATOM/Core/Editor/ImGuiLayer.h
        ATOM/Core/Editor/EditorLayer.cpp
        ATOM/Core/Editor/EditorLayer.h
        ATOM/Core/ScreenData/DrawMap.cpp
        ATOM/Core/ScreenData/DrawMap.h
        Networking/Networking.cpp
        Networking/Networking.h
        Networking/Client.h
        Networking/Client.cpp
        Networking/Server.h
        Networking/Server.cpp
        ATOM/Sensors/Frame.cpp
        ATOM/Sensors/Frame.h
        VideoCapture/VideoCapture.cpp
        VideoCapture/VideoCapture.h
        ATOM/Core/ClientLayer.cpp
        ATOM/Core/ClientLayer.h
        Platform/Window/SDLWindow.cpp
        Platform/Window/SDLWindow.h

)



add_executable(executable ${SOURCES})

target_link_libraries(executable
        SDL3::SDL3
        glfw
        glad
        imgui
        glm
        spdlog
        GameNetworkingSockets
        ${OpenCV_LIBS}
)



target_include_directories(executable PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)