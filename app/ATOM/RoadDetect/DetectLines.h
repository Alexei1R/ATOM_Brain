//
// Created by toor on 2/4/24.
//

#ifndef DETECTLINES_H
#define DETECTLINES_H

#include "ATOM/atompch.h"
#include <opencv2/opencv.hpp>
#include "ATOM/RoadDetect/FindLines.h"

namespace Atom {
    class DetectLines : public Layer {
    public:
        DetectLines();

        ~DetectLines();

        virtual void OnAttach() override;

        virtual void OnDetach() override;

        virtual void OnUpdate() override;

        virtual void OnFixedUpdate() override;

        virtual void OnImGuiRender() override;


        //get ofset center
        [[nodiscard]] float GetOffsetCenter() const { return m_FindLines->GetOffsetCenter(); }

    private:
        cv::Mat *m_Frame;
        FindLines *m_FindLines;
        LineSettings *m_LineSettings;

        GLuint m_Texture;
        bool hasBeenCreated = false;
        //white mask
        int h = 167;
        int s = 129;
        int v = 120;

        bool showDebugWindow = true;
        bool showDebug = false;

        float m_AspectRatio = 0;
        char buffer[100];
        bool m_FrameExist = false;
    };
}

#endif //DETECTLINES_H
