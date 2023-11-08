//
// Created by toor on 11/7/23.
//

#ifndef ATOM_APPLICATION_H
#define ATOM_APPLICATION_H



#include "ATOM/Core/Layers/Layer.h"
#include "ATOM/Core/Layers/LayerStack.h"
#include "ATOM/Core/Window.h"
#include "ATOM/Core/Editor/ImGuiLayer.h"
#include "ATOM/Core/Editor/EditorLayer.h"


namespace Atom {
    class Application {
    public:
        Application();
        ~Application();
        void Run();
        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
        static Application* s_Instance;
        inline static Application& GetApp() { return *s_Instance; }
        inline  Window& GetWindow() { return *m_Window; }
        void WindowClose();
    private:
        bool m_IsRuning = true;
        Window* m_Window;
        LayerStack m_LayerStack;
        ImGuiLayer* m_ImGuiLayer;
        EditorLayer* m_EditorLayer;
    };
}





#endif //ATOM_APPLICATION_H
