#pragma once

#include "../libs/imgui/imgui.h"
#include "../libs/imgui/imgui_impl_glfw.h"
#include "../libs/imgui/imgui_impl_opengl3.h"
#include "../libs/glad/include/glad/glad.h"


class Gui {

  public:
    void Init(GLFWwindow* window, const char* glsl_version);
    void NewFrame();
    void Render();
    void Shutdown(GLFWwindow* window);  
};

