#include "Display.hpp"

#include <iostream>
#include <iomanip>

const double kilo = 1024;
const double hundred = 100;

void printMoinitor(double cpuUsage, MemoryStats memory, SortMode sortMode, size_t displayCount, std::vector<ProcessInfo> processes, NetworkStats network){
    std::cout << "\033[2J\033[H"; //clear

    std::cout << "System Monitor\n\n";

    std::cout << std::setw(10) <<  std::left << std::fixed << std::setprecision(2) << "CPU Usage: " << cpuUsage <<  "%\n";

    std::cout << "RAM Usage: " << (memory.totalKb-memory.availableKb)/(kilo*kilo) << " GB / " 
    << memory.totalKb/(kilo*kilo) << " GB (" 
    << ((memory.totalKb-memory.availableKb)*hundred/memory.totalKb) 
    << "%)" << "\n";
    //std::cout << "Precentage: " << ((totalKb-availableKb)*hundred/totalKb) << "%)" << "\n";

    std::cout << "Netwrok: \n" << "Download: " << network.recivedBytes << "\nUpload: " << network.sentBytes << "\n";

    std::cout << "Sorted by - " << [sortMode](){if (sortMode == SortMode::Memory)return "Memory"; else return "CPU";}() << "\n\n";  //no need for lamda here ,its jsut for fun
    
    std::cout << "\n" << std::setw(10) << std::left << "PID" << std::setw(20) <<  "NAME" << std::setw(14) << "MEMORY (MB)" <<  std::setw(10) << "CPU (%)" << "\n";
    for (size_t i = 0; i < displayCount; i++)
    {
        std::cout << std::left << std::setw(10) << processes[i].pid << std::setw(20) << processes[i].name << std::setw(14) << processes[i].memoryKb/1024.0  << std::setw(10); 
        if(processes[i].cpuPercent < 0){
            std::cout << "DEAD";
        }else{
            std::cout << processes[i].cpuPercent;
        } 
        std::cout << "\n";
    }

    std::cout << "\n\nsort by CPU (c) or memory (m)" << std::flush;
}