//
// Created by toor on 3/9/24.
//

#include "CarState.h"


#include "CarState.h"
#include "ATOM/Application.h"
#include "opencv2/opencv.hpp"


namespace Atom {
    CarState::CarState(Gamepad *gamepad, DrawMap *drawMap, ClientLayer *clientLayer, DetectLines *detectLines)
        : Layer("CarState"), m_Gamepad(gamepad), m_DrawMap(drawMap), m_ClientLayer(clientLayer),
          m_DetectLines(detectLines) {
        m_Pid = new PID(PID_KP, PID_KI, PID_KD);


        m_Gamepad->SetChangeState([&](float value, JoystickAxis joyAxis) {
            // ATLOG_INFO("Value: {0}", value);
            if (m_StateMasine.state == CarStateEnum::IDLE) {
                // ATLOG_INFO("CarState::CarState: Gamepad state changed");
                if (joyAxis == JoystickAxis::LeftY) {
                    float speed = value * -m_MaxSpeed * 2;

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

    void CarState::OnAttach() {
    }

    void CarState::OnDetach() {
    }

    void CarState::OnUpdate() {
        //AUTONOMOUS
        if (m_StateMasine.state == CarStateEnum::AUTONOMOUS) {
            //DO AUTONOMOUS DRIVING
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

            timenow = std::chrono::high_resolution_clock::now();
            deltatime = std::chrono::duration_cast<std::chrono::milliseconds>(timenow - lastTimeState).count();
            if (deltatime > 500) {
                lastTimeState = std::chrono::high_resolution_clock::now();
                //Send Reset Command For Speed
                ComandCarSpeed(m_MaxSpeed);
            }

            //IF AUTONOMOUS DRIVING AND SIGN DETECTED

            for (auto &detectedSign: m_DrawMap->GetSignsDetected()) {
                if (detectedSign.name == "stop-sign") {
                    m_StopSignDistance = detectedSign.distance;
                    if (detectedSign.distance < m_MimSignDistance) {
                        // STOPPED
                        m_StateMasine.state = CarStateEnum::STOPPED;
                        lastTime = std::chrono::high_resolution_clock::now();
                        //RESET SPEED AND STEERING
                        ComandCarSpeed(0);
                        ComandCarSteering(0);
                    }
                }
            }
        }


        //STOPPED
        if (m_StateMasine.state == CarStateEnum::STOPPED) {
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
            if (duration > 200) {
                lastTime = std::chrono::high_resolution_clock::now();
                ComandCarSpeed(0);
                ComandCarSteering(0);
            }


            //WAIT FOR DIRECTION
            //Gamepad
            if (m_Gamepad->GetJoystickState()->PadUp) {
                m_StateMasine.state = CarStateEnum::MOVE_FORWARD;
                lastTime = std::chrono::high_resolution_clock::now();
                ComandCarSpeed(m_MaxSpeed);
            }
            if (m_Gamepad->GetJoystickState()->PadLeft) {
                m_StateMasine.state = CarStateEnum::TURN_LEFT;
                lastTime = std::chrono::high_resolution_clock::now();
                ComandCarSpeed(m_MaxSpeed);
            }
            if (m_Gamepad->GetJoystickState()->PadRight ) {
                m_StateMasine.state = CarStateEnum::TURN_RIGHT;
                lastTime = std::chrono::high_resolution_clock::now();
                ComandCarSpeed(m_MaxSpeed);
            }
        }


        //TURN_LEFT
        if (m_StateMasine.state == CarStateEnum::TURN_LEFT) {
            //FOR 3 SECONDS TURN LEFT
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
            if (duration > 200) {
                if (m_IterationCount < m_TimeToTurnLeft) {
                    ComandCarSteering(-m_MaxSteering / 2);
                    // ComandCarSpeed(m_MaxSpeed);
                    m_IterationCount++;
                    lastTime = std::chrono::high_resolution_clock::now();
                } else {
                    m_StateMasine.state = CarStateEnum::AUTONOMOUS;
                    m_IterationCount = 0;
                }
            }
        }

        //TURN_RIGHT
        if (m_StateMasine.state == CarStateEnum::TURN_RIGHT) {
            //FOR 3 SECONDS TURN RIGHT
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
            if (duration > 200) {
                if (m_IterationCount < m_TimeToTurnRight) {
                    ComandCarSteering(m_MaxSteering);
                    // ComandCarSpeed(m_MaxSpeed);
                    m_IterationCount++;
                    lastTime = std::chrono::high_resolution_clock::now();
                } else {
                    m_StateMasine.state = CarStateEnum::AUTONOMOUS;
                    m_IterationCount = 0;
                }
            }
        }

        //MOVE_FORWARD
        if (m_StateMasine.state == CarStateEnum::MOVE_FORWARD) {
            //FOR 3 SECONDS MOVE FORWARD
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
            if (duration > 200) {
                if (m_IterationCount < m_TimeToMoveForward) {
                    ComandCarSteering(0);
                    m_IterationCount++;
                    lastTime = std::chrono::high_resolution_clock::now();
                } else {
                    m_StateMasine.state = CarStateEnum::AUTONOMOUS;
                    m_IterationCount = 0;
                }
            }
        }




    }

    void CarState::OnFixedUpdate() {
        if (m_Gamepad->GetJoystickState()->LeftShoulder) {
            m_StateMasine.state = CarStateEnum::IDLE;
        }
        if (m_Gamepad->GetJoystickState()->RightShoulder || m_Gamepad->GetJoystickState()->LeftTrigger > 0.2) {
            m_StateMasine.state = CarStateEnum::AUTONOMOUS;
        }
        if (m_Gamepad->GetJoystickState()->ButtonX || m_Gamepad->GetJoystickState()->RightTrigger > 0.5 || m_Gamepad->GetJoystickState()->PadDown) {
            m_StateMasine.state = CarStateEnum::STOPPED;
        }


    }

    void CarState::OnImGuiRender() {
        ImGui::Begin("Car State");
        ImGui::Separator();
        if (ImGui::Checkbox("Autonoumous Driving", &m_CheckBoxState)) {
            if (m_CheckBoxState) {
                m_StateMasine.state = CarStateEnum::AUTONOMOUS;
                ComandCarSpeed(m_MaxSpeed);
            } else {
                m_StateMasine.state = CarStateEnum::IDLE;
            }
        }
        ImGui::Separator();
        ImGui::SliderInt("Min Sign Distance", &m_MimSignDistance, 0, 1000);
        ImGui::Separator();

        ImGui::Text("STOP Sign Distance %d", m_StopSignDistance);
        ImGui::Separator();



        //print state
        if (m_StateMasine.state == CarStateEnum::IDLE) {
            ImGui::Text("State: IDLE");
            m_CheckBoxState = false;
        }
        if (m_StateMasine.state == CarStateEnum::AUTONOMOUS) {
            m_CheckBoxState = true;
            ImGui::Text("State: AUTONOMOUS");
        }
        if (m_StateMasine.state == CarStateEnum::STOPPED) {
            ImGui::Text("State: STOPPED");
        }
        if (m_StateMasine.state == CarStateEnum::TURN_LEFT) {
            ImGui::Text("State: TURN_LEFT");
        }
        if (m_StateMasine.state == CarStateEnum::TURN_RIGHT) {
            ImGui::Text("State: TURN_RIGHT");
        }
        if (m_StateMasine.state == CarStateEnum::MOVE_FORWARD) {
            ImGui::Text("State: MOVE_FORWARD");
        }
        ImGui::Separator();

        ImGui::SliderInt("Time To Turn Left", &m_TimeToTurnLeft, 0, 100);
        ImGui::SliderInt("Time To Turn Right", &m_TimeToTurnRight, 0, 100);
        ImGui::SliderInt("Time To Move Forward", &m_TimeToMoveForward, 0, 100);

        ImGui::SliderFloat("Max Speed", &m_MaxSpeed, 0.0f, 100.0f);
        ImGui::SliderFloat("Max Steering", &m_MaxSteering, 0.0f, 100.0f);
        ImGui::Separator();


        ImGui::Text("PID Out: %.2f", m_PidOut);

        ImGui::SliderFloat("PID_KP", &PID_KP, 0.0f, 10.0f);
        ImGui::SliderFloat("PID_KI", &PID_KI, 0.0f, 10.0f);
        ImGui::SliderFloat("PID_KD", &PID_KD, 0.0f, 10.0f);

        if (ImGui::Button("Set PID")) {
            m_PidChanged = true;
            m_Pid->SetP(PID_KP);
            m_Pid->SetI(PID_KI);
            m_Pid->SetD(PID_KD);
            m_PidOut = 0;

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
