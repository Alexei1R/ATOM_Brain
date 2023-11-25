#include "Frame.h"
#include "opencv2/opencv.hpp"

namespace Atom {

    void MatToTexture(const cv::Mat& mat, GLuint& texture) {
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mat.cols, mat.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, mat.ptr());
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Frame::Frame()
            : Layer("Frame") {
        cv::namedWindow("Received Video", cv::WINDOW_NORMAL);
        cv::resizeWindow("Received Video", 640, 480);

        m_VideoCapture.Open("udpsrc port=5000 ! application/x-rtp, payload=96 ! rtpjpegdepay ! jpegdec ! videoconvert ! appsink");

        if (!m_VideoCapture.isOpened()) {
            std::cout << "Error opening video stream" << std::endl;
        }

        m_VideoCapture.SetFrameRecivedCallback([&](cv::Mat& frame) {
            m_Frame = frame;
        });

        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Frame::~Frame() {}

    void Frame::OnAttach() {}

    void Frame::OnDetach() {}

    void Frame::OnUpdate() {


    }

    void Frame::OnImGuiRender() {

//        m_Frame = m_VideoCapture.GetFrame();

        if(m_VideoCapture.isOpened()){
            if (m_Frame.empty()) {
                std::cout << "End of video stream" << std::endl;
            }
            MatToTexture(m_Frame, textureID);

            ImGui::Begin("Received Video");

            ImVec2 size(m_Frame.cols, m_Frame.rows);
            ImGui::Image((void*)(intptr_t)textureID, size);

            ImGui::End();
        }
    }
}
