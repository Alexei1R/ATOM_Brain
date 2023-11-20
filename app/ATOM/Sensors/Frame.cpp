//
// Created by toor on 11/20/23.
//

#include "Frame.h"
#include "opencv2/opencv.hpp"



namespace Atom {
    Frame::Frame()
            : Layer("Lidar") {

        m_Client = new Client();
        m_Client->ConnectToServer("192.168.1.103:27020");
        m_Client->SetDataReceivedCallback([&](const void* data, unsigned int size) {
            // Assuming 'data' contains image bytes received from the server
            std::vector<uchar> imageBytes(static_cast<const uchar*>(data), static_cast<const uchar*>(data) + size);

            // Convert the received byte data back into an OpenCV Mat
            cv::Mat img = cv::imdecode(imageBytes, cv::IMREAD_COLOR);

            if (!img.empty()) {
                // Display the image using OpenCV
                cv::imshow("Received Image", img);
                cv::waitKey(1); // Add a small delay to allow window rendering and event processing
            } else {
                ATLOG_ERROR("Failed to decode the received image data");
            }
        });
        m_Client->SetServerConnectedCallback([&]() {
            ATLOG_INFO("Connected to server");
        });
        m_Client->SetServerDisconnectedCallback([&]() {
            ATLOG_INFO("Disconnected from server");
        });




    }

    Frame::~Frame() {
    }


    void Frame::OnAttach() {

    }

    void Frame::OnDetach() {
    }

    void Frame::OnUpdate() {



    }

    void Frame::OnImGuiRender() {

    }


}