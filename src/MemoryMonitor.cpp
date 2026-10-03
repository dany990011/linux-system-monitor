#include "MemoryMonitor.hpp"
#include <fstream>
#include <iostream>
#include <filesystem>


MemoryStats readMemoryStatus(){

    std::string line;
    std::string label;
    long long value;
    std::string unit;
    MemoryStats newMemoryStats;

    std::ifstream file("/proc/meminfo");

    newMemoryStats.availableKb = -1;
    newMemoryStats.availableKb = -1;

    if(!file){
        std::cout << "ERROR\n";
        return newMemoryStats;    
    }

    while (std::getline(file, line))
    {
        if(line.find("MemTotal") == 0){
            std::istringstream stream(line);
            stream >> label >> value >> unit;
            newMemoryStats.totalKb = value;
        }
        if(line.find("MemAvailable") == 0){
            std::istringstream stream(line);
            stream >> label >> value >> unit;
            newMemoryStats.availableKb = value;
        }
    }

    return newMemoryStats;

}