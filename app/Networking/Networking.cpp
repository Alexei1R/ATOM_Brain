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
    static Server* s_Instance = nullptr;

    Server::Server(NetSpecs specs)
    :m_Specs(specs)
    {

    }

    Server::~Server() {
        if (m_NetworkThread.joinable())
            m_NetworkThread.join();
    }

    void Server::ServerStart() {


        m_NetworkThread = std::thread(
                [this](){

                    s_Instance = this;
                    m_Running = true;


                    m_Interface = SteamNetworkingSockets();
                    m_ServerLocalAddr.Clear();
                    m_ServerLocalAddr.m_port = m_Specs.Port;
                    m_Opt.SetPtr(k_ESteamNetworkingConfig_Callback_ConnectionStatusChanged,(void*)Server::SteamNetConnectionStatusChangedCallback);


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

                    while(m_Running){
                        PollIncomingMessages();
                        PollConnectionStateChanges();


                    }
                    // Close all the connections
                    std::cout << "Closing connections..." << std::endl;
                    for (const auto& [clientID, clientInfo] : m_MapOfClients)
                    {
                        m_Interface->CloseConnection(clientID, 0, "Server Shutdown", true);
                    }

                    m_MapOfClients.clear();

                    m_Interface->CloseListenSocket(m_ListenSocket);
                    m_ListenSocket = k_HSteamListenSocket_Invalid;

                    m_Interface->DestroyPollGroup(m_PollGroup);
                    m_PollGroup = k_HSteamNetPollGroup_Invalid;



                }
                );





    }

    void Server::ServerStop() {
        m_Running = false;
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
        while (m_Running){
            ISteamNetworkingMessage *pIncomingMsg = nullptr;
            int numMsgs = m_Interface->ReceiveMessagesOnPollGroup(m_PollGroup, &pIncomingMsg, 1);
            if(numMsgs == 0){
                break;
            }
            if(numMsgs < 0){
                ATLOG_CRITICAL("Error checking for messages");
//                NukeProcess(1);
            }
//            assert(numMsgs == 1 && pIncomingMsg);

            auto itClient = m_MapOfClients.find(pIncomingMsg->m_conn);
            if (itClient == m_MapOfClients.end())
            {
                ATLOG_ERROR("Received data from unregistered client");
                continue;
            }
            const void * data = pIncomingMsg->GetData();
            int size = pIncomingMsg->GetSize();
            if(pIncomingMsg->m_conn == k_HSteamNetConnection_Invalid){
                ATLOG_CRITICAL("Message from unknown source");
            }

            if(pIncomingMsg->m_cbSize){
                if(m_OnMessageReceivedCallback){
                    m_OnMessageReceivedCallback(pIncomingMsg->m_conn, data);
                }
            }
            pIncomingMsg->Release();

        }
    }

    void Server::SteamNetConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t *pInfo) {
        s_Instance->OnConnectionStatusChanged(pInfo);
    }

    void Server::PollConnectionStateChanges() {
        m_Interface->RunCallbacks();
    }

    void Server::OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t *status) {
        // Handle connection state
        switch (status->m_info.m_eState)
        {
            case k_ESteamNetworkingConnectionState_None:
                // NOTE: We will get callbacks here when we destroy connections.  You can ignore these.
                break;

            case k_ESteamNetworkingConnectionState_ClosedByPeer:
            case k_ESteamNetworkingConnectionState_ProblemDetectedLocally:
            {
                // Ignore if they were not previously connected.  (If they disconnected
                // before we accepted the connection.)
                if (status->m_eOldState == k_ESteamNetworkingConnectionState_Connected)
                {
                    auto itClient = m_MapOfClients.find(status->m_hConn);
                    if(m_OnClientConnectedCallback){
                        m_OnClientDisconnectedCallback(status->m_hConn);
                    }
                    m_MapOfClients.erase(itClient);
                }
                else
                {
                    //assert(info->m_eOldState == k_ESteamNetworkingConnectionState_Connecting);
                }

                m_Interface->CloseConnection(status->m_hConn, 0, nullptr, false);
                break;
            }

            case k_ESteamNetworkingConnectionState_Connecting:
            {
                if (m_Interface->AcceptConnection(status->m_hConn) != k_EResultOK)
                {
                    m_Interface->CloseConnection(status->m_hConn, 0, nullptr, false);
                    std::cout << "Couldn't accept connection (it was already closed?)" << std::endl;
                    break;
                }
                if (!m_Interface->SetConnectionPollGroup(status->m_hConn, m_PollGroup))
                {
                    m_Interface->CloseConnection(status->m_hConn, 0, nullptr, false);
                    std::cout << "Failed to set poll group" << std::endl;
                    break;
                }
                SteamNetConnectionInfo_t connectionInfo;
                m_Interface->GetConnectionInfo(status->m_hConn, &connectionInfo);
                auto& client = m_MapOfClients[status->m_hConn];
                client.ID = (ClientID)status->m_hConn;
                client.ConnectionDesc = connectionInfo.m_szConnectionDescription;
                if(m_OnClientConnectedCallback){
                    m_OnClientConnectedCallback(status->m_hConn);
                }


                break;
            }
            case k_ESteamNetworkingConnectionState_Connected:
                break;

            default:
                break;
        }
    }


}