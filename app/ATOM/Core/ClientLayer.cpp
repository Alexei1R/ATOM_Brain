//
// Created by toor on 12/28/23.
//

#include "ClientLayer.h"

Atom::ClientLayer::ClientLayer() {

    m_Client = new Client();
    m_Client->SetDataReceivedCallback([&](const void* data, unsigned int size) {
        // ATLOG_INFO("Data Received: {0} bytes", size);
        try {
            if (size < sizeof(uint8_t) + sizeof(size_t)) {
                throw std::runtime_error("Received data is too small to contain a valid message.");
            }
            const char* buffer = static_cast<const char*>(data);
            uint8_t messageID = *buffer;
            // ATLOG_INFO("Message ID: {0}", messageID);
            for (auto map : m_RegistedMessageCallbacks) {
                if (map.find(messageID) != map.end()) {
                    Message message;
                    message.id = messageID;
                    message.payloadSize = size - sizeof(uint8_t) - sizeof(size_t);
                    message.payload = (void*)(buffer + sizeof(uint8_t) + sizeof(size_t));
                    map[messageID](message);
                }
            }

        } catch (std::exception& e) {
            ATLOG_ERROR("Error: {0}", e.what());
        }
    });

    m_Client->SetServerConnectedCallback([&]() {
        ATLOG_INFO("Connected to server");
    });
    m_Client->SetServerDisconnectedCallback([&]() {
        ATLOG_INFO("Disconnected from server");
    });
}

Atom::ClientLayer::~ClientLayer() {
    if (m_Client) {
        delete m_Client;
    }
}

void Atom::ClientLayer::OnAttach() {
}

void Atom::ClientLayer::OnDetach() {
}

void Atom::ClientLayer::OnUpdate() {
}

void Atom::ClientLayer::OnFixedUpdate() {
    Layer::OnFixedUpdate();
}

void Atom::ClientLayer::OnImGuiRender() {
}

void Atom::ClientLayer::ConnectToServer(std::string ip) {
    m_Client->ConnectToServer(ip);
}

void Atom::ClientLayer::SendMessage(const Message& message, bool isReliable) {
    size_t totalSize = sizeof(message.id) + sizeof(message.payloadSize) + message.payloadSize;
    char* buffer = new char[totalSize];
    std::memcpy(buffer, &message.id, sizeof(message.id));
    std::memcpy(buffer + sizeof(message.id), &message.payloadSize, sizeof(message.payloadSize));
    std::memcpy(buffer + sizeof(message.id) + sizeof(message.payloadSize), message.payload, message.payloadSize);
    m_Client->SendDataPointer(buffer, totalSize);
    delete[] buffer;
}
