#include "Display.hpp"

#include <iostream>
#include <iomanip>

const double kilo = 1024;
const double hundred = 100;

void printMoinitor(SystemSnapshot snapshot, SortMode sortMode, size_t displayCount){
    std::cout << "\033[2J\033[H"; //clear

    std::cout << "System Monitor\n\n";

    std::cout << std::setw(10) <<  std::left << std::fixed << std::setprecision(2) << "CPU Usage: " << snapshot.cpuUsage <<  "%\n";

    std::cout << std::setw(10) << "RAM Usage: " << (snapshot.memory.totalKb-snapshot.memory.availableKb)/(kilo*kilo) << " GB / " 
    << snapshot.memory.totalKb/(kilo*kilo) << " GB (" 
    << ((snapshot.memory.totalKb-snapshot.memory.availableKb)*hundred/snapshot.memory.totalKb) 
    << "%)" << "\n\n";
    //std::cout << "Precentage: " << ((totalKb-availableKb)*hundred/totalKb) << "%)" << "\n";

    std::cout << "Netwrok: \n" << std::setw(15) << "Download: " << snapshot.network.recivedBytes <<" B/s\n"<< std::setw(15)<< "Upload: " << snapshot.network.sentBytes << " B/s\n\n\n";

    std::cout << "---------------------------\n"<< "Sorted by - " << [sortMode](){if (sortMode == SortMode::Memory)return "Memory"; else return "CPU";}() << "\n";  //no need for lamda here ,its jsut for fun
    
    std::cout << "\n" << std::setw(10) << std::left << "PID" << std::setw(20) <<  "NAME" << std::setw(14) << "MEMORY (MB)" <<  std::setw(10) << "CPU (%)" << "\n";
    for (size_t i = 0; i < displayCount; i++)
    {
        std::cout << std::left << std::setw(10) << snapshot.processes[i].pid << std::setw(20) << snapshot.processes[i].name << std::setw(14) << snapshot.processes[i].memoryKb/1024.0  << std::setw(10); 
        if(snapshot.processes[i].cpuPercent < 0){
            std::cout << "DEAD";
        }else{
            std::cout << snapshot.processes[i].cpuPercent;
        } 
        std::cout << "\n";
    }

    std::cout << "\n\nsort by CPU (c) or memory (m)" << std::flush;
}