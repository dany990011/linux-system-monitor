#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <iostream>
#include <unistd.h>


#include <serializer.hpp>
#include <Display.hpp>


int main(){
    int socketId;
    sockaddr_in serverAddr{}, clientAddr{};
    std::string buffer(10000,'\0'), compleateMessage = "";
    SystemSnapshot snapshot;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    socketId = socket(AF_INET,SOCK_STREAM,0);
    bind(socketId, reinterpret_cast<sockaddr*>(&clientAddr), sizeof(clientAddr));
    if(connect(socketId, reinterpret_cast<sockaddr*>(&serverAddr),sizeof(serverAddr)) != -1){
        std::cout << "CONNECTION SUCCSESUL !\n\n";
    }
    while (recv(socketId,buffer.data(),buffer.size(),0) > 0){
        compleateMessage.append(buffer.data(),buffer.size());
        if(compleateMessage.find("END_SNAPSHOT") != std::string::npos){
            snapshot = deSerializeSnapshot(compleateMessage);
            size_t displayCount = std::min<size_t>(10, snapshot.processes.size());
            printMoinitor(snapshot, SortMode::Memory, displayCount);
            compleateMessage = "";
            sleep(1);
        }
        
    }
    
}