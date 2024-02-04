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
        bool IsConnected;
        int Index;
        int count;

        glm::vec2 LeftStick;
        bool LeftStickButton;
        glm::vec2 RightStick;
        bool RightStickButton;

        // Pad
        bool PadUp;
        bool PadDown;
        bool PadLeft;
        bool PadRight;

        // Triggers
        float LeftTrigger;
        float RightTrigger;

        // Shoulders
        bool LeftShoulder;
        bool RightShoulder;

        // Start and Select
        bool Start;
        bool Select;

        // Buttons
        bool ButtonA;
        bool ButtonB;
        bool ButtonX;
        bool ButtonY;
    };

    class Gamepad : public Layer {
    public:
        Gamepad();

        ~Gamepad();

        virtual void OnAttach() override;

        virtual void OnDetach() override;

        virtual void OnUpdate() override;

        virtual void OnImGuiRender() override;

    private:
        static void joystick_callback(int jid, int event);
        void joystick_callback_non_static(int jid, int event);

    private:
        Joystick m_Joystick;
    };
}

#endif // GAMEPAD_H
