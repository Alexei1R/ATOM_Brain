//
// Created by toor on 11/7/23.
//

#ifndef ATOM_APPLICATION_H
#define ATOM_APPLICATION_H



#include "ATOM/Core/Layers/Layer.h"
#include "ATOM/Core/Layers/LayerStack.h"
#include "ATOM/Core/Window.h"

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

    private:
        bool m_IsRuning = true;
        Window* m_Window;
        LayerStack m_LayerStack;


    };
}





#endif //ATOM_APPLICATION_H
