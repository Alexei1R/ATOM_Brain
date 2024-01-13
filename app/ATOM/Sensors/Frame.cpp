#include "Frame.h"
#include "opencv2/opencv.hpp"

namespace Atom {



    Frame::Frame()
            : Layer("Frame") {
        cv::namedWindow("Received Video", cv::WINDOW_NORMAL);
        cv::resizeWindow("Received Video", 640, 480);

        m_VideoCapture.Open("udpsrc port=5000 ! application/x-rtp,encoding-name=JPEG,payload=26 ! rtpjpegdepay ! jpegdec ! videoconvert ! appsink sync=false");

        if (!m_VideoCapture.isOpened()) {
            std::cout << "Error opening video stream" << std::endl;
        }

        m_VideoCapture.SetFrameRecivedCallback([&](cv::Mat& frame) {
            m_Frame = frame;
        });


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
            ImGui::Begin("Received Video");

            ImVec2 size(m_Frame.cols, m_Frame.rows);
            // ImGui::Image((void*)(intptr_t) , size); // Display image
            ImGui::Image((void*)(intptr_t) m_Frame.data, size); // Display image

            ImGui::End();
        }
    }
}
