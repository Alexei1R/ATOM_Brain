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





        std::function<void()> joystick = [&]() {
            ImGui::Begin("Joystick Data");

            if (glfwJoystickPresent(joy)) {
                if (axesCount >= 4) { // Check if at least two axes are available
                    float xAxisLeft = axes[0]; // X position
                    float yAxisLeft = axes[1]; // Y position
                    float xAxisRight = axes[2]; // X position
                    float yAxisRight = axes[3]; // Y position
                    if(xAxisLeftMaxValue < xAxisLeft){
                        xAxisLeftMaxValue = xAxisLeft;
                    }
                    if(xAxisRightMaxValue < xAxisRight){
                        xAxisRightMaxValue = xAxisRight;
                    }

                    ImGui::Text("Left Joystick");
                    ImGui::Text("X: %f", xAxisLeft);
                    ImGui::Text("Y: %f", yAxisLeft);
                    ImGui::Text("Right Joystick");
                    ImGui::Text("X: %f", xAxisRight);
                    ImGui::Text("Y: %f", yAxisRight);

                    // Send yaxis left as angle and xaxis right as speed
                    //Send if value is different from last value
                    if(xAxisLeft != xAxisLeftLast){
                        xAxisLeftLast = xAxisLeft;
                        float angle = xAxisLeft * 25;
                        if (m_ClientLayer->IsRunning()) {
                            Message message;
                            message.id = 2;  // Set message ID
                            message.payloadSize = sizeof(angle);  // Set the size of the payload
                            message.payload = static_cast<void *>(&angle); // Set the payload
                            m_ClientLayer->SendMessage(message);
                        }
                    }



                    if(xAxisRight != xAxisRightLast){
                        xAxisRightLast = xAxisRight;
                        float speed = xAxisRight * 50;
                        if (m_ClientLayer->IsRunning()) {
                            Message message;
                            message.id = 1;  // Set message ID
                            message.payloadSize = sizeof(speed);  // Set the size of the payload
                            message.payload = static_cast<void *>(&speed); // Set the payload
                            m_ClientLayer->SendMessage(message);
                        }
                    }
                } else {
                    ImGui::Text("Insufficient axes data available");
                }
            } else {
                ImGui::Text("Joystick not detected");
            }

            ImGui::End();
        };
        m_EditorLayer->AddDrawCallback(joystick);

        std::function<void()> drawPopUp = [&]() {
            SelectIPPopUpWindow();
        };
        m_EditorLayer->AddDrawCallback(drawPopUp);

        std::function<void()> test = [&]() {
            // SelectIPPopUpWindow();
            ImGui::Begin("Test");
            ImGui::Text("Hello World");


            if(Keyboard::key(GLFW_KEY_SPACE)){
                steering = 0;
                Message message;
                message.id = 2;  // Set message ID
                message.payloadSize = sizeof(steering);  // Set the size of the payload
                message.payload = static_cast<void *>(&steering); // Set the payload
                m_ClientLayer->SendMessage(message);

                speed = 0;
                message.id = 1;  // Set message ID
                message.payloadSize = sizeof(speed);  // Set the size of the payload
                message.payload = static_cast<void *>(&speed); // Set the payload
                m_ClientLayer->SendMessage(message);

            }


            ImGui::SliderFloat("Slider", &sliderValue, -50.0, 50.0);
            if(lastsliderValue != sliderValue){
                lastsliderValue = sliderValue;
                    if (m_ClientLayer->IsRunning()) {
                        Message message;
                        message.id = 1;  // Set message ID
                        message.payloadSize = sizeof(sliderValue);  // Set the size of the payload
                        message.payload = static_cast<void *>(&sliderValue); // Set the payload
                        m_ClientLayer->SendMessage(message);
                    }
            }

            ImGui::SliderFloat("Angle", &angle, -25.0, 25.0);
            if(lastAngle != angle){
                lastAngle = angle;
                if (m_ClientLayer->IsRunning()) {
                    Message message;
                    message.id = 2;  // Set message ID
                    message.payloadSize = sizeof(angle);  // Set the size of the payload
                    message.payload = static_cast<void *>(&angle); // Set the payload
                    m_ClientLayer->SendMessage(message);
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

            axes = glfwGetJoystickAxes(joy, &axesCount);

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
                    static char inputBuffer[256] = "192.168.1.102";
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