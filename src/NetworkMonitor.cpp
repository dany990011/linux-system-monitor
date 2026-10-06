#include "NetworkMonitor.hpp"

#include <fstream>
#include <sstream>
#include <iostream>


NetworkStats readNetwork(){

    std::string line, interfaceName;
    std::ifstream file("/proc/net/dev");
    NetworkStats newNetworkStats;
    newNetworkStats.recivedBytes = 0;
    newNetworkStats.sentBytes = 0;

    while (std::getline(file, line)){

        int i = 1;
        if(line.find("eth") != std::string::npos){
            std::istringstream stream(line);
            std::getline(stream, interfaceName, ':');
            long long  value;
            while(stream >> value){
                if (i == 1){
                    newNetworkStats.recivedBytes = value;
                }else if (i == 9){
                    newNetworkStats.sentBytes = value;
                }
                i++;
            }
        }
    }
    return newNetworkStats;
}