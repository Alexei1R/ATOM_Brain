//
// Created by toor on 11/7/23.
//
#include "atompch.h"
#include "Application.h"
#include "imgui.h"

namespace Atom
{
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        s_Instance = (Application*)this;

        m_Window = Window::Create();
        m_Window->SetVSync(false);
        m_Window->SetWindowCloseCallback(BIND_EVENT_FN(WindowClose));


        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
        m_EditorLayer = new EditorLayer();
        PushOverlay(m_EditorLayer);
        m_ClientLayer = new ClientLayer();
        PushLayer(m_ClientLayer);

        m_ClientLayer->RegisterMessageWithID(2, [&](Message message)
        {
            ATLOG_INFO("Message Received: ID = 2 {0}", *(int *) message.payload);
        });
        std::function<void()> drawPopUp = [&]()
        {
            SelectIPPopUpWindow();
        };
        m_EditorLayer->AddDrawCallback(drawPopUp);


        // m_Frame = new Frame();
        // PushLayer(m_Frame);
    }


    Application::~Application()
    {
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
        ATLOG_WARN("Begin Runing");
        while (m_IsRuning)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                m_ImGuiLayer->ProcesEvent(event);
                if (event.type == SDL_EVENT_QUIT)
                {
                    WindowClose();
                }

                if (event.type == SDL_EVENT_WINDOW_RESIZED)
                {
                    SDL_Log("Window %d resized to %dx%d",
                            event.window.windowID, event.window.data1,
                            event.window.data2);
                }
            }
            auto time = SDL_GetTicks() / 10000.f;
            auto red = (std::sin(time) + 1) / 2.0 * 255;
            auto green = (std::sin(time / 2) + 1) / 2.0 * 255;
            auto blue = (std::sin(time) * 2 + 1) / 2.0 * 255;
            m_Window->ClearDisplay(glm::vec3(red, green, blue));

            for (Layer* layer : m_LayerStack)
            {
                layer->OnUpdate();
            }
            m_ImGuiLayer->Begin();
            for (Layer *layer: m_LayerStack) {
                layer->OnImGuiRender();
            }
            m_ImGuiLayer->End();


            m_Window->OnUpdate();
        }
    }

    void Application::WindowClose()
    {
        m_IsRuning = false;
    }


    void Application::SelectIPPopUpWindow()
    {
        if (!isConnected)
        {
            auto mainWindowSizePair = m_Window->GetSize();
            ImVec2 mainWindowSize = ImVec2(static_cast<float>(mainWindowSizePair.first),
                                           static_cast<float>(mainWindowSizePair.second));

            auto mainWindowPositionPair = m_Window->GetPosition();
            ImVec2 mainWindowPosition = ImVec2(static_cast<float>(mainWindowPositionPair.first),
                                               static_cast<float>(mainWindowPositionPair.second));

            ImVec2 popupSize = ImVec2(300, 200);
            ImVec2 popupPos = ImVec2(mainWindowPosition.x + (mainWindowSize.x - popupSize.x) * 0.5f,
                                     mainWindowPosition.y + (mainWindowSize.y - popupSize.y) * 0.5f);

            ImGui::SetNextWindowPos(popupPos);
            ImGui::SetNextWindowSize(popupSize);


            ImGui::OpenPopup("Select IP");
            if (ImGui::BeginPopupModal("Select IP", nullptr, 0))
            {
                enum MenuItemsIndex
                {
                    SelectIP,
                    DefaultIP,
                };
                static const char* menuItems[] = {
                    "Select IP",
                    "Default IP",
                };
                static int m_IPIndex = 0;
                ImGui::Combo("IP", &m_IPIndex, menuItems, IM_ARRAYSIZE(menuItems));

                if (m_IPIndex == SelectIP)
                {
                    static char inputBuffer[256] = "192.168.1.102";
                    ImGui::InputText("Enter IP", inputBuffer, IM_ARRAYSIZE(inputBuffer));
                    if (ImGui::Button("Connect"))
                    {
                        isConnected = true;
                        std::string ip = inputBuffer;
                        ip.append(":27020");
                        m_ClientLayer->ConnectToServer(ip);
                    }
                }
                else if (m_IPIndex == DefaultIP)
                {
                    if (ImGui::Button("Connect with Default IP"))
                    {
                        isConnected = true;
                        m_ClientLayer->ConnectToServer("192.168.100.119:27020");
                    }
                }
            }
            ImGui::EndPopup();
        }
    }
}
