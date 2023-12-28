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


        std::pair<int, int> GetSize() const override;
        std::pair<int, int> GetPosition() const override;


        // Window attributes
        void SetWindowCloseCallback(const EventCallbackFn& callback) override { m_Data.m_EventCallback = callback; }
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        void* GetNativeWindow() const override { return m_Window; };

        void SetCallbacks();
    private:
        virtual void Init(const WindowSpecs& specs);
        virtual void Shutdown();

    private:

        GLFWwindow* m_Window;



        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;
            EventCallbackFn m_EventCallback;
        };

        WindowData m_Data;


    };



}
#endif //ATOM_OPENGLWINDOW_H
