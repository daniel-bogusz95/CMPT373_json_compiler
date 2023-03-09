cmake_minimum_required (VERSION 3.8)

add_executable(guiTest main.cpp gui.cpp)

target_link_libraries(guiTest PUBLIC imgui glfw glad)


