#include "Frame.h"
#include "ATOM/Application.h"
#include "opencv2/opencv.hpp"


namespace Atom {
    cv::Mat Frame::s_Frame = cv::Mat();

    void FrameTexture(const cv::Mat &mat, GLuint &texture) {
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mat.cols, mat.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, mat.ptr());
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Frame::Frame()
        : Layer("Frame") {
        glGenTextures(1, &m_Texture);
        glBindTexture(GL_TEXTURE_2D, m_Texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Frame::~Frame() {
    }

    void Frame::OnAttach() {
    }

    void Frame::OnDetach() {
    }

    void Frame::OnUpdate() {
    }

    void Frame::OnFixedUpdate() {
        if(m_IsFocused) {
            FrameTexture(m_Frame, m_Texture);
        }
    }

    void Frame::OnImGuiRender() {
        if (m_CaptureOpened) {
            ImGui::Begin("Received Video");
            m_IsFocused = ImGui::IsWindowFocused();
            if (m_IsFocused) {
                if (m_VideoCapture.isOpened()) {
                    if (m_Frame.empty()) {
                        std::cout << "End of video stream" << std::endl;
                        ImGui::End();
                        return;
                    }

                    m_AspectRatio = (float) m_Frame.cols / (float) m_Frame.rows;
                    if (ImGui::GetContentRegionAvail().x > ImGui::GetContentRegionAvail().y * m_AspectRatio) {
                        ImVec2 available = ImGui::GetContentRegionAvail();
                        ImGui::SetCursorPosX(
                            ImGui::GetCursorPosX() + (available.x / 2 - available.y * m_AspectRatio / 2));
                        ImGui::Image((void *) m_Texture, ImVec2(available.y * m_AspectRatio, available.y), ImVec2(1, 0),
                                     ImVec2(0, 1));
                    } else {
                        ImVec2 available = ImGui::GetContentRegionAvail();
                        ImGui::SetCursorPosY(
                            ImGui::GetCursorPosY() + (available.y / 2 - available.x / m_AspectRatio / 2));
                        ImGui::Image((void *) m_Texture, ImVec2(available.x, available.x / m_AspectRatio), ImVec2(1, 0),
                                     ImVec2(0, 1));
                    }
                }
            } else {
                ImGui::SetCursorPosX(
                    (ImGui::GetWindowSize().x - ImGui::CalcTextSize("Window is not focused").x) * 0.5f);
                ImGui::Text("Window is not focused");
            }

            ImGui::End();
        }
    }

    void Frame::OpenVideoCapture() {
        m_VideoCapture.Open(
            "udpsrc port=5000 ! application/x-rtp,encoding-name=JPEG,payload=26 ! rtpjpegdepay ! jpegdec ! videoconvert ! appsink sync=false");


        m_VideoCapture.SetFrameRecivedCallback([&](cv::Mat &frame) {
            // cv::flip(frame,frame,0);
                //flip horizontal and vertical
                    // cv::flip(frame, frame, -1);
            m_Frame = frame;
            s_Frame = frame;
            m_CaptureOpened = true;
        });
        ATLOG_WARN("Register callback for frame")
    }

    void Frame::Shutdown() {
        m_VideoCapture.Close();
    }
}
