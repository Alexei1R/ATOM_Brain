//
// Created by toor on 1/23/24.
//

#include "OpenGLWindow.h"
#include "ATOM/atompch.h"




namespace Atom {

    static void GLFWErrorCallback(int error, const char* description)
    {
        ATLOG_ERROR("GLFW Error ({0}): {1}", error, description);
    }


    Window* Window::Create(const WindowSpecs& specs)
    {
        return new OpenGLWindow(specs);
    }

    OpenGLWindow::OpenGLWindow(const WindowSpecs& spec)
    {
        Init(spec);
    }

    OpenGLWindow::~OpenGLWindow()
    {
        Shutdown();
    }


    // init OpenGl window stuff
    void OpenGLWindow::Init(const WindowSpecs& specs)
    {
        m_Data.Title = specs.Title;
        m_Data.Width = specs.Width;
        m_Data.Height = specs.Height;

        ATLOG_INFO("Creating Window ::  {0}, {1}:{2}", m_Data.Title, m_Data.Width, m_Data.Height)
        int success = glfwInit();
        ATOM_ASSERT(success, "Could not intialize GLFW!");
        if(success != 1){ATLOG_CRITICAL("!Succes glfw") }
        glfwSetErrorCallback(GLFWErrorCallback);
        m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), m_Data.Fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);
        int version = gladLoadGL(glfwGetProcAddress);
        if (!version)
        {
            ATOM_ASSERT(version, "Could not intialize GLFW!");
            std::cout << "Failed to initialize OpenGL context" << std::endl;
            return;
        }
        ATLOG_INFO("OpenGL Info:");
        ATLOG_INFO("  Vendor: {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
        ATLOG_INFO("  Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
        ATLOG_INFO("  Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));




        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);
        SetCallbacks();
        // Enables the Depth Buffer
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glfwWindowHint(GLFW_DEPTH_BITS, 24);
        SetVSync(true);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        SetCallbacks();
    }
    //close window
    void OpenGLWindow::Shutdown()
    {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    std::pair<int, int> OpenGLWindow::GetSize() const {
        int width, height;
        glfwGetWindowSize(m_Window, &width, &height);
        return {width, height};
    }

    std::pair<int, int> OpenGLWindow::GetPosition() const {
        int xpos, ypos;
        glfwGetWindowPos(m_Window, &xpos, &ypos);
        return {xpos, ypos};
    }

    void OpenGLWindow::MakeFullscreen() {
        glfwSetWindowMonitor(m_Window, glfwGetPrimaryMonitor(), 0, 0, m_Data.Width, m_Data.Height, GLFW_DONT_CARE);
    }


    void OpenGLWindow::OnUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    void OpenGLWindow::ClearDisplay(glm::vec3 color)
    {
        glClearColor(color.r, color.g, color.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void OpenGLWindow::SetVSync(bool enabled)
    {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);
        m_Data.VSync = enabled;
    }

    bool OpenGLWindow::IsVSync() const
    {
        return m_Data.VSync;
    }

    void OpenGLWindow::SetCallbacks()
    {
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            //resize viewport
            glViewport(0, 0, width, height);
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.Width = width;
            data.Height = height;

        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.m_EventCallback();
        });


    }


}

