cmake_minimum_required(VERSION 3.20)
project(imgui)


# print curent directory
message(STATUS "CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")


include_directories(${CMAKE_SOURCE_DIR}/SDL)
include_directories(${CMAKE_SOURCE_DIR}/SDL/include)
# # Include the directories where OpenGL and GLEW headers are located
include_directories(${CMAKE_SOURCE_DIR}/libs/glfw/include)
include_directories(${CMAKE_SOURCE_DIR}/libs/glad/include)



set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)


set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED)


set(ROOT ${IMGUI_DIR})

add_library(imgui STATIC



	${ROOT}/imgui.cpp
	${ROOT}/imgui.h
	${ROOT}/imstb_rectpack.h
	${ROOT}/imstb_textedit.h
	${ROOT}/imstb_truetype.h
    ${ROOT}/imgui_demo.cpp
    ${ROOT}/imgui_draw.cpp
    ${ROOT}/imgui_internal.h
    ${ROOT}/imgui_tables.cpp
    ${ROOT}/imgui_widgets.cpp




		${ROOT}/backends/imgui_impl_sdl3.h
		${ROOT}/backends/imgui_impl_sdl3.cpp
		${ROOT}/backends/imgui_impl_sdlrenderer3.h
		${ROOT}/backends/imgui_impl_sdlrenderer3.cpp

)

target_include_directories(imgui
  PUBLIC ${IMGUI_DIR}
  PUBLIC ${IMGUI_DIR}/backends
)



# Link against the OpenGL and GLEW libraries
target_link_libraries(imgui
		glfw
		glad
		SDL3::SDL3
)
