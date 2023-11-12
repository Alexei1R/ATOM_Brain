cmake_minimum_required(VERSION 3.0)
project(ATOM)

set(CMAKE_CXX_STANDARD 17)  # or the version you are using
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message(==============${CMAKE_CURRENT_SOURCE_DIR})


set(SOURCES
        main.cpp
#        ATOM/Core/Layers/Layer.cpp
#        ATOM/Core/Layers/Layer.h
#        ATOM/Core/Layers/LayerStack.h
#        ATOM/Core/Layers/LayerStack.cpp
#        ATOM/atompch.h
#        ATOM/Core/macros.h
#        ATOM/Core/Loging/Log.h
#        ATOM/Core/Loging/Log.cpp
#        ATOM/Application.h
#        ATOM/Application.cpp
#        ATOM/Core/Window.h
#        Platform/Window/OpenGLWindow.h
#        Platform/Window/OpenGLWindow.cpp
#        ATOM/Core/Event/Mause.cpp
#        ATOM/Core/Event/Keyboard.cpp
#        ATOM/Core/Editor/ImGuiLayer.cpp
#        ATOM/Core/Editor/ImGuiLayer.h
#        ATOM/Core/Editor/EditorLayer.cpp
#        ATOM/Core/Editor/EditorLayer.h
#        ATOM/Core/Editor/FrameBuffer.h
#        ATOM/Core/Editor/FrameBuffer.cpp
#        ATOM/Core/Renderer/Shader.cpp
#        ATOM/Core/Renderer/Shader.h
#        ATOM/Core/Renderer/Camera.cpp
#        ATOM/Core/Renderer/Camera.h
#        ATOM/Core/Renderer/stb_image.cpp
#        ATOM/Core/Renderer/stb_image.h
#        ATOM/Core/Renderer/Mesh.cpp
#        ATOM/Core/Renderer/Mesh.h
#        ATOM/Core/Renderer/Model.cpp
#        ATOM/Core/Renderer/Model.h
#        ATOM/Core/Renderer/Transform.cpp
#        ATOM/Core/Renderer/Transform.h
#        ATOM/Core/ScreenData/DrawMap.cpp
#        ATOM/Core/ScreenData/DrawMap.h

)



add_executable(executable ${SOURCES})

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
 target_compile_options(executable PRIVATE -stdlib=libc++)
 set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
 target_compile_options(executable PRIVATE -stdlib=libstdc++)
 set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libstdc++")
endif()

target_link_libraries(executable
    assimp
    imgui
    glad
    glfw
    glm
    spdlog
    filameshio
    filament
    filamat
    filaflat
    filabridge
    geometry
    backend
    bluegl
    ibl
    image
    meshoptimizer
    smol-v
    utils
 )



target_include_directories(executable PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)