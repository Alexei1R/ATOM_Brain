//
// Created by toor on 11/8/23.
//

#ifndef ATOM_EDITORLAYER_H
#define ATOM_EDITORLAYER_H
#include "ATOM/atompch.h"
#include "imgui.h"

namespace Atom {

    class EditorLayer : public Layer {
    public:
        EditorLayer();

        ~EditorLayer();

        virtual void OnAttach() override;

        virtual void OnDetach() override;

        virtual void OnUpdate() override;

        virtual void OnFixedUpdate() override;

        virtual void OnImGuiRender() override;

        void AddDrawCallback(std::function<void()>& func) { m_FunctionsVenctor.push_back(func); }

        void SetViewportSizeCallback(std::function<void(int, int)> callback) {
            m_ViewportCallbackFunction = callback;
        }

    private:
        void DrawMenu();

        std::vector<std::function<void()>> m_FunctionsVenctor;
        std::function<void(int, int)> m_ViewportCallbackFunction;
        ImVec2 previousWindowSize = ImVec2(0, 0);

    };
}


#endif //ATOM_EDITORLAYER_H
