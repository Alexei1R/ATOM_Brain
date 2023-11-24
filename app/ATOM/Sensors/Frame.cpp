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

        m_VideoCapture.open("udpsrc port=5000 ! application/x-rtp, payload=96 ! rtpjpegdepay ! jpegdec ! videoconvert ! appsink", cv::CAP_GSTREAMER);

        if (!m_VideoCapture.isOpened()) {
            std::cout << "Error opening video stream" << std::endl;
        }

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

        m_VideoCapture >> m_Frame;

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
