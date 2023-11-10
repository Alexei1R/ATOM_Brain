//
// Created by toor on 11/10/23.
//

#ifndef ATOM_DRAWMAP_H
#define ATOM_DRAWMAP_H
#include "ATOM/atompch.h"
#include "ATOM/Core/Renderer/Shader.h"
#include "ATOM/Core/Renderer/Model.h"
#include "ATOM/Core/Renderer/Camera.h"
#include "ATOM/Core/Renderer/Transform.h"


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
        Camera* m_Camera;
        Shader* m_Shader;
        Model* m_Model;
        Transform* m_Transform;
    };




}




#endif //ATOM_DRAWMAP_H
