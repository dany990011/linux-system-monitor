#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <mutex>

#include <TcpServer.hpp>
#include <SystemSnapshot.hpp>
#include <serializer.hpp>
#include <unistd.h>
#include <thread>

bool TcpServer::startServer()
{
    int socketId;
    sockaddr_in serverAddr{};

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if((socketId = socket(AF_INET,SOCK_STREAM,0)) == -1){
        perror("socket");
        return false;
    }
    this->listeningSocket = socketId;

    if(bind(socketId, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1){
        perror("bind");
        return false;
    }
    if(listen(socketId,4) == -1){
        perror("listen");
        return false;
    }
    return true;
}

void TcpServer::runServer(SystemSnapshot &snapshot, std::mutex &snapshotMutex)
{
    int clientId;
    sockaddr_in clientAddr{};
    socklen_t clientAdrrSize;
    while (running){
        clientAdrrSize = sizeof(clientAddr);
        if((clientId = accept(listeningSocket, reinterpret_cast<sockaddr*>(&clientAddr),&clientAdrrSize)) == -1){
        }else{
            std::thread clientThread(&TcpServer::handleClient, this, clientId, std::ref(snapshot), std::ref(snapshotMutex));
            clientThread.detach();
        }
    }
}

void TcpServer::handleClient(int clientId, SystemSnapshot& snapshot, std::mutex& snapshotMutex){
    ssize_t sendReturnMsg;
    std::string serverMessage = "message from server\n", serverSnapshot;
    sendReturnMsg = send(clientId, serverMessage.data(), serverMessage.size(), 0);

    while(sendReturnMsg > 0){
        SystemSnapshot localSnapCopy;
        {
            std::lock_guard<std::mutex> lock(snapshotMutex);
            localSnapCopy = snapshot;
        }       
        serverSnapshot = stringSerializeSnapshot(localSnapCopy);
        sendReturnMsg = send(clientId, serverSnapshot.data(), serverSnapshot.size(), 0);

        sleep(1);
    }
    close(clientId);

}


void TcpServer::closeServer(){
    running = false;
    shutdown(listeningSocket, SHUT_RDWR);
    close(this->listeningSocket);
    listeningSocket = -1;
    return;
}

std::string TcpServer::getPort()
{
    return std::string();
}
