//
// Created by toor on 11/20/23.
//

#ifndef ATOM_FRAME_H
#define ATOM_FRAME_H

#include "glad/gl.h"

#include "ATOM/atompch.h"
#include "Networking/Client.h"
#include <opencv2/opencv.hpp>
#include "VideoCapture/VideoCapture.h"


namespace Atom {
    class Frame : public Layer {
    public:
        Frame();

        ~Frame();

        virtual void OnAttach() override;

        virtual void OnDetach() override;

        virtual void OnUpdate() override;

        virtual void OnFixedUpdate() override;

        virtual void OnImGuiRender() override;

        static cv::Mat &GetNativeFrame() { return s_Frame; }
        bool IsCaptureOpened() { return m_CaptureOpened; }

        void OpenVideoCapture();

        void Shutdown();

    private:
        Atom::VideoCapture m_VideoCapture;
        cv::Mat m_Frame;
        static cv::Mat s_Frame;
        GLuint m_Texture;
        float m_AspectRatio = 0;
        bool m_CaptureOpened = false;
        bool m_IsFocused = false;
    };
}


#endif //ATOM_FRAME_H
