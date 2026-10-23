#pragma once 

#include <atomic>

#include "SystemSnapshot.hpp"

class TcpServer{
    private:
        int listeningSocket = -1;
        std::atomic<bool> running = true;
    public:
    bool startServer();
    void runServer(SystemSnapshot& snapshot, std::mutex& snapshotMutex);
    void handleClient(int clientId, SystemSnapshot& snapshot, std::mutex& snapshotMutex);
    void closeServer();
    std::string getPort();
};
