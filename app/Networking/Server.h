//
// Created by toor on 11/15/23.
//

#ifndef ATOM_SERVER_H
#define ATOM_SERVER_H



#include "ATOM/atompch.h"


namespace Atom {


    using ClientID = HSteamNetConnection;

    struct ClientInfo
    {
        ClientID ID;
        std::string ConnectionDesc;
    };

    class Server
    {
    public:
        using DataReceivedCallback = std::function<void(const ClientInfo&, const void* data,unsigned  int size)>;
        using ClientConnectedCallback = std::function<void(const ClientInfo&)>;
        using ClientDisconnectedCallback = std::function<void(const ClientInfo&)>;
    public:
        Server(int port);
        ~Server();

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Start and Stop the server
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void Start();
        void Stop();

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Set callbacks for server events
        // These callbacks will be called from the server thread
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        void SetDataReceivedCallback(const DataReceivedCallback& function);
        void SetClientConnectedCallback(const ClientConnectedCallback& function);
        void SetClientDisconnectedCallback(const ClientDisconnectedCallback& function);

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Send Data
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        template<typename T>
        void SendDataToClient(ClientID clientID, const T& data, bool reliable = true) {
            m_Interface->SendMessageToConnection((HSteamNetConnection)clientID, &data, sizeof(T), reliable ? k_nSteamNetworkingSend_Reliable : k_nSteamNetworkingSend_Unreliable, nullptr);
        }

        template<typename T>
        void SendDataToAllClients(const T& data, ClientID excludeClientID, bool reliable = true) {
            for (const auto& [clientID, clientInfo] : m_ConnectedClients) {
                if (clientID != excludeClientID)
                    SendDataToClient(clientID, data, reliable);
            }
        }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        void KickClient(ClientID clientID);

        bool IsRunning() const { return m_Running; }
        const std::map<HSteamNetConnection, ClientInfo>& GetConnectedClients() const { return m_ConnectedClients; }
    private:
        void NetworkThreadFunc(); // Server thread

        static void ConnectionStatusChangedCallback(SteamNetConnectionStatusChangedCallback_t* info);
        void OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* info);

        // Server functionality
        void PollIncomingMessages();
        void SetClientNick(HSteamNetConnection hConn, const char* nick);
        void PollConnectionStateChanges();

        void OnFatalError(const std::string& message);
    private:
        std::thread m_NetworkThread;
        DataReceivedCallback m_DataReceivedCallback;
        ClientConnectedCallback m_ClientConnectedCallback;
        ClientDisconnectedCallback m_ClientDisconnectedCallback;

        int m_Port = 0;
        bool m_Running = false;
        std::map<HSteamNetConnection, ClientInfo> m_ConnectedClients;

        ISteamNetworkingSockets* m_Interface = nullptr;
        HSteamListenSocket m_ListenSocket = 0u;
        HSteamNetPollGroup m_PollGroup = 0u;
    };

}


#endif //ATOM_SERVER_H
