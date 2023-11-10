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
        m_Camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
        m_Shader = new Shader("ASSETS/vs.glsl","ASSETS/fs.glsl");
        m_Shader->Bind();
        m_Model = new Model();
        m_Model->loadModel("ASSETS/Traseu.fbx");


        m_Transform = new Transform(*m_Shader);

    }


    Application::~Application()
    {
        delete m_Framebuffer;
        delete m_Window;
        delete m_ImGuiLayer;
        delete m_EditorLayer;


        delete m_Camera;
        delete m_Shader;
        delete m_Model;
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
            m_Transform->UpdateCam(m_Camera->GetViewMatrix(), m_Camera->GetCameraPos(), m_Camera->GetCameraFront(),m_Window->GetHeight(),m_Window->GetWidth());
            m_Camera->Update();


            m_Framebuffer->Bind();


            glClearColor(0.8, 0.8, 0.8, 1.0);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            m_Shader->Bind();
            m_Model->Draw(*m_Shader);
            m_Shader->UnBind();
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


//            ATLOG_INFO("Some info");
            m_Window->OnUpdate();

        }
    }

    void Application::WindowClose() {
        m_IsRuning = false;
    }


}