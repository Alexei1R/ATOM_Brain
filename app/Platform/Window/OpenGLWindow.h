//
// Created by toor on 11/7/23.
//

#ifndef ATOM_OPENGLWINDOW_H
#define ATOM_OPENGLWINDOW_H
#include "ATOM/atompch.h"
#include "ATOM/Core/Window.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "ATOM/Core/Event/Mause.h"
#include "ATOM/Core/Event/Keyboard.h"

namespace Atom {

    class OpenGLWindow : public Window {

    public:
        OpenGLWindow(const WindowSpecs& spec);
        ~OpenGLWindow();


        void OnUpdate()override;

        inline  unsigned int GetWidth()const override {return m_Data.Width;};
        inline unsigned int GetHeight()const override { return m_Data.Height; };

        // Window attributes
        void SetWindowCloseCallback(const EventCallbackFn& callback) override { m_EventCallback = callback; }
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        void* GetNativeWindow() const override { return m_Window; };

        void SetCallbacks();
    private:
        virtual void Init(const WindowSpecs& specs);
        virtual void Shutdown();

    private:

        GLFWwindow* m_Window;
        EventCallbackFn m_EventCallback;


        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

        };

        WindowData m_Data;


    };



}
#endif //ATOM_OPENGLWINDOW_H
