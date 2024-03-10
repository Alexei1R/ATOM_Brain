// Created by toor on 3/9/24.

#include "CarState.h"

namespace Atom {
    CarState::CarState(Gamepad *gamepad, DrawMap *drawMap, ClientLayer *clientLayer, DetectLines *detectLines)
        : Layer("CarState"), m_Gamepad(gamepad), m_DrawMap(drawMap), m_ClientLayer(clientLayer),
          m_DetectLines(detectLines) {
        m_Pid = new PID(PID_KP, PID_KI, PID_KD);

        m_Gamepad->SetChangeState([&](float value, JoystickAxis joyAxis) {
            if (m_StateMasine.state == CarStateEnum::IDLE) {
                ATLOG_INFO("CarState::CarState: Gamepad state changed");
                if (joyAxis == JoystickAxis::LeftY) {
                    float speed = value * m_MaxSpeed;
                    ComandCarSpeed(speed);
                }
                if (joyAxis == JoystickAxis::RightX) {
                    float angle = value * m_MaxSteering;
                    angle = std::clamp(angle, -m_MaxSteering, m_MaxSteering);
                    angle += m_OffsetSteering;
                    ComandCarSteering(angle);
                }
            }
        });
    }

    CarState::~CarState() {
        delete m_Pid;
    }

    void CarState::OnAttach() {}

    void CarState::OnDetach() {}

    void CarState::OnUpdate() {
        timenow = std::chrono::high_resolution_clock::now();
        deltatime = std::chrono::duration_cast<std::chrono::milliseconds>(timenow - lastTimeState).count();
        if (deltatime > 100) {
            // AUTONOMOUS
            if (m_StateMasine.state == CarStateEnum::AUTONOMOUS) {
                ComandCarSpeed(m_MaxSpeed);
                // DO AUTONOMOUS DRIVING
                auto currentTimePid = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> timeSpanPid = currentTimePid - lastTimePid;
                if (timeSpanPid.count() > 100) {
                    lastTimePid = std::chrono::high_resolution_clock::now();
                    float deltaTime = timeSpanPid.count() / 1000.0f;
                    m_PidOut = m_Pid->calculate(0, m_DetectLines->GetOffsetCenter() / 3, deltaTime, m_MaxSteering,
                                                -m_MaxSteering);
                    m_PidOut = std::clamp(m_PidOut, -m_MaxSteering, m_MaxSteering);
                    ComandCarSteering(m_PidOut);
                }

                // IF AUTONOMOUS DRIVING AND SIGN DETECTED
                for (auto &detectedSign: m_DrawMap->GetSignsDetected()) {
                    if (detectedSign.name == "stop-sign" && detectedSign.distance < m_MimSignDistance) {
                        // STOPPED
                        m_StateMasine.state = CarStateEnum::STOPPED;
                    }
                }
            }
            // STOPPED
            else if (m_StateMasine.state == CarStateEnum::STOPPED) {
                ComandCarSpeed(0);
                ComandCarSteering(0);

                // WAIT FOR DIRECTION
                if (m_Gamepad->GetJoystickState()->PadUp) {
                    m_StateMasine.state = CarStateEnum::MOVE_FORWARD;
                }
                else if (m_Gamepad->GetJoystickState()->PadLeft) {
                    m_StateMasine.state = CarStateEnum::TURN_LEFT;
                }
                else if (m_Gamepad->GetJoystickState()->PadRight) {
                    m_StateMasine.state = CarStateEnum::TURN_RIGHT;
                }
            }
            // TURN_LEFT
            else if (m_StateMasine.state == CarStateEnum::TURN_LEFT) {
                ComandCarSpeed(m_MaxSpeed);
                ComandCarSteering(-m_MaxSteering);
                auto currentTime = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
                if (duration > 3000) {
                    m_StateMasine.state = CarStateEnum::AUTONOMOUS;
                    ComandCarSpeed(m_MaxSpeed);
                    ComandCarSteering(0);
                    lastTime = std::chrono::high_resolution_clock::now();
                }
            }
            // TURN_RIGHT
            else if (m_StateMasine.state == CarStateEnum::TURN_RIGHT) {
                ComandCarSpeed(m_MaxSpeed);
                ComandCarSteering(m_MaxSteering);
                auto currentTime = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
                if (duration > 2000) {
                    m_StateMasine.state = CarStateEnum::AUTONOMOUS;
                    ComandCarSpeed(m_MaxSpeed);
                    ComandCarSteering(0);
                    lastTime = std::chrono::high_resolution_clock::now();
                }
            }
            // MOVE_FORWARD
            else if (m_StateMasine.state == CarStateEnum::MOVE_FORWARD) {
                ComandCarSpeed(m_MaxSpeed);
                ComandCarSteering(0);
                auto currentTime = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
                if (duration > 2000) {
                    m_StateMasine.state = CarStateEnum::AUTONOMOUS;
                    ComandCarSpeed(m_MaxSpeed);
                    ComandCarSteering(0);
                    lastTime = std::chrono::high_resolution_clock::now();
                }
            }
        }
    }

    void CarState::OnFixedUpdate() {}

    void CarState::OnImGuiRender() {
        ImGui::Begin("Car State");
        ImGui::Separator();
        if (ImGui::Checkbox("Autonoumous Driving", &m_CheckBoxState)) {
            m_StateMasine.state = (m_CheckBoxState) ? CarStateEnum::AUTONOMOUS : CarStateEnum::IDLE;
            ComandCarSpeed(m_MaxSpeed);
        }
        ImGui::Separator();
        ImGui::SliderInt("Min Sign Distance", &m_MimSignDistance, 0, 1000);
        ImGui::Separator();

        // Print state
        switch (m_StateMasine.state) {
            case CarStateEnum::IDLE:
                ImGui::Text("State: IDLE");
                break;
            case CarStateEnum::AUTONOMOUS:
                ImGui::Text("State: AUTONOMOUS");
                break;
            case CarStateEnum::STOPPED:
                ImGui::Text("State: STOPPED");
                break;
            case CarStateEnum::TURN_LEFT:
                ImGui::Text("State: TURN_LEFT");
                break;
            case CarStateEnum::TURN_RIGHT:
                ImGui::Text("State: TURN_RIGHT");
                break;
            case CarStateEnum::MOVE_FORWARD:
                ImGui::Text("State: MOVE_FORWARD");
                break;
        }

        ImGui::Separator();
        ImGui::SliderFloat("Max Speed", &m_MaxSpeed, 0.0f, 100.0f);
        ImGui::SliderFloat("Max Steering", &m_MaxSteering, 0.0f, 100.0f);
        ImGui::Separator();
        ImGui::Text("PID Out: %.2f", m_PidOut);
        ImGui::SliderFloat("PID_KP", &PID_KP, 0.0f, 10.0f);
        ImGui::SliderFloat("PID_KI", &PID_KI, 0.0f, 10.0f);
        ImGui::SliderFloat("PID_KD", &PID_KD, 0.0f, 10.0f);

        if (ImGui::Button("Set PID")) {
            m_PidChanged = true;
        }

        ImGui::End();
    }

    void CarState::ComandCarSpeed(float speed) {
        if (m_ClientLayer->IsRunning()) {
            Message message;
            message.id = 1; // Set message ID
            message.payloadSize = sizeof(speed); // Set the size of the payload
            message.payload = static_cast<void *>(&speed); // Set the payload
            m_ClientLayer->SendMessage(message);
        }
    }

    void CarState::ComandCarSteering(float angle) {
        if (m_ClientLayer->IsRunning()) {
            Message message;
            message.id = 2; // Set message ID
            message.payloadSize = sizeof(angle); // Set the size of the payload
            message.payload = static_cast<void *>(&angle); // Set the payload
            m_ClientLayer->SendMessage(message);
        }
    }
}
