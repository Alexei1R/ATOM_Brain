//
// Created by toor on 11/7/23.
//

#ifndef ATOM_WINDOW_H
#define ATOM_WINDOW_H
#include "ATOM/atompch.h"

namespace Atom {


    struct WindowSpecs {
        std::string Title;
        unsigned int Width, Height;

        WindowSpecs(const std::string title = "Atom", int width = 1080, int height = 720)
                :Title(title), Width(width), Height(height) {}
    };



    class Window
    {
    public:
        using EventCallbackFn = std::function<void()>;

        virtual ~Window() = default;

        virtual void OnUpdate() = 0;
        virtual void ClearDisplay(glm::vec3 color) = 0;

        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;


        virtual std::pair<int,int > GetSize() const = 0;
        virtual std::pair<int,int > GetPosition() const = 0;

        virtual void MakeFullscreen() = 0;

        // Window attributes
        virtual void SetWindowCloseCallback(const EventCallbackFn& callback) = 0;
        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        virtual void* GetNativeWindow() const = 0;
        virtual void* GetRederer() const = 0;

        static Window* Create(const WindowSpecs& specs = WindowSpecs());
    };



}


#endif //ATOM_WINDOW_H
