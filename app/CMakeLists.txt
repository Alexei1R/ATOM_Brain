cmake_minimum_required(VERSION 3.0)
project(ATOM)
find_package(OpenCV REQUIRED)


message(==============${CMAKE_CURRENT_SOURCE_DIR})


if (CMAKE_BUILD_TYPE STREQUAL "Release")
    message(Link to TensorRT)
    message(${CMAKE_CURRENT_SOURCE_DIR})
    #strip before last /
    string(REGEX REPLACE "/[^/]*$" "" MAIN_CMAKE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
    message(${MAIN_CMAKE_PATH})

    include(${MAIN_CMAKE_PATH}/cmake/ccache.cmake)
    set(CMAKE_MODULE_PATH "${MAIN_CMAKE_PATH}/cmake" ${MAIN_CMAKE_PATH})
    set(TensorRT_DIR /usr/)
    set(TensorRT_NVPARSERS_LIBRARY /usr/lib/libnvonnxparser.so)
    set(CUDA_TOOLKIT_ROOT_DIR /usr/local/cuda)
    find_package(TensorRT REQUIRED COMPONENTS nvparsers)
    find_package(CUDA REQUIRED)
    find_package(OpenCV REQUIRED)
endif ()


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
        ATOM/Sensors/Frame.cpp
        ATOM/Map/DrawMap.cpp
        ATOM/Map/DrawMap.h
        Platform/Window/OpenGLWindow.cpp
        Platform/Window/OpenGLWindow.h
        ATOM/GamePad/Gamepad.cpp
        ATOM/GamePad/Gamepad.h
        ATOM/Core/Editor/stb_image.cpp
        ATOM/Core/Editor/stb_image.h
        ATOM/RoadDetect/DetectLines.cpp
        ATOM/RoadDetect/DetectLines.h
        ATOM/RoadDetect/FindLines.cpp
        ATOM/RoadDetect/FindLines.h
        ATOM/AI/TrainEngine.cpp
        ATOM/AI/TrainEngine.h
        ATOM/AI/engine.cpp
        ATOM/AI/engine.h
        ATOM/AI/YoloV8.cpp
        ATOM/AI/YoloV8.h

)


add_executable(executable ${SOURCES})


if (CMAKE_BUILD_TYPE STREQUAL "Release")

    target_link_libraries(executable
            glad
            glfw
            imgui
            glm
            spdlog
            GameNetworkingSockets
            ${OpenCV_LIBS}
            ${CUDA_LIBRARIES}
            ${CMAKE_THREAD_LIBS_INIT}
            ${TensorRT_LIBRARIES}

    )

    target_include_directories(executable PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ ${CUDA_INCLUDE_DIRS} ${TensorRT_INCLUDE_DIRS})

else ()
    target_link_libraries(executable
            glad
            glfw
            imgui
            glm
            spdlog
            GameNetworkingSockets
            ${OpenCV_LIBS}
    )
    target_include_directories(executable PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)

endif ()

