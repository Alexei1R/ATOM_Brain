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
#include "ATOM/Core/Editor/FrameBuffer.h"

#include "ATOM/Core/ScreenData/DrawMap.h"
#include "ATOM/Sensors/ImuLayer.h"
#include "ATOM/Sensors/Frame.h"


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
        Framebuffer* m_Framebuffer;

        DrawMap* m_DrawMap;
        Frame* m_Frame;
    };
}





#endif //ATOM_APPLICATION_H
