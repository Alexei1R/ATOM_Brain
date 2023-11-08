//
// Created by toor on 11/8/23.
//

#ifndef ATOM_FRAMEBUFFER_H
#define ATOM_FRAMEBUFFER_H

#include "ATOM/Application.h"

namespace Atom {
    class Framebuffer : public Layer {
    public:
        Framebuffer();

        ~Framebuffer();

        virtual void OnAttach() override;

        virtual void OnDetach() override;

        virtual void OnUpdate() override;

        virtual void OnImGuiRender() override;

        void Bind();

        void UnBind();

        void SetFramebufferTextureSizeCallback(int width, int height);

        unsigned int GetFramebufferTexture() { return m_FramebufferTexture; }

    private:
        unsigned int width = 720;
        unsigned int height = 480;
        unsigned int m_FBO;
        unsigned int m_FramebufferTexture;


    };


}


#endif //ATOM_FRAMEBUFFER_H