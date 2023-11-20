//
// Created by toor on 11/20/23.
//

#include "Frame.h"




namespace Atom {
    Frame::Frame()
            : Layer("Lidar") {

        m_Client = new Client();
        m_Client->ConnectToServer("192.168.1.103:27020");
        m_Client->SetDataReceivedCallback([&](const void* data, unsigned int size) {
            ATLOG_INFO("Data Received: {0} bytes", size)
            ATLOG_INFO("Message: {0}", std::string ((char*)data));
        });
        m_Client->SetServerConnectedCallback([&]() {
            ATLOG_INFO("Connected to server");
            m_Client->SendData("Hello from client", 17);
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