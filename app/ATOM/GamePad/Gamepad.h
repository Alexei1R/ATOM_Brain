//
// Created by toor on 2/4/24.
//
// Gamepad.h
#ifndef GAMEPAD_H
#define GAMEPAD_H

#include "glad/gl.h"
#include "ATOM/atompch.h"

namespace Atom {
    struct Joystick {
        std::string Name;
        bool IsConnected = false;
        int Index;
        int count;

        glm::vec2 LeftStick;
        bool LeftStickButton;
        glm::vec2 RightStick;
        bool RightStickButton;

        // Pad
        bool PadUp = false;
        bool PadDown = false;
        bool PadLeft = false;
        bool PadRight = false;

        // Triggers
        float LeftTrigger = false;
        float RightTrigger = false;

        // Shoulders
        bool LeftShoulder = false;
        bool RightShoulder = false;

        // Start and Select
        bool Start = false;
        bool Select = false;

        // Buttons
        bool ButtonA = false;
        bool ButtonB = false;
        bool ButtonX = false;
        bool ButtonY = false;

        //dead zone
        float DeadZone = 0.05f;
        float treshold = 0.1f;

        //change
        bool isChanged = false;

        // Center button
        bool CenterButton;
    };

    typedef struct JoystickPosition {
        glm::vec2 LeftStickPosition = glm::vec2(323, 260);
        glm::vec2 RightStickPosition = glm::vec2(500, 260);

        glm::vec2 ButtonAPosition = glm::vec2(577, 198);
        glm::vec2 ButtonBPosition = glm::vec2(611, 162);
        glm::vec2 ButtonXPosition = glm::vec2(544, 162);
        glm::vec2 ButtonYPosition = glm::vec2(577, 127);

        glm::vec2 ConnectButtonPosition = glm::vec2(413, 161);
        glm::vec2 StartButtonPosition = glm::vec2(364, 161);
        glm::vec2 SelectButtonPosition = glm::vec2(462, 161);

        glm::vec3 PadUpPosition = glm::vec3(235, 122, 21);
        glm::vec3 PadDownPosition = glm::vec3(235, 171, 21);
        glm::vec3 PadLeftPosition = glm::vec3(211, 147, 21);
        glm::vec3 PadRightPosition = glm::vec3(260, 147, 21);

        glm::vec2 LeftShoulderPosition = glm::vec2(215, 60);
        glm::vec2 RightShoulderPosition = glm::vec2(535, 60);

        glm::vec2 SPSize = glm::vec2(70, 8);

        glm::vec2 LeftTriggerPosition = glm::vec2(70, 60);
        glm::vec2 RightTriggerPosition = glm::vec2(750, 60);

        glm::vec2 TriggerSize = glm::vec2(20, 70);

        float ConnectButtonRadius = 22.0f;
        float ButtonRadius = 17.0f;
        float ButtonRadiusCenter = 15.0f;
        float JoystickRadius = 15.0f;
        float MaxJoystickRadius = 45.0f;
    } JoystickPosition;

    enum class JoystickAxis {
        LeftX = 0,
        LeftY = 1,
        RightX = 2,
        RightY = 3,
        LeftTrigger = 4,
        RightTrigger = 5
    };

    class Gamepad : public Layer {
    public:
        using GamepadChageState = std::function<void(float value,JoystickAxis joyAxis)>;

    public:
        Gamepad();

        ~Gamepad();

        virtual void OnAttach() override;

        virtual void OnDetach() override;

        virtual void OnUpdate() override;

        virtual void OnFixedUpdate() override;

        virtual void OnImGuiRender() override;

        Joystick *GetJoystickState() { return &m_Joystick; }
        void SetChangeState(GamepadChageState state) { m_JoystickChangeState = state; }

    private:
        static void joystick_callback(int jid, int event);

        void joystick_callback_non_static(int jid, int event);

    private:
        Joystick m_Joystick;
        static Joystick s_Joystick;
        JoystickPosition m_JoystickPosition;
        unsigned int m_JoystickTexture;
        unsigned char *m_JoystickImage;
        int m_JoystickImageWidth, m_JoystickImageHeight;
        int m_JoystickImageChannels;

        GamepadChageState m_JoystickChangeState;

        float m_AspectRatio;
        float m_WidthMultiplier;
        float m_HeightMultiplier;
        float m_ScaleFactor;
        float m_FirstScaleFactor;
        float m_FirstCanvasWidth = 0.0f;
        float m_FirstCanvasHeight = 0.0f;
        char buffer[100];
    };
}

#endif // GAMEPAD_H
