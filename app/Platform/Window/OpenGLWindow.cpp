//
// Created by toor on 11/7/23.
//
#include "OpenGLWindow.h"


namespace Atom {


    //  Funcion created in Window class
    Window* Window::Create(const WindowSpecs& specs)
    {
        return new OpenGLWindow(specs);
    }


    static void GLFWErrorCallback(int error, const char* description)
    {
        ATLOG_ERROR("GLFW Error ({0}): {1}", error, description);
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
        //ATOM_ASSERT(success, "Could not intialize GLFW!");
        if(success != 1){ATLOG_CRITICAL("!Succes glfw") }
        glfwSetErrorCallback(GLFWErrorCallback);


        m_Window = glfwCreateWindow((int)m_Data.Width, (int)m_Data.Height, m_Data.Title.c_str(), nullptr, nullptr);



        glfwMakeContextCurrent(m_Window);
        int successGlad = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        //ATOM_ASSERT(successGlad, "Could not intialize GLFW!");
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
    }
    //close window
    void OpenGLWindow::Shutdown()
    {
        //glfwTerminate();
        //glfwDestroyWindow(m_Window);
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



    void OpenGLWindow::OnUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
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

        // Set GLFW callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {

        });



        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.m_EventCallback();
        });


        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            if (Keyboard::key(GLFW_KEY_F1)) { glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); }
            if (Keyboard::key(GLFW_KEY_F2)) { glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); }


            Keyboard::keyCallback(window, key, scancode, action, mods);

        });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
            Mouse::mouseButtonCallback(window, button, action, mods);
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            Mouse::mouseWheelCallback(window, xOffset, yOffset);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
        {
            Mouse::cursorPosCallback(window,xPos,yPos);
        });
    }


}