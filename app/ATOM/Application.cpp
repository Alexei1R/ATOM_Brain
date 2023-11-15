//
// Created by toor on 11/7/23.
//
#include "atompch.h"
#include "Application.h"
#include "imgui.h"

namespace Atom {
    Application* Application::s_Instance = nullptr;
    Application::Application()
    {
        s_Instance = (Application*)this;

        m_Window = Window::Create();
        m_Window->SetVSync(false);
        m_Window->SetWindowCloseCallback(BIND_EVENT_FN(WindowClose));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        m_Framebuffer = new Framebuffer();
        PushLayer(m_Framebuffer);

        m_EditorLayer = new EditorLayer(m_Framebuffer);
        PushLayer(m_EditorLayer);



        std::function<void()> imdraw = [&]() {


            ImGui::Begin("Settings");

            ImGui::Text("FPS : %f", ImGui::GetIO().Framerate);

            ImGui::Text("Settins test");
            ImGui::End();
        };
        m_EditorLayer->AddDrawCallback(imdraw);


        m_DrawMap = new DrawMap();
        PushLayer(m_DrawMap);


        m_ImuLayer = new ImuLayer();
        PushLayer(m_ImuLayer);


    }


    Application::~Application()
    {
        delete m_Framebuffer;
        delete m_Window;
        delete m_ImGuiLayer;
        delete m_EditorLayer;
    }





    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer* layer)
    {
        m_LayerStack.PushOverlay(layer);
    }


    void Application::Run()
    {


        while (m_IsRuning)
        {

            glClearColor(0.8, 0.8, 0.8, 1.0);


            m_Framebuffer->Bind();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            m_DrawMap->DrawModel(m_Window->GetWidth(), m_Window->GetHeight());
            m_Framebuffer->UnBind();


            for (Layer* layer : m_LayerStack)
            {
                layer->OnUpdate();
            }

            m_ImGuiLayer->Begin();
            {
                for (Layer* layer : m_LayerStack)
                    layer->OnImGuiRender();
            }
            m_ImGuiLayer->End();


            m_Window->OnUpdate();

        }
    }

    void Application::WindowClose() {
        m_IsRuning = false;
    }


}