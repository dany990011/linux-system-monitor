#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <iostream>
#include <unistd.h>


#include <serializer.hpp>
#include <Display.hpp>


int main(){
    int socketId;
    bool running = true;
    sockaddr_in serverAddr{}, clientAddr{};
    std::string buffer(10000,'\0'), compleateMessage = "";
    SystemSnapshot snapshot;
    ssize_t received;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    socketId = socket(AF_INET,SOCK_STREAM,0);
    if(socketId == -1){
        perror("socket error");
        return 1;
    }
    int connectResult = connect(socketId, reinterpret_cast<sockaddr*>(&serverAddr),sizeof(serverAddr));
    if (connectResult == -1){
        perror("connection error");
        return 1;
    }
    std::cout << "CONNECTION SUCCSESUL !\n\n";
    
    while (running){
        received = recv(socketId,buffer.data(),buffer.size(),0);
        //std::cout << "recv returned: " << received << "\n";
        if(received == 0){
            std::cout << "server closed connection\n";
            running = false;
            break;
        }else if(received == -1){
            perror("recv error");
            break;
        }
        compleateMessage.append(buffer.data(),buffer.size());
        if(compleateMessage.find("END_SNAPSHOT") != std::string::npos){
            snapshot = deSerializeSnapshot(compleateMessage);
            size_t displayCount = std::min<size_t>(10, snapshot.processes.size());
            printMoinitor(snapshot, SortMode::Memory, displayCount, "Client Server Monitor");
            compleateMessage = "";
            sleep(1);
        }
        
    }
    
}