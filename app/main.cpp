#include <stdio.h>
#include <imgui.h>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main(){
    
    // print imgui version
    printf("imgui version: %s\n", IMGUI_VERSION);
    glm::vec3 v(1.0f, 0.0f, 0.0f);

    // print glm version
    printf("glm version: %d.%d.%d\n", GLM_VERSION_MAJOR, GLM_VERSION_MINOR, GLM_VERSION_PATCH);

    // print glad version
    // printf("glad version: %d.%d\n", GLVersion.major, GLVersion.minor);
// 
    // print glfw version
    int major, minor, revision; 
    glfwGetVersion(&major, &minor, &revision);
    printf("glfw version: %d.%d.%d\n", major, minor, revision);
    

    
}