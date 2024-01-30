//
// Created by toor on 1/23/24.
//

#ifndef OPENGLWINDOW_H
#define OPENGLWINDOW_H


#include "ATOM/atompch.h"
#include "ATOM/Core/Window.h"



namespace Atom {

    class OpenGLWindow : public Window {

    public:
        OpenGLWindow(const WindowSpecs& spec);
        ~OpenGLWindow();


        void OnUpdate()override;
        void ClearDisplay(glm::vec3 color) override;

        inline  unsigned int GetWidth()const override {return m_Data.Width;};
        inline unsigned int GetHeight()const override { return m_Data.Height; };


        std::pair<int, int> GetSize() const override;
        std::pair<int, int> GetPosition() const override;

        void MakeFullscreen() override;


        // Window attributes
        void SetWindowCloseCallback(const EventCallbackFn& callback) override { m_Data.m_EventCallback = callback; }
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        void* GetNativeWindow() const override { return m_Window; };
        void* GetRederer() const override { return nullptr; };

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
            bool Fullscreen = false;
            bool VSync;
            EventCallbackFn m_EventCallback;
        };

        WindowData m_Data;


    };



}



#endif //OPENGLWINDOW_H
