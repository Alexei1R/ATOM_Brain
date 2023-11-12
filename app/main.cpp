#  define USE_GL_LINUX USE_GL
#  define GLFW_EXPOSE_NATIVE_X11
#  define GLFW_EXPOSE_NATIVE_GLX

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <filament/Engine.h>
#include <filament/Renderer.h>
#include <filament/SwapChain.h>
#include <filament/Scene.h>
#include <filament/View.h>
#include <filament/Viewport.h>
#include <filament/Camera.h>
#include <filament/TransformManager.h>
#include <filament/Texture.h>
#include <filament/RenderTarget.h>
#include <math/mat3.h>
#include <math/mat4.h>
#include <utils/Path.h>
#include <utils/EntityManager.h>
#include <filameshio/MeshReader.h>

#ifdef Success
#undef Success
#endif
#include <filament/LightManager.h>
#include <filament/IndirectLight.h>


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}


int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    GLFWwindow* window = glfwCreateWindow(800, 600, "GLFW Window", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    glfwMakeContextCurrent(nullptr);


    filament::Engine* fila_engine = filament::Engine::create(
            filament::backend::Backend::OPENGL,
            nullptr,
            (void*)glfwGetGLXContext(window)
    );

    if (nullptr == fila_engine) {
        printf("Failled to create the filament::Engine. (exiting)\n");
        exit(EXIT_FAILURE);
    }

    filament::SwapChain* fila_swap_chain = fila_engine->createSwapChain((void*)glfwGetX11Window(window));
    if (nullptr == fila_swap_chain) {
        printf("Failed to create the filament::SwapChain. (exiting)\n");
        exit(EXIT_FAILURE);
    }


    filament::Renderer* fila_renderer = fila_engine->createRenderer();
    if (nullptr == fila_renderer) {
        printf("Failed to create the filament::Renderer. (exiting)\n");
        exit(EXIT_FAILURE);
    }

    filament::Scene* fila_scene = fila_engine->createScene();
    if (nullptr == fila_scene) {
        printf("Failed to create the filament::Scene. (exiting)\n");
        exit(EXIT_FAILURE);
    }

    filament::View* fila_view = fila_engine->createView();
    if (nullptr == fila_view) {
        printf("Failed to create the filament::View. (exiting)\n");
        exit(EXIT_FAILURE);
    }
    utils::Entity fila_view_entity = utils::EntityManager::get().create();
    filament::Camera* fila_cam = fila_engine->createCamera(fila_view_entity);
    if (nullptr == fila_cam) {
        printf("Failed to create a filament::Camera. (exting)\n");
        exit(EXIT_FAILURE);
    }

    fila_cam->setExposure(16.0f, 1 / 125.0f, 100.0f);
    fila_cam->setExposure(100.0f);
    fila_cam->setProjection(45.0f, float(800)/600, 0.1f, 100.0f);
    fila_cam->lookAt({0, 0, 10.0}, {0, 0, 0}, {0, 1, 0});

    fila_view->setViewport({0, 0, 800, 600});
    fila_view->setScene(fila_scene);
    fila_view->setCamera(fila_cam); /* When we don't set the camera we run into a segfault. */
    fila_view->setName("main-view");


    fila_renderer->setClearOptions({
                                           .clearColor = { 1.0f, 0.13f, 0.0f, 1.0f },
                                           .clear = true
                                   });



    filamesh::MeshReader::MaterialRegistry material_registry;
    filamesh::MeshReader::Mesh mesh;
    utils::Path mesh_path("/home/toor/Desktop/filament-with-glfw/install/bin//monkey.filamesh");
    mesh = filamesh::MeshReader::loadMeshFromFile(
            fila_engine,
            mesh_path,
            material_registry
    );
    fila_scene->addEntity(mesh.renderable);











    while (!glfwWindowShouldClose(window)) {

//        glfwMakeContextCurrent(window);
//        glClearColor(0.1f, 0.1f, 0.13f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (true == fila_renderer->beginFrame(fila_swap_chain)) {
            fila_renderer->render(fila_view);
            fila_renderer->endFrame();
        }

        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Terminate GLFW
    glfwTerminate();

    return 0;
}