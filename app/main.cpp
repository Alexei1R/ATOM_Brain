#include <GLFW/glfw3.h>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}




// #include <spdlog/spdlog.h>
// #include "ATOM/Application.h"
// #include "ATOM/Core/Loging/Log.h"
// #include <filesystem>
//
// int main(){
//     Atom::Log::Init();
//     ATLOG_INFO("Initialized Log!");
//     ATLOG_INFO("Curent path: {0}", std::filesystem::current_path().string());
//     std::filesystem::path current_path = std::filesystem::current_path();
//     try {
//         std::string path;
//         current_path = current_path.parent_path().parent_path();
//         path = current_path.string() + "/app";
//         std::filesystem::current_path(path);
//         ATLOG_INFO("Curent path  set to: {0}", path);
//     } catch (const std::filesystem::filesystem_error& ex) {
//         ATLOG_ERROR("Error setting current path:  {0}", ex.what());
//     }
//
//     Atom::Application* app = new Atom::Application();
//     app->Run();
//     delete app;
// }