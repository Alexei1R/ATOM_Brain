#include "Frame.h"
#include "ATOM/Application.h"
#include "opencv2/opencv.hpp"

namespace Atom {

    Frame::Frame()
            : Layer("Frame") {
        m_Renderer = (SDL_Renderer*)Application::GetApp().GetWindow().GetRederer();
        m_Texture = nullptr;


        m_VideoCapture.Open("udpsrc port=5000 ! application/x-rtp,encoding-name=JPEG,payload=26 ! rtpjpegdepay ! jpegdec ! videoconvert ! appsink sync=false");

        if (!m_VideoCapture.isOpened()) {
            std::cout << "Error opening video stream" << std::endl;
        }

        m_VideoCapture.SetFrameRecivedCallback([&](cv::Mat& frame) {
            m_Frame = frame;
        });


        // m_Frame = cv::imread("ASSETS/bot.jpg");
        // if (m_Frame.empty()) {
        //     std::cout << "Could not open or find the image" << std::endl;
        // }


        // cap = std::make_unique<cv::VideoCapture>("/home/toor/Downloads/pc.mp4");
        // if (!cap->isOpened()) {
        //     std::cout << "Error opening video stream" << std::endl;
        // }




    }

    Frame::~Frame()
    {
        SDL_DestroyTexture(m_Texture);
    }

    void Frame::OnAttach() {}

    void Frame::OnDetach() {}

    void Frame::OnUpdate() {
        // cap->read(m_Frame);

        if(!m_Frame.empty()){
            if (m_Texture == nullptr) {
                m_Texture = SDL_CreateTexture(m_Renderer, SDL_PIXELFORMAT_BGR24, SDL_TEXTUREACCESS_STREAMING, m_Frame.cols, m_Frame.rows);
            }
            SDL_UpdateTexture(m_Texture, nullptr, m_Frame.data, m_Frame.cols * 3);
        }

    }

    void Frame::OnImGuiRender() {
        ImGui::Begin("Received Video");
        ImGui::Image((void*)(intptr_t)m_Texture, ImVec2(640, 480));
        ImGui::End();
    }

    void Frame::Shutdown() {
        m_VideoCapture.Close();
    }

}