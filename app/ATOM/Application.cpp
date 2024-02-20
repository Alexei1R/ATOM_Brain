//
// Created by toor on 11/7/23.
//
#include "atompch.h"
#include "Application.h"
#include "imgui.h"


namespace Atom {
    char inputBuffer[256] = "/home/toor/Downloads/pc.mp4";
    static const char *comboItems[] = {
        "nvarguscamerasrc sensor_id=0 ! video/x-raw(memory:NVMM),width=640, height=480, framerate=30/1 ! nvvidconv flip-method=0 ! video/x-raw,width=640, height=480 ! nvvidconv ! video/x-raw,format=BGRx ! videoconvert ! appsink",
        "v4l2src device=/dev/video0 ! video/x-raw,format=YUY2,width=640,height=480,framerate=30/1 ! videoconvert ! appsink",
        "1",
    };

    Application *Application::s_Instance = nullptr;


    Application::Application() {
        s_Instance = (Application *) this;

        m_Window = Window::Create();
        m_Window->SetVSync(m_VSync);
        m_Window->SetWindowCloseCallback([this] { WindowClose(); });


        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
        m_EditorLayer = new EditorLayer();
        PushOverlay(m_EditorLayer);
        m_ClientLayer = new ClientLayer();
        PushLayer(m_ClientLayer);
        m_DrawMap = new DrawMap();
        PushLayer(m_DrawMap);
        m_Frame = new Frame();
        PushLayer(m_Frame);
        m_Gamepad = new Gamepad();
        PushLayer(m_Gamepad);
        m_DetectLines = new DetectLines();
        PushLayer(m_DetectLines);


        m_ClientLayer->RegisterMessageWithID(2, [&](Message message) {
            ATLOG_INFO("Message Received: ID = 2 {0}", *(int *) message.payload);
        });

        m_ClientLayer->RegisterMessageWithID(50, [&](Message message) {
            std::string data = static_cast<char *>(message.payload);
            if (data == "OK") {
                m_Frame->OpenVideoCapture();
            } else {
                ATLOG_CRITICAL("Error to open camera")
            }
        });


        m_Gamepad->SetChangeState([&](float value, JoystickAxis joyAxis) {
            if (joyAxis == JoystickAxis::LeftY) {
                float speed = value * -50;
                if (m_ClientLayer->IsRunning()) {
                    Message message;
                    message.id = 1; // Set message ID
                    message.payloadSize = sizeof(speed); // Set the size of the payload
                    message.payload = static_cast<void *>(&speed); // Set the payload
                    m_ClientLayer->SendMessage(message);
                }
            }
            if (joyAxis == JoystickAxis::RightX) {
                float angle = value * 25;
                if (m_ClientLayer->IsRunning()) {
                    Message message;
                    message.id = 2; // Set message ID
                    message.payloadSize = sizeof(angle); // Set the size of the payload
                    message.payload = static_cast<void *>(&angle); // Set the payload
                    m_ClientLayer->SendMessage(message);
                }
            }
        });


        std::function<void()> drawPopUp = [&]() {
            SelectIPPopUpWindow();
        };
        m_EditorLayer->AddDrawCallback(drawPopUp);

        std::function<void()> draw = [&]() {
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


            if (ImGui::CollapsingHeader("UI Settings")) {
                DrawUISetings();
            }
            if (ImGui::CollapsingHeader("Camera Settings")) {
                DrawCameraSettings();
            } else {
                if (m_Gamepad->GetJoystickState()->ButtonA && m_Gamepad->GetJoystickState()->ButtonY &&
                    isOpenCameraComandSent == false) {
                    if (m_ClientLayer->IsRunning()) {
                        ATLOG_WARN("Open Camera using Gamepad");
                        isOpenCameraComandSent = true;
                        std::string data = comboItems[0];
                        Message message;
                        message.id = 50;
                        message.payloadSize = data.size();
                        message.payload = static_cast<void *>(const_cast<char *>(data.c_str()));
                        m_ClientLayer->SendMessage(message);
                    }
                }
            }
            if (ImGui::CollapsingHeader("Map Settings")) {
                DrawMapSettings();
            }


            ImGui::End();
        };
        m_EditorLayer->AddDrawCallback(draw);
    }


    Application::~Application() {
        delete m_Window;
        delete m_ImGuiLayer;
        delete m_EditorLayer;
        m_ClientLayer->Shutdown();
        delete m_ClientLayer;
        m_Frame->Shutdown();
        delete m_Frame;
    }


    void Application::PushLayer(Layer *layer) {
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer *layer) {
        m_LayerStack.PushOverlay(layer);
    }


    void Application::Run() {
        ATLOG_WARN("Begin Runing");
        while (m_IsRuning) {
            for (Layer *layer: m_LayerStack) {
                layer->OnUpdate();
            }
            auto currentTime = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> timeSpan = currentTime - lastTime;
            if (timeSpan.count() > 33) {
                lastTime = std::chrono::high_resolution_clock::now();
                for (Layer *layer: m_LayerStack) {
                    layer->OnFixedUpdate();
                }
            }

            m_Window->ClearDisplay(glm::vec3(0, 255, 255));

            m_ImGuiLayer->Begin();
            for (Layer *layer: m_LayerStack) {
                layer->OnImGuiRender();
            }
            m_ImGuiLayer->End();
            m_Window->OnUpdate();
        }
    }

    void Application::WindowClose() {
        m_IsRuning = false;
        // m_Frame->Shutdown();
        // m_ClientLayer->Shutdown();
    }


    void Application::SelectIPPopUpWindow() {
        if (!isConnected) {
            //get curent imgui window position and size
            ImVec2 mainWindowPos = ImGui::GetWindowPos();
            ImVec2 mainWindowSize = ImGui::GetWindowSize();
            mainWindowSize.x -= mainWindowSize.x * 0.2f;
            ImVec2 popupSize = ImVec2(320, 240);
            ImVec2 popupPos = ImVec2(mainWindowPos.x + (mainWindowSize.x - popupSize.x) * 0.5f,
                                     mainWindowPos.y + (mainWindowSize.y - popupSize.y) * 0.5f);

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
                    // static char inputBuffer[256] = "192.168.36.60";
                    // static char inputBuffer[256] = "192.168.1.16";
                    // static char inputBuffer[256] = "192.168.1.8";
                    // static char inputBuffer[256] = "192.168.156.32";
                    static char inputBuffer[256] = "192.168.1.102";
                    ImGui::InputText("Enter IP", inputBuffer, IM_ARRAYSIZE(inputBuffer));
                    //button or enter key is pressed
                    if (ImGui::Button("Connect") || ImGui::IsKeyPressed(ImGuiKey_Enter, false) || m_Gamepad->
                        GetJoystickState()->ButtonA) {
                        isConnected = true;
                        std::string ip = inputBuffer;
                        ip.append(":27020");
                        m_ClientLayer->ConnectToServer(ip);
                    }
                } else if (m_IPIndex == DefaultIP) {
                    //button or enter key is pressed
                    if (ImGui::Button("Connect with Default IP")) {
                        isConnected = true;
                        m_ClientLayer->ConnectToServer("192.168.100.119:27020");
                    }
                }
            }
            ImGui::EndPopup();
        }
    }

    void Application::DrawUISetings() {
        ImGui::Spacing();
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Spacing();
        ImGui::Separator();
        //set vsync
        ImGui::Checkbox("VSync", &m_VSync);
        if (m_VSync != m_Window->IsVSync()) {
            m_Window->SetVSync(m_VSync);
        }
        ImGui::Separator();
    }

    void Application::DrawCameraSettings() {
        ImGui::Text("Open Camera");

        ImGui::BeginTabBar("##TabBar", ImGuiTabBarFlags_None);
        if (ImGui::BeginTabItem(" Custom ")) {
            ImGui::Text("Enter Pipeline");
            ImGui::PushItemWidth(-1);
            ImGui::InputText("##InputText", inputBuffer, IM_ARRAYSIZE(inputBuffer));
            ImGui::PopItemWidth();
            ImGui::EndTabItem();
            if (ImGui::Button("Open Camera") && isOpenCameraComandSent == false) {
                if (m_ClientLayer->IsRunning()) {
                    isOpenCameraComandSent = true;
                    std::string data = inputBuffer;
                    Message message;
                    message.id = 50;
                    message.payloadSize = data.size();
                    message.payload = static_cast<void *>(const_cast<char *>(data.c_str()));
                    m_ClientLayer->SendMessage(message);
                }
            }
        }
        if (ImGui::BeginTabItem(" Default ")) {
            static int m_ComboIndex = 0;

            ImGui::Combo("##Combo", &m_ComboIndex, comboItems, IM_ARRAYSIZE(comboItems));
            if (ImGui::Button("Open Camera") && isOpenCameraComandSent == false) {
                if (m_ClientLayer->IsRunning()) {
                    isOpenCameraComandSent = true;
                    std::string data = comboItems[m_ComboIndex];
                    Message message;
                    message.id = 50;
                    message.payloadSize = data.size();
                    message.payload = static_cast<void *>(const_cast<char *>(data.c_str()));
                    m_ClientLayer->SendMessage(message);
                }
            }

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();


        ImGui::Separator();
    }

    void Application::DrawMapSettings() {
        MapSetings *mapSetings = m_DrawMap->GetMapSetings();
        int bg = static_cast<int>(mapSetings->background);

        static int m_ComboIndex = 1;
        static const char *comboItems[] = {
            "None",
            "Track",
            "ColorTrack",
            "Intersection",
            "MainRoad",
            "SideRoad",
            "Parking",
            "PedestrianCrossing",
        };
        ImGui::Combo("##Combo", &m_ComboIndex, comboItems, IM_ARRAYSIZE(comboItems));
        if (bg != m_ComboIndex) {
            mapSetings->background = static_cast<MapBackground>(m_ComboIndex);
            mapSetings->isChanged = true;
        }
        ImGui::Checkbox("Show Points", &mapSetings->showPoints);
        ImGui::Separator();

        ImGui::SliderInt("Erode Value", m_DrawMap->GetErodeValue(), 3, 35);
    }

    void Application::ConntrollCarUithGamepad() {
    }
}
