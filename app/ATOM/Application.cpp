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
        m_Window->SetVSync(true);
        m_Window->SetWindowCloseCallback(BIND_EVENT_FN(WindowClose));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);





        std::function<void()> imdraw = [&]() {


            ImGui::Begin("Settings");

            ImGui::Text("Renderer2D Stats:");

            ImGui::Text("Settins test");
            ImGui::End();
        };


    }


    Application::~Application()
    {
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
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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


//            ATLOG_INFO("Some info");
            m_Window->OnUpdate();

        }
    }

    void Application::WindowClose() {
        m_IsRuning = false;
    }


}