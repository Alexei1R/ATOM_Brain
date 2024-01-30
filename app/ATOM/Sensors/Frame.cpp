#include "Frame.h"
#include "ATOM/Application.h"
#include "opencv2/opencv.hpp"



namespace Atom {
    void FrameTexture(const cv::Mat &mat, GLuint &texture) {
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mat.cols, mat.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, mat.ptr());
        glBindTexture(GL_TEXTURE_2D, 0);
    }


    Frame::Frame()
            : Layer("Frame") {

        m_VideoCapture.Open("udpsrc port=5000 ! application/x-rtp,encoding-name=JPEG,payload=26 ! rtpjpegdepay ! jpegdec ! videoconvert ! appsink sync=false");


        m_VideoCapture.SetFrameRecivedCallback([&](cv::Mat& frame) {
            m_Frame = frame;
        });
        ATLOG_WARN("Register callback for frame")
        glGenTextures(1, &m_Texture);
        glBindTexture(GL_TEXTURE_2D, m_Texture);
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
        ImGui::Begin("Received Video");
        if(ImGui::IsWindowFocused()){
            if(m_VideoCapture.isOpened()){
                if (m_Frame.empty()) {
                    std::cout << "End of video stream" << std::endl;
                }

                FrameTexture(m_Frame, m_Texture);
                m_AspectRatio = (float) m_Frame.cols / (float) m_Frame.rows;
//                ATLOG_ERROR("Aspect ratio: {0}", m_AspectRatio)


                if (ImGui::GetContentRegionAvail().x > ImGui::GetContentRegionAvail().y * m_AspectRatio) {
                    ImVec2 available = ImGui::GetContentRegionAvail();
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (available.x / 2 - available.y * m_AspectRatio / 2));
                    ImGui::Image((void *) m_Texture, ImVec2(available.y * m_AspectRatio, available.y), ImVec2(1, 0),
                                 ImVec2(0, 1));
                } else {
                    ImVec2 available = ImGui::GetContentRegionAvail();
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (available.y / 2 - available.x / m_AspectRatio / 2));
                    ImGui::Image((void *) m_Texture, ImVec2(available.x, available.x / m_AspectRatio), ImVec2(1, 0),
                                 ImVec2(0, 1));
                }
//
            }
        }
        else{
            ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Window is not focused").x) * 0.5f);
            ImGui::Text("Window is not focused");
        }

        ImGui::End();

    }

    void Frame::Shutdown() {

    }
}
