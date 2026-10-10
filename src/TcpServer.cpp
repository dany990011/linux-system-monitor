#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <mutex>

#include <TcpServer.hpp>
#include <SystemSnapshot.hpp>
#include <serializer.hpp>
#include <unistd.h>

void runServer(SystemSnapshot& snapshot, std::mutex& snapshotMutex){

    SystemSnapshot localSnapCopy;

    {
        std::lock_guard<std::mutex> lock(snapshotMutex);
        localSnapCopy = snapshot;
    }

    int socketId, clientId;
    sockaddr_in serverAddr{}, clientAddr{};
    socklen_t clientAdrrSize = sizeof(clientAddr);
    std::string serverMessage = "message from server\n";
    std::string serverSnapshot;
    ssize_t sendReturnMsg;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if((socketId = socket(AF_INET,SOCK_STREAM,0)) == -1){
        //return -1;
    }
    if(bind(socketId, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1){
        //return -1;
    }
    if(listen(socketId,4) == -1){
        //return -1;
    }
    while (true){
        clientAdrrSize = sizeof(clientAddr);
        if((clientId = accept(socketId,reinterpret_cast<sockaddr*>(&clientAddr),&clientAdrrSize)) == -1){
        //return -1;
        }else{
            sendReturnMsg = send(clientId, serverMessage.data(), serverMessage.size(), 0);

            while(sendReturnMsg){
                SystemSnapshot localSnapCopy;
                {
                    std::lock_guard<std::mutex> lock(snapshotMutex);
                    localSnapCopy = snapshot;
                }       
                serverSnapshot = stringSerializeSnapshot(localSnapCopy);
                sendReturnMsg = send(clientId, serverSnapshot.data(), serverSnapshot.size(), 0);

                sleep(1);
            }
        }
    }
    

}