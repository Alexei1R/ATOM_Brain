//
// Created by toor on 11/10/23.
//

#include "DrawMap.h"

namespace Atom {
    DrawMap::DrawMap()
            :Layer("DrawMap")
    {
        m_Camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
        m_Shader = new Shader("ASSETS/vs.glsl","ASSETS/fs.glsl");
        m_Shader->Bind();
        m_Model = new Model();
        m_Model->loadModel("ASSETS/t.fbx");
        m_Transform = new Transform(*m_Shader);
        m_Transform->rotateRadians(180.0f, glm::vec3(0.0f, 0.0f, 1.0f));
        m_Transform->transform(glm::vec3(0.0f, -5.0f, 0.0f));
    }

    DrawMap::~DrawMap()
    {
        delete m_Camera;
        delete m_Shader;
        delete m_Model;
    }



    void DrawMap::OnAttach()
    {
    }
    void DrawMap::OnDetach()
    {
    }
    void DrawMap::OnUpdate()
    {



    }
    void DrawMap::OnImGuiRender()
    {

    }

    void DrawMap::DrawModel(int width, int height) {
        m_Transform->UpdateCam(m_Camera->GetViewMatrix(), m_Camera->GetCameraPos(), m_Camera->GetCameraFront(),width,height);
        m_Camera->Update();
        m_Shader->Bind();
        m_Model->Draw(*m_Shader);
        m_Shader->UnBind();
    }


}