//
// Created by toor on 11/7/23.
//

#ifndef ATOM_APPLICATION_H
#define ATOM_APPLICATION_H


#include "ATOM/Core/Layers/Layer.h"
#include "ATOM/Core/Layers/LayerStack.h"
#include "ATOM/Core/Window.h"
#include "ATOM/Core/Editor/ImGuiLayer.h"
#include "ATOM/Core/Editor/EditorLayer.h"
#include "ATOM/Sensors/Frame.h"
#include "ATOM/Core/ClientLayer.h"
#include "ATOM/Map/DrawMap.h"
#include "ATOM/GamePad/Gamepad.h"
#include "ATOM/RoadDetect/DetectLines.h"

namespace Atom {
    class Application {
    public:
        Application();

        ~Application();

        void Run();

        void PushLayer(Layer *layer);

        void PushOverlay(Layer *layer);

        static Application *s_Instance;
        inline static Application &GetApp() { return *s_Instance; }
        [[nodiscard]] inline Window &GetWindow() const { return *m_Window; }
        [[nodiscard]] inline Frame &GetFrame() const { return *m_Frame; }
        [[nodiscard]] inline Gamepad &GetGamepad() const { return *m_Gamepad; }

        void WindowClose();


        void SelectIPPopUpWindow();

        void DrawUISetings();

        void DrawCameraSettings();

        void DrawMapSettings();

    private:
        bool m_IsRuning = true;
        bool isConnected = false;
        bool m_VSync = true;

        Window *m_Window;
        LayerStack m_LayerStack;
        ImGuiLayer *m_ImGuiLayer;
        EditorLayer *m_EditorLayer;
        Frame *m_Frame;
        ClientLayer *m_ClientLayer;
        DrawMap *m_DrawMap;
        Gamepad *m_Gamepad;
        DetectLines *m_DetectLines;

        bool isOpenCameraComandSent = false;

        int counter = 0;
        float sliderValue = 0;
        float lastsliderValue = 0;
        float angle = 0;
        float lastAngle = 0;
        float speed = 0;
        float steering = 0;
        int axesCount{};
        float xAxisRightLast = 0;
        float xAxisLeftLast = 0;
        float xAxisRightMaxValue = 0;
        float xAxisLeftMaxValue = 0;
    };
}


#endif //ATOM_APPLICATION_H
