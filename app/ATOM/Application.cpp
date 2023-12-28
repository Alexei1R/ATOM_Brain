//
// Created by toor on 11/7/23.
//
#include "atompch.h"
#include "Application.h"
#include "imgui.h"

namespace Atom {


    Application *Application::s_Instance = nullptr;

    Application::Application() {
        s_Instance = (Application *) this;

        m_Window = Window::Create();
        m_Window->SetVSync(false);
        m_Window->SetWindowCloseCallback(BIND_EVENT_FN(WindowClose));


        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
        m_Framebuffer = new Framebuffer();
        PushLayer(m_Framebuffer);
        m_EditorLayer = new EditorLayer(m_Framebuffer);
        PushOverlay(m_EditorLayer);
        m_Frame = new Frame();
        PushLayer(m_Frame);
        m_ClientLayer = new ClientLayer();
        PushLayer(m_ClientLayer);

        m_ClientLayer->RegisterMessageWithID(2, [&](Message message) {
            ATLOG_INFO("Message Received: ID = 2 {0}", *(int *) message.payload);
        });





        std::function<void()> drawPopUp = [&]() {
            SelectIPPopUpWindow();
        };
        m_EditorLayer->AddDrawCallback(drawPopUp);

        std::function<void()> test = [&]() {
            // SelectIPPopUpWindow();
            ImGui::Begin("Test");
            ImGui::Text("Hello World");


            //text imput
            static char inputBuffer[256] = "Default Text";
            ImGui::InputText("Enter Text", inputBuffer, IM_ARRAYSIZE(inputBuffer));

            if (ImGui::Button("Send Data")) {
                if (m_ClientLayer->IsRunning()) {
                    std::string data = inputBuffer;
                    Message message;
                    message.id = 1;  // Set message ID
                    message.payloadSize = data.size();  // Set the size of the payload
                    message.payload = static_cast<void*>(const_cast<char*>(data.c_str())); // Set the payload
                    m_ClientLayer->SendMessage(message);

                    int data2 = 123;
                    Message message2;
                    message2.id = 2;  // Set message ID
                    message2.payloadSize = sizeof(data2);  // Set the size of the payload
                    message2.payload = static_cast<void*>(&data2); // Set the payload
                    m_ClientLayer->SendMessage(message2);

                    //id 3 , glm::vec3
                    glm::vec3 data3 = glm::vec3(1, 2, 3);
                    Message message3;
                    message3.id = 3;  // Set message ID
                    message3.payloadSize = sizeof(data3);  // Set the size of the payload
                    message3.payload = static_cast<void*>(&data3); // Set the payload
                    m_ClientLayer->SendMessage(message3);

                }
            }





            ImGui::End();
        };
        m_EditorLayer->AddDrawCallback(test);







        m_DrawMap = new DrawMap();
        PushLayer(m_DrawMap);



    }


    Application::~Application() {
        delete m_Framebuffer;
        delete m_Window;
        delete m_ImGuiLayer;
        delete m_EditorLayer;
    }


    void Application::PushLayer(Layer *layer) {
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer *layer) {
        m_LayerStack.PushOverlay(layer);
    }


    void Application::Run() {


        while (m_IsRuning) {

            glClearColor(0.8, 0.8, 0.8, 1.0);


            m_Framebuffer->Bind();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            m_DrawMap->DrawModel(m_Window->GetWidth(), m_Window->GetHeight());
            m_Framebuffer->UnBind();



            for (Layer *layer: m_LayerStack) {
                layer->OnUpdate();
            }
            m_ImGuiLayer->Begin();
            {
                for (Layer *layer: m_LayerStack)
                    layer->OnImGuiRender();
            }
            m_ImGuiLayer->End();



            m_Window->OnUpdate();

        }
    }

    void Application::WindowClose() {
        m_IsRuning = false;
    }




    void Application::SelectIPPopUpWindow() {
        if (!isConnected) {

            auto mainWindowSizePair = m_Window->GetSize();
            ImVec2 mainWindowSize = ImVec2(static_cast<float>(mainWindowSizePair.first), static_cast<float>(mainWindowSizePair.second));

            auto mainWindowPositionPair = m_Window->GetPosition();
            ImVec2 mainWindowPosition = ImVec2(static_cast<float>(mainWindowPositionPair.first), static_cast<float>(mainWindowPositionPair.second));

            ImVec2 popupSize = ImVec2(300, 200);
            ImVec2 popupPos = ImVec2(mainWindowPosition.x + (mainWindowSize.x - popupSize.x) * 0.5f,
                                     mainWindowPosition.y + (mainWindowSize.y - popupSize.y) * 0.5f);

            ImGui::SetNextWindowPos(popupPos);
            ImGui::SetNextWindowSize(popupSize);



            ImGui::OpenPopup("Select IP");
            if (ImGui::BeginPopupModal("Select IP", nullptr, 0)) {
                enum MenuItemsIndex {
                    SelectIP,
                    DefaultIP,
                };
                static const char *menuItems[] = {
                        "Select IP",
                        "Default IP",
                };
                static int m_IPIndex = 0;
                ImGui::Combo("IP", &m_IPIndex, menuItems, IM_ARRAYSIZE(menuItems));

                if (m_IPIndex == SelectIP) {
                    static char inputBuffer[256] = "192.168.100.119";
                    ImGui::InputText("Enter IP", inputBuffer, IM_ARRAYSIZE(inputBuffer));
                    if (ImGui::Button("Connect")) {
                        isConnected = true;
                        std::string ip = inputBuffer;
                        ip.append(":27020");
                        m_ClientLayer->ConnectToServer(ip);


                    }
                } else if (m_IPIndex == DefaultIP) {
                    if (ImGui::Button("Connect with Default IP")) {
                        isConnected = true;
                        m_ClientLayer->ConnectToServer("192.168.100.119:27020");

                    }
                }
            }
            ImGui::EndPopup();
        }
    }



}