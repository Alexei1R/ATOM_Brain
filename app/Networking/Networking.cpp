//
// Created by toor on 11/14/23.
//

#include "Networking.h"

static void NukeProcess( int rc )
{
#ifdef _WIN32
    ExitProcess( rc );
#else
    (void)rc; // Unused formal parameter
    kill( getpid(), SIGKILL );
#endif
}





namespace Atom{
    Server::Server(NetSpecs specs)
    :m_Specs(specs)
    {

    }

    Server::~Server() {

    }

    [[noreturn]] void Server::ServerStart() {
        m_Interface = SteamNetworkingSockets();
        m_ServerLocalAddr.Clear();
        m_ServerLocalAddr.m_port = m_Specs.Port;
        m_Opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged,(void*)SteamNetConnectionStatusChangedCallback);


        m_ListenSocket = m_Interface->CreateListenSocketIP(m_ServerLocalAddr, 1, &m_Opt);
        if(m_ListenSocket == k_HSteamListenSocket_Invalid){
            ATLOG_CRITICAL("Failed to create listen socket on port {1}", m_Specs.Port);
            NukeProcess(1);
        }
        m_PollGroup = m_Interface->CreatePollGroup();
        if(m_PollGroup == k_HSteamNetPollGroup_Invalid){
            ATLOG_CRITICAL("Failed to create poll group");
            NukeProcess(1);
        }
        ATLOG_INFO("Server listening on port {0}", m_Specs.Port);

        while(true){
            PollIncomingMessages();

            m_Interface->RunCallbacks();

        }


    }

    void Server::ServerStop() {

    }



    template<typename T>
    void Server::SendDataToAllClient(const T &data, HSteamNetConnection except, bool reliable) {
        for(auto& client : m_MapOfClients){
            if(client.first != except)
                SendDataToClient(client.first, data, reliable);
        }

    }


    template<typename T>
    void Server::SendDataToClient(ClientID clientID, const T &data, bool reliable) {
        unsigned  int dataSize = sizeof(T);
        if(m_Interface != nullptr){
            m_Interface->SendMessageToConnection(clientID, &data, dataSize, reliable, nullptr);
        }
    }

    void Server::PollIncomingMessages() {
        while (true){
            ISteamNetworkingMessage *pIncomingMsg = nullptr;
            int numMsgs = m_Interface->ReceiveMessagesOnPollGroup(m_PollGroup, &pIncomingMsg, 1);
            if(numMsgs == 0){
                break;
            }
            if(numMsgs < 0){
                ATLOG_CRITICAL("Error checking for messages");
                NukeProcess(1);
            }
            assert(numMsgs == 1 && pIncomingMsg);
            const void * data = pIncomingMsg->GetData();
            int size = pIncomingMsg->GetSize();
            if(pIncomingMsg->m_conn == k_HSteamNetConnection_Invalid){
                ATLOG_CRITICAL("Message from unknown source");
            }

            if(m_OnMessageReceivedCallback){
                m_OnMessageReceivedCallback(pIncomingMsg->m_conn, data);
            }

        }
    }

    void Server::SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t *pInfo) {

    }


}