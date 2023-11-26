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
        m_Window->SetVSync(true);
        m_Window->SetWindowCloseCallback(BIND_EVENT_FN(WindowClose));

        Init();

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



    void Application::Init() {
        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
        m_Framebuffer = new Framebuffer();
        PushLayer(m_Framebuffer);
        m_EditorLayer = new EditorLayer(m_Framebuffer);
        PushLayer(m_EditorLayer);
        m_Frame = new Frame();
        PushLayer(m_Frame);

        InitClient();

        std::function<void()> drawPopUp = [&]() {
            SelectIPPopUpWindow();
        };
        m_EditorLayer->AddDrawCallback(drawPopUp);

        std::function<void()> imdraw = [&]() {


            ImGui::Begin("Settings");

            ImGui::Text("FPS : %f", ImGui::GetIO().Framerate);

            //text imput for message
            static char inputBuffer[256] = "Default message";
            ImGui::InputText("Enter Message", inputBuffer, IM_ARRAYSIZE(inputBuffer));

            if(isConnected){
                ImGui::Text("Connected to server");
                //create a button and sent a hi to server
                if (ImGui::Button("Send")) {
                    if(m_Client->IsRunning()){
                        m_Client->SendData(inputBuffer, strlen(inputBuffer));

                    } else {
                        ATLOG_INFO("Client is not running");
                        isConnected = false;
                    }
                }
                if (ImGui::Button("Echo")) {
                    if(m_Client->IsRunning()){
                        m_Client->SendData("echo", 4);

                    } else {
                        ATLOG_INFO("Client is not running");
                        isConnected = false;
                    }
                }
            }

            ImGui::Text("Settins test");
            ImGui::End();

        };
        m_EditorLayer->AddDrawCallback(imdraw);

        m_DrawMap = new DrawMap();
        PushLayer(m_DrawMap);



    }
    void Application::InitClient() {
        m_Client = new Client();

        m_Client->SetDataReceivedCallback([&](const void* data, unsigned int size) {
            ATLOG_WARN("[SERVER] : {0} ,size {1} bytes", (char*)data, size);
        });
        m_Client->SetServerConnectedCallback([&]() {
            ATLOG_INFO("Connected to server");
            m_Client->SendData("Hello from client", 17);
        });
        m_Client->SetServerDisconnectedCallback([&]() {
            ATLOG_INFO("Disconnected from server");
        });


    }


    void Application::SelectIPPopUpWindow() {
        if (!isConnected) {


            ImGuiIO &io = ImGui::GetIO();
            ImVec2 windowSize = ImVec2(400, 200);
            ImVec2 windowPos = ImVec2((io.DisplaySize.x - windowSize.x) * 0.5f,
                                      (io.DisplaySize.y - windowSize.y) * 0.5f);

            ImGui::SetNextWindowPos(windowPos);
            ImGui::SetNextWindowSize(windowSize);

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
                    static char inputBuffer[256] = "192.168.1.1";
                    ImGui::InputText("Enter IP", inputBuffer, IM_ARRAYSIZE(inputBuffer));
                    if (ImGui::Button("Connect")) {
                        isConnected = true;
                        std::string ip = inputBuffer;
                        ip.append(":27020");
                        m_Client->ConnectToServer(ip);

                    }
                } else if (m_IPIndex == DefaultIP) {
                    if (ImGui::Button("Connect with Default IP")) {
                        isConnected = true;
                        m_Client->ConnectToServer("192.168.1.8:27020");

                    }
                }
            }
            ImGui::EndPopup();
        }
    }



}