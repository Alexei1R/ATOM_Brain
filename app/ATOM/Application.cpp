//
// Created by toor on 11/7/23.
//
#include "atompch.h"
#include "Application.h"
#include "imgui.h"
#include "SDL3/SDL_filesystem.h"

namespace Atom
{
    char inputBuffer[256] = "/home/toor/Downloads/pc.mp4";
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
        m_Frame = new Frame();
        PushLayer(m_Frame);

        m_ClientLayer->RegisterMessageWithID(2, [&](Message message)
        {
            ATLOG_INFO("Message Received: ID = 2 {0}", *(int *) message.payload);
        });

        m_ClientLayer->RegisterMessageWithID(50, [&](Message message)
        {
            //string if equal to OK
            std::string data = static_cast<char*>(message.payload);
            if (data == "OK")
            {
                // m_Frame = new Frame();
                // PushLayer(m_Frame);
            }
            else
            {
                ATLOG_CRITICAL("Error to open camera")
            }
        });

        std::function<void()> drawPopUp = [&]()
        {
            SelectIPPopUpWindow();
        };
        m_EditorLayer->AddDrawCallback(drawPopUp);

        std::function<void()> draw = [&]()
        {
            //the panel is 20% of the screen
            auto mainWindowSizePair = m_Window->GetSize();
            ImVec2 mainWindowSize = ImVec2(static_cast<float>(mainWindowSizePair.first),
                                           static_cast<float>(mainWindowSizePair.second));
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImVec2(mainWindowSize.x * 0.2f, mainWindowSize.y));
            //hide tab bar and menu bar
            ImGui::Begin("##Control Panel", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove |
                                                       ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                                                       ImGuiWindowFlags_NoSavedSettings);

            ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Control Panel").x) * 0.5f);
            ImGui::TextColored(ImVec4(0, 255, 255, 255), "Control Panel");
            ImGui::Separator();


            ImGui::Spacing();
            ImGui::Text("Open Camera");

            ImGui::BeginTabBar("##TabBar", ImGuiTabBarFlags_None);
            if (ImGui::BeginTabItem(" Custom "))
            {

                // ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Enter Pipeline").x) * 0.5f);
                ImGui::Text("Enter Pipeline");
                ImGui::InputText("##InputText", inputBuffer, IM_ARRAYSIZE(inputBuffer));
                ImGui::EndTabItem();
                if(ImGui::Button("Open Camera"))
                {
                    if (m_ClientLayer->IsRunning()) {
                        std::string data = inputBuffer;
                        Message message;
                        message.id = 50;
                        message.payloadSize = data.size();
                        message.payload = static_cast<void*>(const_cast<char*>(data.c_str()));
                        m_ClientLayer->SendMessage(message);
                    }

                }
            }
            if (ImGui::BeginTabItem(" Default "))
            {
                static int m_ComboIndex = 0;
                static const char* comboItems[] = {
                    "0",
                    "1",
                };
                ImGui::Combo("##Combo", &m_ComboIndex, comboItems, IM_ARRAYSIZE(comboItems));
                if(ImGui::Button("Open Camera"))
                {
                    if (m_ClientLayer->IsRunning()) {
                        std::string data = comboItems[m_ComboIndex];
                        Message message;
                        message.id = 50;
                        message.payloadSize = data.size();
                        message.payload = static_cast<void*>(const_cast<char*>(data.c_str()));
                        m_ClientLayer->SendMessage(message);
                    }

                }

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();


            ImGui::Separator();




            ImGui::End();





        };
        m_EditorLayer->AddDrawCallback(draw);



    }


    Application::~Application()
    {
        delete m_Window;
        delete m_ImGuiLayer;
        delete m_EditorLayer;
        delete m_ClientLayer;
        delete m_Frame;
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
            m_Window->ClearDisplay(glm::vec3(0, 255, 255));
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                m_ImGuiLayer->ProcesEvent(event);
                if (event.type == SDL_EVENT_QUIT)
                {
                    WindowClose();
                }
            }


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
                    static char inputBuffer[256] = "192.168.1.8";
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
