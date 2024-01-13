//
// Created by toor on 11/10/23.
//

#ifndef ATOM_DRAWMAP_H
#define ATOM_DRAWMAP_H
#include "ATOM/atompch.h"



namespace Atom {
    class DrawMap : public Layer {
    public:
        DrawMap();
        ~DrawMap();
        virtual void OnAttach()override;
        virtual void OnDetach()override;
        virtual void OnUpdate()override;
        virtual void OnImGuiRender() override;

        void DrawModel(int width, int height);

    private:
    };




}




#endif //ATOM_DRAWMAP_H
