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
    int socketId, option = 1;
    sockaddr_in serverAddr{};

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
 
    if((socketId = socket(AF_INET,SOCK_STREAM,0)) == -1){
        perror("socket");
        return false;
    }
    this->listeningSocket = socketId;

    setsockopt(socketId,SOL_SOCKET,SO_REUSEADDR,&option,sizeof(option));

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
            clientSockets.push_back(clientId);
            clientThreads.emplace_back(
                &TcpServer::handleClient, this, clientId, std::ref(snapshot), std::ref(snapshotMutex)
            );
            connectedClientsNum += 1;
            //clientThread.detach();
        }
    }
}

void TcpServer::handleClient(int clientId, SystemSnapshot& snapshot, std::mutex& snapshotMutex){
    ssize_t sendReturnMsg;
    std::string serverMessage = "message from server\n", serverSnapshot;
    sendReturnMsg = send(clientId, serverMessage.data(), serverMessage.size(), 0);

    while(sendReturnMsg > 0 && running){
        SystemSnapshot localSnapCopy;
        {
            std::lock_guard<std::mutex> lock(snapshotMutex);
            localSnapCopy = snapshot;
        }       
        serverSnapshot = stringSerializeSnapshot(localSnapCopy);
        sendReturnMsg = send(clientId, serverSnapshot.data(), serverSnapshot.size(), MSG_NOSIGNAL);

        sleep(1);
    }
    close(clientId);
    connectedClientsNum -= 1;
}


void TcpServer::closeServer(){
    running = false;
    shutdown(listeningSocket, SHUT_RDWR);

    for (int clientId : clientSockets)
    {
        shutdown(clientId,SHUT_RDWR);
    }
    

    close(this->listeningSocket);
    listeningSocket = -1;
    for (std::thread& thread : clientThreads)
    {
        if (thread.joinable()){
            thread.join();
        }
    }
    
    return;
}

std::string TcpServer::getPort()
{
    return std::string();
}

int TcpServer::getConnectedClients(){
    return connectedClientsNum;
}
