//
// Created by toor on 2/4/24.
//
// Gamepad.cpp
#include "Gamepad.h"
#include "ATOM/Application.h"

namespace Atom {
    static Gamepad *g_GamepadInstance = nullptr;
    Joystick Gamepad::s_Joystick = {};

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
            // reset all joystick values
            m_Joystick.LeftStick = glm::vec2(0);
            m_Joystick.RightStick = glm::vec2(0);
            m_Joystick.LeftTrigger = 0;
            m_Joystick.RightTrigger = 0;
            m_Joystick.ButtonA = false;
            m_Joystick.ButtonB = false;
            m_Joystick.ButtonX = false;
            m_Joystick.ButtonY = false;
            m_Joystick.LeftShoulder = false;
            m_Joystick.RightShoulder = false;
            m_Joystick.Start = false;
            m_Joystick.Select = false;
            m_Joystick.LeftStickButton = false;
            m_Joystick.RightStickButton = false;
            m_Joystick.PadUp = false;
            m_Joystick.PadRight = false;
            m_Joystick.PadDown = false;
            m_Joystick.PadLeft = false;
            m_Joystick.CenterButton = false;
            m_Joystick.count = 0;

        }
    }

    Gamepad::Gamepad()
        : Layer("Gamepad") {
        // Set the instance to this object
        g_GamepadInstance = this;

        m_JoystickImage = stbi_load("ASSETS/joystick.png", &m_JoystickImageWidth, &m_JoystickImageHeight,
                                    &m_JoystickImageChannels, 0);
        m_AspectRatio = (float) m_JoystickImageWidth / (float) m_JoystickImageHeight;

        glGenTextures(1, &m_JoystickTexture);
        glBindTexture(GL_TEXTURE_2D, m_JoystickTexture);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load and generate the texture
        if (m_JoystickImage) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_JoystickImageWidth, m_JoystickImageHeight, 0, GL_RGBA,
                         GL_UNSIGNED_BYTE, m_JoystickImage);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            ATLOG_ERROR("Failed to load texture");
        }
        stbi_image_free(m_JoystickImage);


        // check in a for if a joystick is connected
        for (int jid = GLFW_JOYSTICK_1; jid <= GLFW_JOYSTICK_LAST; jid++) {
            if (glfwJoystickPresent(jid) == GLFW_TRUE) {
                m_Joystick.Index = jid;
                m_Joystick.Name = glfwGetJoystickName(jid);
                m_Joystick.IsConnected = true;
                ATLOG_WARN("Joystick connected: {0}", jid);
                ATLOG_WARN("Joystick Name: {0}", m_Joystick.Name);
                break;
            }
        }
        // bind the joystick callback
        glfwSetJoystickCallback(joystick_callback);

        m_FirstCanvasHeight = 525.7025;
        m_FirstCanvasWidth = 834;
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
        if (m_Joystick.IsConnected && glfwJoystickPresent(m_Joystick.Index) == GLFW_TRUE) {
            glfwGetJoystickAxes(m_Joystick.Index, &m_Joystick.count);
            const float *axes = glfwGetJoystickAxes(m_Joystick.Index, &m_Joystick.count);
            if (m_Joystick.count >= 6) {
                m_Joystick.LeftStick.x = axes[0];
                m_Joystick.LeftStick.y = axes[1];
                m_Joystick.RightStick.x = axes[2];
                m_Joystick.RightStick.y = axes[3];
                m_Joystick.RightTrigger = axes[4];
                m_Joystick.LeftTrigger = axes[5];
            }
            glfwGetJoystickButtons(m_Joystick.Index, &m_Joystick.count);
            const unsigned char *buttons = glfwGetJoystickButtons(m_Joystick.Index, &m_Joystick.count);
            //print button count
            if (m_Joystick.count >= 14) {
                m_Joystick.ButtonA = buttons[0];
                m_Joystick.ButtonB = buttons[1];
                m_Joystick.ButtonX = buttons[3];
                m_Joystick.ButtonY = buttons[4];
                m_Joystick.LeftShoulder = buttons[6];
                m_Joystick.RightShoulder = buttons[7];
                m_Joystick.Start = buttons[10];
                m_Joystick.Select = buttons[11];
                m_Joystick.LeftStickButton = buttons[13];
                m_Joystick.RightStickButton = buttons[14];
                m_Joystick.PadUp = buttons[15];
                m_Joystick.PadRight = buttons[16];
                m_Joystick.PadDown = buttons[17];
                m_Joystick.PadLeft = buttons[18];
            }
            s_Joystick = m_Joystick;
        }
    }

    void Gamepad::OnImGuiRender() {
        ImGui::Begin("Joystick");
        ImGui::SetWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);


        ImVec2 canvas_left_top = ImGui::GetCursorScreenPos();
        ImVec2 canvas_size = ImGui::GetContentRegionAvail();
        ImVec2 canvas_bottom_right = ImVec2(canvas_left_top.x + canvas_size.x, canvas_left_top.y + canvas_size.y);
        ImVec2 available = ImGui::GetContentRegionAvail();
        if (available.x > available.y * m_AspectRatio) {
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (available.x / 2 - available.y * m_AspectRatio / 2));
            canvas_left_top.x = ImGui::GetCursorScreenPos().x;
            canvas_bottom_right.x = canvas_left_top.x + available.y * m_AspectRatio;
            available.x = available.y * m_AspectRatio;
        } else {
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (available.y / 2 - available.x / m_AspectRatio / 2));
            canvas_left_top.y = ImGui::GetCursorScreenPos().y;
            canvas_bottom_right.y = canvas_left_top.y + available.x / m_AspectRatio;
            available.y = available.x / m_AspectRatio;
        }


        m_HeightMultiplier = available.y / m_FirstCanvasHeight;
        m_WidthMultiplier = available.x / m_FirstCanvasWidth;
        m_ScaleFactor = (m_HeightMultiplier + m_WidthMultiplier) / 2;


        ImGuiIO &io = ImGui::GetIO();
        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        draw_list->AddRectFilled(canvas_left_top, canvas_bottom_right, IM_COL32(50, 50, 50, 255));


        //draw image to background
        draw_list->AddImage((void *) m_JoystickTexture, canvas_left_top, canvas_bottom_right);


        //if curent window is bigger than 400x400
        if(available.x > 400 && available.y > 400){
            sprintf(buffer, "%s", m_Joystick.Name.c_str());
            draw_list->AddText(ImVec2(canvas_left_top.x + 10, canvas_left_top.y + 10), IM_COL32(255, 255, 255, 255),
                               buffer);
        }

        // draw left joystick center
        // draw right joystick center
        draw_list->AddCircleFilled(
            ImVec2(
                canvas_left_top.x + m_JoystickPosition.LeftStickPosition.x * m_WidthMultiplier + m_JoystickPosition.
                MaxJoystickRadius * m_Joystick.LeftStick.x * m_ScaleFactor,
                canvas_left_top.y + m_JoystickPosition.LeftStickPosition.y * m_HeightMultiplier + m_JoystickPosition.
                MaxJoystickRadius * m_Joystick.LeftStick.y * m_ScaleFactor),
            m_JoystickPosition.JoystickRadius * m_ScaleFactor,
            IM_COL32(100, 255, 255, 255), 25);
        draw_list->AddCircleFilled(
            ImVec2(
                canvas_left_top.x + m_JoystickPosition.RightStickPosition.x * m_WidthMultiplier + m_JoystickPosition.
                MaxJoystickRadius * m_Joystick.RightStick.x * m_ScaleFactor,
                canvas_left_top.y + m_JoystickPosition.RightStickPosition.y * m_HeightMultiplier + m_JoystickPosition.
                MaxJoystickRadius * m_Joystick.RightStick.y * m_ScaleFactor),
            m_JoystickPosition.JoystickRadius * m_ScaleFactor,
            IM_COL32(100, 255, 255, 255), 25);
        // Button A
        if (m_Joystick.ButtonA) {
            draw_list->AddCircleFilled(
                ImVec2(canvas_left_top.x + m_JoystickPosition.ButtonAPosition.x * m_WidthMultiplier,
                       canvas_left_top.y + m_JoystickPosition.ButtonAPosition.y * m_HeightMultiplier),
                m_JoystickPosition.ButtonRadius * m_ScaleFactor,
                IM_COL32(50, 255, 50, 100), 10);
        }
        sprintf(buffer, "A");
        draw_list->AddText(
            ImVec2(canvas_left_top.x + m_JoystickPosition.ButtonAPosition.x * m_WidthMultiplier - 5,
                   canvas_left_top.y + m_JoystickPosition.ButtonAPosition.y * m_HeightMultiplier - 5),
            IM_COL32(255, 255, 255, 255), buffer);
        // Button B
        if (m_Joystick.ButtonB) {
            draw_list->AddCircleFilled(
                ImVec2(canvas_left_top.x + m_JoystickPosition.ButtonBPosition.x * m_WidthMultiplier,
                       canvas_left_top.y + m_JoystickPosition.ButtonBPosition.y * m_HeightMultiplier),
                m_JoystickPosition.ButtonRadius * m_ScaleFactor,
                IM_COL32(255, 50, 50, 100), 10);
        }
        sprintf(buffer, "B");
        draw_list->AddText(
            ImVec2(canvas_left_top.x + m_JoystickPosition.ButtonBPosition.x * m_WidthMultiplier - 5,
                   canvas_left_top.y + m_JoystickPosition.ButtonBPosition.y * m_HeightMultiplier - 5),
            IM_COL32(255, 255, 255, 255), buffer);
        // Button X
        if (m_Joystick.ButtonX) {
            draw_list->AddCircleFilled(
                ImVec2(canvas_left_top.x + m_JoystickPosition.ButtonXPosition.x * m_WidthMultiplier,
                       canvas_left_top.y + m_JoystickPosition.ButtonXPosition.y * m_HeightMultiplier),
                m_JoystickPosition.ButtonRadius * m_ScaleFactor,
                IM_COL32(50, 50, 255, 100), 10);
        }
        sprintf(buffer, "X");
        draw_list->AddText(
            ImVec2(canvas_left_top.x + m_JoystickPosition.ButtonXPosition.x * m_WidthMultiplier - 5,
                   canvas_left_top.y + m_JoystickPosition.ButtonXPosition.y * m_HeightMultiplier - 5),
            IM_COL32(255, 255, 255, 255), buffer);
        // Button Y
        if (m_Joystick.ButtonY) {
            draw_list->AddCircleFilled(
                ImVec2(canvas_left_top.x + m_JoystickPosition.ButtonYPosition.x * m_WidthMultiplier,
                       canvas_left_top.y + m_JoystickPosition.ButtonYPosition.y * m_HeightMultiplier),
                m_JoystickPosition.ButtonRadius * m_ScaleFactor,
                IM_COL32(255, 255, 50, 100), 10);
        }
        sprintf(buffer, "Y");
        draw_list->AddText(
            ImVec2(canvas_left_top.x + m_JoystickPosition.ButtonYPosition.x * m_WidthMultiplier - 5,
                   canvas_left_top.y + m_JoystickPosition.ButtonYPosition.y * m_HeightMultiplier - 5),
            IM_COL32(255, 255, 255, 255), buffer);

        //Connect Button
        if (m_Joystick.IsConnected) {
            draw_list->AddCircleFilled(
                ImVec2(canvas_left_top.x + m_JoystickPosition.ConnectButtonPosition.x * m_WidthMultiplier,
                       canvas_left_top.y + m_JoystickPosition.ConnectButtonPosition.y * m_HeightMultiplier),
                m_JoystickPosition.ConnectButtonRadius * m_ScaleFactor,
                IM_COL32(200, 255, 200, 255), 10);
        }
        //Start Button
        if (m_Joystick.Start) {
            draw_list->AddCircleFilled(
                ImVec2(canvas_left_top.x + m_JoystickPosition.StartButtonPosition.x * m_WidthMultiplier,
                       canvas_left_top.y + m_JoystickPosition.StartButtonPosition.y * m_HeightMultiplier),
                m_JoystickPosition.ButtonRadiusCenter * m_ScaleFactor,
                IM_COL32(255, 255, 255, 255), 10);
        }
        //Select Button
        if (m_Joystick.Select) {
            draw_list->AddCircleFilled(
                ImVec2(canvas_left_top.x + m_JoystickPosition.SelectButtonPosition.x * m_WidthMultiplier,
                       canvas_left_top.y + m_JoystickPosition.SelectButtonPosition.y * m_HeightMultiplier),
                m_JoystickPosition.ButtonRadiusCenter * m_ScaleFactor,
                IM_COL32(255, 255, 255, 255), 10);
        }

        //Pad Up
        if (m_Joystick.PadUp) {
            draw_list->AddRectFilled(ImVec2(canvas_left_top.x + m_JoystickPosition.PadUpPosition.x * m_WidthMultiplier,
                                            canvas_left_top.y + m_JoystickPosition.PadUpPosition.y *
                                            m_HeightMultiplier),
                                     ImVec2(
                                         canvas_left_top.x + m_JoystickPosition.PadUpPosition.x * m_WidthMultiplier +
                                         m_JoystickPosition.PadUpPosition.z * m_ScaleFactor,
                                         canvas_left_top.y + m_JoystickPosition.PadUpPosition.y * m_HeightMultiplier +
                                         m_JoystickPosition.PadUpPosition.z * m_ScaleFactor),
                                     IM_COL32(150, 255, 255, 150));
        }
        //Pad Down
        if (m_Joystick.PadDown) {
            draw_list->AddRectFilled(
                ImVec2(canvas_left_top.x + m_JoystickPosition.PadDownPosition.x * m_WidthMultiplier,
                       canvas_left_top.y + m_JoystickPosition.PadDownPosition.y * m_HeightMultiplier),
                ImVec2(
                    canvas_left_top.x + m_JoystickPosition.PadDownPosition.x * m_WidthMultiplier + m_JoystickPosition.
                    PadDownPosition.z * m_ScaleFactor,
                    canvas_left_top.y + m_JoystickPosition.PadDownPosition.y * m_HeightMultiplier + m_JoystickPosition.
                    PadDownPosition.z * m_ScaleFactor),
                IM_COL32(150, 255, 255, 150));
        }
        //Pad Left
        if (m_Joystick.PadLeft) {
            draw_list->AddRectFilled(
                ImVec2(canvas_left_top.x + m_JoystickPosition.PadLeftPosition.x * m_WidthMultiplier,
                       canvas_left_top.y + m_JoystickPosition.PadLeftPosition.y * m_HeightMultiplier),
                ImVec2(
                    canvas_left_top.x + m_JoystickPosition.PadLeftPosition.x * m_WidthMultiplier + m_JoystickPosition.
                    PadLeftPosition.z * m_ScaleFactor,
                    canvas_left_top.y + m_JoystickPosition.PadLeftPosition.y * m_HeightMultiplier + m_JoystickPosition.
                    PadLeftPosition.z * m_ScaleFactor),
                IM_COL32(150, 255, 255, 150));
        }
        //Pad Right
        if (m_Joystick.PadRight) {
            draw_list->AddRectFilled(
                ImVec2(canvas_left_top.x + m_JoystickPosition.PadRightPosition.x * m_WidthMultiplier,
                       canvas_left_top.y + m_JoystickPosition.PadRightPosition.y * m_HeightMultiplier),
                ImVec2(
                    canvas_left_top.x + m_JoystickPosition.PadRightPosition.x * m_WidthMultiplier + m_JoystickPosition.
                    PadRightPosition.z * m_ScaleFactor,
                    canvas_left_top.y + m_JoystickPosition.PadRightPosition.y * m_HeightMultiplier + m_JoystickPosition.
                    PadRightPosition.z * m_ScaleFactor),
                IM_COL32(150, 255, 255, 150));
        }

        //left shoulder , draw a rectangle
        if (m_Joystick.LeftShoulder) {
            draw_list->AddRectFilled(
                ImVec2(canvas_left_top.x + m_JoystickPosition.LeftShoulderPosition.x * m_WidthMultiplier,
                       canvas_left_top.y + m_JoystickPosition.LeftShoulderPosition.y * m_HeightMultiplier),
                ImVec2(
                    canvas_left_top.x + m_JoystickPosition.LeftShoulderPosition.x * m_WidthMultiplier +
                    m_JoystickPosition.SPSize.x * m_ScaleFactor,
                    canvas_left_top.y + m_JoystickPosition.LeftShoulderPosition.y * m_HeightMultiplier +
                    m_JoystickPosition.SPSize.y * m_ScaleFactor),
                IM_COL32(255, 255, 255, 255));
        }
        //right shoulder , draw a rectangle
        if (m_Joystick.RightShoulder) {
            draw_list->AddRectFilled(
                ImVec2(canvas_left_top.x + m_JoystickPosition.RightShoulderPosition.x * m_WidthMultiplier,
                       canvas_left_top.y + m_JoystickPosition.RightShoulderPosition.y * m_HeightMultiplier),
                ImVec2(
                    canvas_left_top.x + m_JoystickPosition.RightShoulderPosition.x * m_WidthMultiplier +
                    m_JoystickPosition.SPSize.x * m_ScaleFactor,
                    canvas_left_top.y + m_JoystickPosition.RightShoulderPosition.y * m_HeightMultiplier +
                    m_JoystickPosition.SPSize.y * m_ScaleFactor),
                IM_COL32(255, 255, 255, 255));
        }

        //left trigger , draw a rectangle empty
        draw_list->AddRectFilled(
            ImVec2(canvas_left_top.x + m_JoystickPosition.LeftTriggerPosition.x * m_WidthMultiplier,
                   canvas_left_top.y + m_JoystickPosition.LeftTriggerPosition.y * m_HeightMultiplier),
            ImVec2(
                canvas_left_top.x + m_JoystickPosition.LeftTriggerPosition.x * m_WidthMultiplier + m_JoystickPosition.
                TriggerSize.x * m_ScaleFactor,
                canvas_left_top.y + m_JoystickPosition.LeftTriggerPosition.y * m_HeightMultiplier + m_JoystickPosition.
                TriggerSize.y * m_ScaleFactor),
            IM_COL32(255, 255, 255, 255));

        float height = m_JoystickPosition.TriggerSize.y * (1 - ((m_Joystick.LeftTrigger + 2) / 2 - 0.5)) - 10;
        sprintf(buffer, "%.2f", ((m_Joystick.LeftTrigger + 2) / 2 - 0.5));
        draw_list->AddText(
            ImVec2(canvas_left_top.x + m_JoystickPosition.LeftTriggerPosition.x * m_WidthMultiplier,
                   canvas_left_top.y + m_JoystickPosition.LeftTriggerPosition.y * m_HeightMultiplier - 20),
            IM_COL32(255, 255, 255, 255), buffer);
        draw_list->AddRectFilled(
            ImVec2(canvas_left_top.x + m_JoystickPosition.LeftTriggerPosition.x * m_WidthMultiplier,
                   canvas_left_top.y + m_JoystickPosition.LeftTriggerPosition.y * m_HeightMultiplier),
            ImVec2(
                canvas_left_top.x + m_JoystickPosition.LeftTriggerPosition.x * m_WidthMultiplier + m_JoystickPosition.
                TriggerSize.x * m_ScaleFactor,
                canvas_left_top.y + height * m_HeightMultiplier + m_JoystickPosition.TriggerSize.y * m_ScaleFactor),
            IM_COL32(0, 0, 0, 255));
        draw_list->AddRect(ImVec2(canvas_left_top.x + m_JoystickPosition.LeftTriggerPosition.x * m_WidthMultiplier,
                                  canvas_left_top.y + m_JoystickPosition.LeftTriggerPosition.y * m_HeightMultiplier),
                           ImVec2(
                               canvas_left_top.x + m_JoystickPosition.LeftTriggerPosition.x * m_WidthMultiplier +
                               m_JoystickPosition.TriggerSize.x * m_ScaleFactor,
                               canvas_left_top.y + m_JoystickPosition.LeftTriggerPosition.y * m_HeightMultiplier +
                               m_JoystickPosition.TriggerSize.y * m_ScaleFactor),
                           IM_COL32(255, 255, 255, 255));


        //right trigger , draw a rectangle empty
        draw_list->AddRectFilled(
            ImVec2(canvas_left_top.x + m_JoystickPosition.RightTriggerPosition.x * m_WidthMultiplier,
                   canvas_left_top.y + m_JoystickPosition.RightTriggerPosition.y * m_HeightMultiplier),
            ImVec2(
                canvas_left_top.x + m_JoystickPosition.RightTriggerPosition.x * m_WidthMultiplier + m_JoystickPosition.
                TriggerSize.x * m_ScaleFactor,
                canvas_left_top.y + m_JoystickPosition.RightTriggerPosition.y * m_HeightMultiplier + m_JoystickPosition.
                TriggerSize.y * m_ScaleFactor),
            IM_COL32(255, 255, 255, 255));
        height = m_JoystickPosition.TriggerSize.y * (1 - ((m_Joystick.RightTrigger + 2) / 2 - 0.5)) - 10;
        sprintf(buffer, "%.2f", ((m_Joystick.RightTrigger + 2) / 2 - 0.5));
        draw_list->AddText(
            ImVec2(canvas_left_top.x + m_JoystickPosition.RightTriggerPosition.x * m_WidthMultiplier,
                   canvas_left_top.y + m_JoystickPosition.RightTriggerPosition.y * m_HeightMultiplier - 20),
            IM_COL32(255, 255, 255, 255), buffer);
        draw_list->AddRectFilled(
            ImVec2(canvas_left_top.x + m_JoystickPosition.RightTriggerPosition.x * m_WidthMultiplier,
                   canvas_left_top.y + m_JoystickPosition.RightTriggerPosition.y * m_HeightMultiplier),
            ImVec2(
                canvas_left_top.x + m_JoystickPosition.RightTriggerPosition.x * m_WidthMultiplier + m_JoystickPosition.
                TriggerSize.x * m_ScaleFactor,
                canvas_left_top.y + height * m_HeightMultiplier + m_JoystickPosition.TriggerSize.y * m_ScaleFactor),
            IM_COL32(0, 0, 0, 255));
        draw_list->AddRect(ImVec2(canvas_left_top.x + m_JoystickPosition.RightTriggerPosition.x * m_WidthMultiplier,
                                  canvas_left_top.y + m_JoystickPosition.RightTriggerPosition.y * m_HeightMultiplier),
                           ImVec2(
                               canvas_left_top.x + m_JoystickPosition.RightTriggerPosition.x * m_WidthMultiplier +
                               m_JoystickPosition.TriggerSize.x * m_ScaleFactor,
                               canvas_left_top.y + m_JoystickPosition.RightTriggerPosition.y * m_HeightMultiplier +
                               m_JoystickPosition.TriggerSize.y * m_ScaleFactor),
                           IM_COL32(255, 255, 255, 255));


        draw_list->AddRect(canvas_left_top, canvas_bottom_right, IM_COL32(255, 255, 255, 255));
        draw_list->PushClipRect(canvas_left_top, canvas_bottom_right, true);
        draw_list->PopClipRect();


        ImGui::End();
    }
}
