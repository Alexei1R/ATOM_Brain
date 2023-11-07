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

            ATLOG_INFO("Some info");

        }
    }











}