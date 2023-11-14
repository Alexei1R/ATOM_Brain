//
// Created by toor on 11/14/23.
//

#include "ImuLayer.h"


namespace Atom {
    ImuLayer::ImuLayer()
            : Layer("ImuLayer") {
        m_Server = new Server(27020);
        m_Server->Start();
        m_Server->SetClientConnectedCallback([&](const ClientInfo& info) {
            ATLOG_INFO("Client Connected: {0}", info.ConnectionDesc);
        });
        m_Server->SetClientDisconnectedCallback([&](const ClientInfo& info) {
            ATLOG_INFO("Client Disconnected: {0}", info.ConnectionDesc);
        });

        m_Server->SetDataReceivedCallback([&](const ClientInfo& info, const void* data, unsigned int size) {
            ATLOG_INFO("Data Received from {0}: {1} bytes", info.ConnectionDesc, size);
//            also print message as string
            ATLOG_INFO("Data Received from {0}: {1} bytes", info.ConnectionDesc, (char*)data);
            m_Server->SendDataToClient(info.ID, data, size);
        });

    }

    ImuLayer::~ImuLayer() {
    }


    void ImuLayer::OnAttach() {
    }

    void ImuLayer::OnDetach() {
    }

    void ImuLayer::OnUpdate() {


    }

    void ImuLayer::OnImGuiRender() {

    }


}