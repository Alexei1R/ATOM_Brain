//
// Created by toor on 3/9/24.
//

#ifndef CARSTATE_H
#define CARSTATE_H

#include "ATOM/atompch.h"
#include "ATOM/GamePad/Gamepad.h"
#include "ATOM/Map/DrawMap.h"
#include "ATOM/RoadDetect/DetectLines.h"
#include "ATOM/Core/ClientLayer.h"
#include "ATOM/PID/pid.h"

namespace Atom {
    enum class CarStateEnum {
        IDLE,
        AUTONOMOUS,
        STOPPED,
        TURN_LEFT,
        TURN_RIGHT,
        MOVE_FORWARD,
    };


    typedef struct StateMasine {
        CarStateEnum state = CarStateEnum::IDLE;
    } StateMasine;


    class CarState : public Layer {
    public:
        CarState(Gamepad *gamepad, DrawMap *drawMap, ClientLayer *clientLayer, DetectLines *detectLines);

        ~CarState();

        virtual void OnAttach() override;

        virtual void OnDetach() override;

        virtual void OnUpdate() override;

        virtual void OnFixedUpdate() override;

        virtual void OnImGuiRender() override;

        void ComandCarSpeed(float speed);

        void ComandCarSteering(float angle);


    private:
        StateMasine m_StateMasine;
        bool m_CheckBoxState = false;
        int m_MimSignDistance = 200;


        Gamepad *m_Gamepad;
        DrawMap *m_DrawMap;
        ClientLayer *m_ClientLayer;
        DetectLines *m_DetectLines;
        std::chrono::time_point<std::chrono::high_resolution_clock> lastTime =
                std::chrono::high_resolution_clock::now();

        //update time for state
        std::chrono::time_point<std::chrono::high_resolution_clock> lastTimeState =
                std::chrono::high_resolution_clock::now();
        std::chrono::time_point<std::chrono::high_resolution_clock> timenow;
        int deltatime = 1;

        bool m_CarStop = false;
        int sleepFor = 1000;
        int m_IterationCount = 0;


        bool m_GamepadChanged = false;

        float m_MaxSpeed = -10;
        float m_MaxSteering = 45;
        float m_OffsetSteering = 0;


        int m_TimeToTurnLeft = 10;
        int m_TimeToTurnRight = 10;
        int m_TimeToMoveForward = 10;


        //PID
        PID *m_Pid;
        // PID Constants
        float PID_KP = 5.0f;
        float PID_KI = 0.5f;
        float PID_KD = 0.025f;
        //Pid Out
        float m_PidOut = 0;
        float error;
        bool m_PidChanged = false;
        //lastLime pid out
        std::chrono::time_point<std::chrono::high_resolution_clock> lastTimePid =
                std::chrono::high_resolution_clock::now();
    };
}


#endif //CARSTATE_H
