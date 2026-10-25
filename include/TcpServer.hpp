#pragma once 

#include <atomic>
#include <thread>
#include <poll.h>

#include "SystemSnapshot.hpp"

class TcpServer{
    private:
        int listeningSocket = -1;
        size_t connectedClientsNum = 0;
        std::atomic<bool> running = true;
        std::vector<std::thread> clientThreads;
        std::vector<int> clientSockets;
        std::vector<pollfd> sockets;
    public:
    bool startServer();
    void runServer(SystemSnapshot& snapshot, std::mutex& snapshotMutex);
    void handleClient(int clientId, SystemSnapshot& snapshot, std::mutex& snapshotMutex);
    void closeServer();
    std::string getPort();
    int getConnectedClients();
};
