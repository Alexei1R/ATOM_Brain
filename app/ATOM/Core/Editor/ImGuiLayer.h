//
// Created by toor on 11/8/23.
//

#ifndef ATOM_IMGUILAYER_H
#define ATOM_IMGUILAYER_H
#include "ATOM/atompch.h"

namespace Atom {
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() = default;

        void OnAttach() override;
        void OnDetach() override;

        void Begin();
        void End();


        void SetDarkThemeColors();
    };
}


#endif //ATOM_IMGUILAYER_H
