//
// Created by toor on 1/13/24.
//

#ifndef SDLWINDOW_H
#define SDLWINDOW_H


#include "ATOM/atompch.h"
#include "ATOM/Core/Window.h"



class SDLWindow {

};


namespace Atom {

    class SDLWindow : public Window {

    public:
        SDLWindow(const WindowSpecs& spec);
        ~SDLWindow();


        void OnUpdate()override;
        void ClearDisplay(glm::vec3 color) override;

        inline  unsigned int GetWidth()const override {return m_Data.Width;};
        inline unsigned int GetHeight()const override { return m_Data.Height; };


        std::pair<int, int> GetSize() const override;
        std::pair<int, int> GetPosition() const override;


        // Window attributes
        void SetWindowCloseCallback(const EventCallbackFn& callback) override { m_Data.m_EventCallback = callback; }
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        void* GetNativeWindow() const override { return m_Window; };
        void* GetRederer() const override { return m_Renderer; };

        void SetCallbacks();
    private:
        virtual void Init(const WindowSpecs& specs);
        virtual void Shutdown();

    private:

        SDL_Window* m_Window;
        SDL_Renderer* m_Renderer;



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

#endif //SDLWINDOW_H
