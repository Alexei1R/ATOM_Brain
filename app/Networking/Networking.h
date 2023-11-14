//
// Created by toor on 11/14/23.
//

#ifndef ATOM_NETWORKING_H
#define ATOM_NETWORKING_H


#include "ATOM/atompch.h"



namespace Atom{

    struct NetSpecs{
        std::string IP;
        unsigned int Port;
        NetSpecs(const std::string& ip = "localhost", unsigned int port = 12345)
                :IP(ip), Port(port) {}
    };

    using ClientID = HSteamNetConnection;

    struct ClientInfo
    {
        ClientID ID;
        std::string ConnectionDesc;
    };
    struct ClientName
    {
        std::string m_Name;
    };

    class Server {
    public:
        explicit Server(NetSpecs specs = NetSpecs());
        ~Server();

        [[noreturn]] void ServerStart();
        void ServerStop();


        template<typename T>
        void SendDataToClient(ClientID clientID, const T& data, bool reliable = true);

        template<typename T>
        void SendDataToAllClient( const T& data,HSteamNetConnection except = k_HSteamNetConnection_Invalid,  bool reliable = true);


        void AttachOnMessageReceivedCallback(std::function<void(ClientID,const void* data)> callback) {m_OnMessageReceivedCallback = callback;};

        void AttachOnClientConnectedCallback(std::function<void(ClientID)> callback) {m_OnClientConnectedCallback = callback;}
        void AttachOnClientDisconnectedCallback(std::function<void(ClientID)> callback) {m_OnClientDisconnectedCallback = callback;}

    private:
        static void SteamNetConnectionStatusChangedCallback( SteamNetConnectionStatusChangedCallback_t *pInfo );
        void OnConnectionStatusChanged(SteamNetConnectionStatusChangedCallback_t* status);
        void PollIncomingMessages();
        void PollConnectionStateChanges();
    private:
        NetSpecs m_Specs;
        std::function<void(ClientID)> m_OnClientConnectedCallback;
        std::function<void(ClientID)> m_OnClientDisconnectedCallback;
        std::function<void(ClientID,const void* data)> m_OnMessageReceivedCallback;


        HSteamListenSocket m_ListenSocket;
        HSteamNetPollGroup m_PollGroup;
        ISteamNetworkingSockets* m_Interface = nullptr;
        SteamNetworkingIPAddr m_ServerLocalAddr;
        SteamNetworkingConfigValue_t m_Opt;
        std::map< HSteamNetConnection,ClientInfo> m_MapOfClients;


        std::thread m_NetworkThread;
        bool m_Running = false;

    };

}

#endif //ATOM_NETWORKING_H
