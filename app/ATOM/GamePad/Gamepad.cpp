//
// Created by toor on 2/4/24.
//
// Gamepad.cpp
#include "Gamepad.h"
#include "ATOM/Application.h"

namespace Atom {
    static Gamepad *g_GamepadInstance = nullptr;

    void Gamepad::joystick_callback(int jid, int event) {
        if (g_GamepadInstance) {
            // Use the instance to call the non-static member function
            g_GamepadInstance->joystick_callback_non_static(jid, event);
        }
    }

    void Gamepad::joystick_callback_non_static(int jid, int event) {
        if (event == GLFW_CONNECTED) {
            m_Joystick.Index = jid;
            m_Joystick.Name = glfwGetJoystickName(jid);
            m_Joystick.IsConnected = true;
            ATLOG_WARN("Joystick connected: {0}", jid);
            ATLOG_WARN("Joystick Name: {0}", m_Joystick.Name);
        } else if (event == GLFW_DISCONNECTED) {
            m_Joystick.IsConnected = false;
            m_Joystick.Name = "";
            m_Joystick.Index = -1;
            ATLOG_WARN("Joystick disconnected: {0}", jid);
        }
    }

    Gamepad::Gamepad()
        : Layer("Gamepad") {
        // Set the instance to this object
        g_GamepadInstance = this;

        // check in a for if a joystick is connected
        for (int jid = GLFW_JOYSTICK_1; jid <= GLFW_JOYSTICK_LAST; jid++) {
            if (glfwJoystickPresent(jid)) {
                m_Joystick.Index = jid;
                m_Joystick.Name = glfwGetJoystickName(jid);
                m_Joystick.IsConnected = true;
                ATLOG_WARN("Joystick connected: {0}", jid);
                ATLOG_WARN("Joystick Name: {0}", m_Joystick.Name);
                return;
            }
        }
        // bind the joystick callback
        glfwSetJoystickCallback(joystick_callback);
    }

    Gamepad::~Gamepad() {
        // Unset the instance when the object is destroyed
        g_GamepadInstance = nullptr;
    }


    void Gamepad::OnAttach() {
    }

    void Gamepad::OnDetach() {
    }

    void Gamepad::OnUpdate() {
        if (m_Joystick.IsConnected) {
            // const float* glfwGetJoystickAxes(int jid, int* count)
            glfwGetJoystickAxes(m_Joystick.Index, &m_Joystick.count);
            const float *axes = glfwGetJoystickAxes(m_Joystick.Index, &m_Joystick.count);
            if (m_Joystick.count >= 6) {
                m_Joystick.LeftStick.x = axes[0];
                m_Joystick.LeftStick.y = axes[1];
                m_Joystick.RightStick.x = axes[2];
                m_Joystick.RightStick.y = axes[3];
                m_Joystick.RightTrigger = axes[4];
                m_Joystick.LeftTrigger = axes[5];
            } else {
                ATLOG_CRITICAL("Joystick count is less than 6");
            }
            glfwGetJoystickButtons(m_Joystick.Index, &m_Joystick.count);
            const unsigned char *buttons = glfwGetJoystickButtons(m_Joystick.Index, &m_Joystick.count);
            if (m_Joystick.count >= 14) {
                m_Joystick.ButtonA = buttons[0];
                m_Joystick.ButtonB = buttons[1];
                m_Joystick.ButtonX = buttons[2];
                m_Joystick.ButtonY = buttons[3];
                m_Joystick.LeftShoulder = buttons[4];
                m_Joystick.RightShoulder = buttons[5];
                m_Joystick.Start = buttons[6];
                m_Joystick.Select = buttons[7];

                m_Joystick.LeftStickButton = buttons[9];
                m_Joystick.RightStickButton = buttons[10];
                m_Joystick.PadUp = buttons[11];
                m_Joystick.PadRight = buttons[12];
                m_Joystick.PadDown = buttons[13];
                m_Joystick.PadLeft = buttons[14];
            } else {
                ATLOG_CRITICAL("Joystick count is less than 14");
            }
        }
    }

    void Gamepad::OnImGuiRender() {
        ImGui::Begin("Joystick Layer");

        //min size of the window
        ImGui::SetWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

        ImGui::Text("Gamepad Layer");
        //print all the joystick data
        ImGui::Text("Name: %s", m_Joystick.Name.c_str());
        ImGui::Text("IsConnected: %s", m_Joystick.IsConnected ? "true" : "false");
        ImGui::Text("Index: %d", m_Joystick.Index);
        ImGui::Text("LeftStick: (%f,%f)", m_Joystick.LeftStick.x, m_Joystick.LeftStick.y);
        ImGui::Text("LeftStickButton: %s", m_Joystick.LeftStickButton ? "true" : "false");
        ImGui::Text("RightStick: (%f,%f)", m_Joystick.RightStick.x, m_Joystick.RightStick.y);
        ImGui::Text("RightStickButton: %s", m_Joystick.RightStickButton ? "true" : "false");
        ImGui::Text("PadUp: %s", m_Joystick.PadUp ? "true" : "false");
        ImGui::Text("PadDown: %s", m_Joystick.PadDown ? "true" : "false");
        ImGui::Text("PadLeft: %s", m_Joystick.PadLeft ? "true" : "false");
        ImGui::Text("PadRight: %s", m_Joystick.PadRight ? "true" : "false");
        ImGui::Text("LeftTrigger: %f", m_Joystick.LeftTrigger);
        ImGui::Text("RightTrigger: %f", m_Joystick.RightTrigger);
        ImGui::Text("LeftShoulder: %s", m_Joystick.LeftShoulder ? "true" : "false");
        ImGui::Text("RightShoulder: %s", m_Joystick.RightShoulder ? "true" : "false");
        ImGui::Text("Start: %s", m_Joystick.Start ? "true" : "false");
        ImGui::Text("Select: %s", m_Joystick.Select ? "true" : "false");
        ImGui::Text("ButtonA: %s", m_Joystick.ButtonA ? "true" : "false");
        ImGui::Text("ButtonB: %s", m_Joystick.ButtonB ? "true" : "false");
        ImGui::Text("ButtonX: %s", m_Joystick.ButtonX ? "true" : "false");
        ImGui::Text("ButtonY: %s", m_Joystick.ButtonY ? "true" : "false");

       


        ImGui::End();
    }
}
