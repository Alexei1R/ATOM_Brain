//
// Created by toor on 12/28/23.
//

#ifndef CLIENTLAYER_H
#define CLIENTLAYER_H


#include "ATOM/atompch.h"
#include "Networking/Client.h"
#include "Networking/Server.h"

namespace Atom {
    typedef struct Message {
        uint8_t id;
        size_t payloadSize;
        void *payload;
    } Message;

    class ClientLayer : public Layer {
    public:
    public:
        ClientLayer();

        ~ClientLayer();

        virtual void OnAttach() override;

        virtual void OnDetach() override;

        virtual void OnUpdate() override;

        virtual void OnFixedUpdate() override;

        virtual void OnImGuiRender() override;

        void ConnectToServer(std::string ip);

        bool IsRunning() { return m_Client->IsRunning(); }
        void Shutdown() { m_Client->Shutdown(); };

        void SendMessage(const Message &message, bool isReliable = true);

        void RegisterMessageWithID(uint8_t id, const std::function<void(Message message)> &function) {
            m_RegistedMessageCallbacks.push_back({{id, function}});
        }

    private:
        Client *m_Client;
        std::vector<std::map<uint8_t, std::function<void(Message message)> > > m_RegistedMessageCallbacks;
    };
}


#endif //CLIENTLAYER_H
