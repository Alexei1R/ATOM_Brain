//
// Created by toor on 11/8/23.
//

#ifndef ATOM_IMGUILAYER_H
#define ATOM_IMGUILAYER_H
#include "ATOM/atompch.h"

namespace Atom {
    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer();

        ~ImGuiLayer() = default;

        virtual void OnAttach() override;

        virtual void OnDetach() override;

        virtual void OnUpdate() override;

        virtual void OnFixedUpdate() override;

        virtual void OnImGuiRender() override;

        void Begin();

        void End();

    private:
        void SetDarkThemeColors();

    private:
        GLFWwindow *window;
        ImGuiIO *io;
        ImGuiWindowFlags window_flags;
        bool dockspaceOpen = true;
    };
}


#endif //ATOM_IMGUILAYER_H
