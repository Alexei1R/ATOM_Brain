//
// Created by toor on 11/8/23.
//

#include "FrameBuffer.h"
#include "ATOM/Application.h"

namespace Atom {
    Framebuffer::Framebuffer()
            : Layer("Framebuffer") {


    }

    Framebuffer::~Framebuffer() {
    }


    void Framebuffer::OnAttach() {
        Application &m_App = Application::GetApp();
        Window *m_Window = static_cast<Window *>(&m_App.GetWindow());

        glGenFramebuffers(1, &m_FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
        // Create Framebuffer Texture
        glGenTextures(1, &m_FramebufferTexture);
        glBindTexture(GL_TEXTURE_2D, m_FramebufferTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Window->GetWidth(), m_Window->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                     NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FramebufferTexture, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


    }

    void Framebuffer::Bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    }

    void Framebuffer::UnBind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::SetFramebufferTextureSizeCallback(int width, int height) {
        this->width = width;
        this->height = height;
        glGenFramebuffers(1, &m_FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
        // Create Framebuffer Texture
        glGenTextures(1, &m_FramebufferTexture);
        glBindTexture(GL_TEXTURE_2D, m_FramebufferTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_FramebufferTexture, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }


    void Framebuffer::OnDetach() {
    }

    void Framebuffer::OnUpdate() {

    }

    void Framebuffer::OnImGuiRender() {
    }


}