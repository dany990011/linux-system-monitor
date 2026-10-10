#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <iostream>
#include <unistd.h>


#include <serializer.hpp>


int main(){
    int socketId;
    sockaddr_in serverAddr{}, clientAddr{};
    std::string buffer(10000,'\0');


    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);


    socketId = socket(AF_INET,SOCK_STREAM,0);
    bind(socketId, reinterpret_cast<sockaddr*>(&clientAddr), sizeof(clientAddr));
    if(connect(socketId, reinterpret_cast<sockaddr*>(&serverAddr),sizeof(serverAddr)) != -1){
        std::cout << "CONNECTION SUCCSESUL !\n\n";
    }
    while (recv(socketId,buffer.data(),buffer.size(),0) != -1){
        std::cout << buffer;
        sleep(1);
    }

    
}