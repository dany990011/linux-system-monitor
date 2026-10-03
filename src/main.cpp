#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>

#include "CpuMonitor.hpp"
#include "PorcessMonitor.hpp"
#include "MemoryMonitor.hpp"


int main(){
    
    bool running = true;
    static double kilo = 1024;
    static double hundred = 100;
    int maxProcesses = 5;
    char sortChoice = 'c', key;
    long logicalCpus = sysconf(_SC_NPROCESSORS_ONLN);

    termios oldSettings;
    termios newSettings;

    tcgetattr(STDIN_FILENO, &oldSettings);

    newSettings = oldSettings;
    newSettings.c_lflag &= ~(ICANON | ECHO);
    newSettings.c_cc[VMIN] = 0;
    newSettings.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);

    while (running){

        //RAM

        MemoryStats memory = readMemoryStatus();

        std::vector<ProcessInfo> processes;

        if(read(STDIN_FILENO, &key, 1) > 0 ){
            if (key == 'q'){
                running = false;
                break;
            }else{
                if (key == 'm' || key == 'c'){
                    sortChoice = key;
                }
                
            }
        }


        //CPU

        CpuStats measurement1, measurement2; 
        long long totalDIff, idleDiff;
        double cpuUsage;

        measurement1 = readCpuStatus();
        processes = readProcesses();

        for (auto &p : processes)
        {
            p.cpuTicks = findProcessCpuTicks(p.pid);
            //std::cout << "TEST BEFORE : " << p.cpuTicks << "\n";
        }


        std::this_thread::sleep_for(std::chrono::seconds(1));


        measurement2 = readCpuStatus();
        totalDIff = measurement2.total - measurement1.total;
        idleDiff = measurement2.idle - measurement1.idle;
        cpuUsage = ((totalDIff - idleDiff)*1.0) / totalDIff * hundred;

        for (auto &p : processes)
        {
            p.cpuTicks = findProcessCpuTicks(p.pid) - p.cpuTicks;
            p.cpuPercent = (p.cpuTicks*1.0 / totalDIff) * hundred * logicalCpus; //16 logical cores usually
        }


        std::sort(processes.begin(),processes.end(),[sortChoice](const ProcessInfo& a,const ProcessInfo& b) { //captures 'sortChoise' from outside lambda scope and saves it for use in lambda. 
            if (sortChoice == 'm'){
                return a.memoryKb > b.memoryKb;
            }else{
                return a.cpuPercent > b.cpuPercent;
            }
            
        });

        //Processes

        if(processes.size() < maxProcesses){
            maxProcesses = processes.size();
        }

        //Prints

        std::cout << "\033[2J\033[H"; //clear

        std::cout << "System Monitor\n\n";

        std::cout << std::setw(10) <<  std::left << std::fixed << std::setprecision(2) << "CPU Usage: " << cpuUsage <<  "%\n";

        std::cout << "RAM Usage: " << (memory.totalKb-memory.availableKb)/(kilo*kilo) << " GB / " 
        << memory.totalKb/(kilo*kilo) << " GB (" 
        << ((memory.totalKb-memory.availableKb)*hundred/memory.totalKb) 
        << "%)" << "\n";
        //std::cout << "Precentage: " << ((totalKb-availableKb)*hundred/totalKb) << "%)" << "\n";

        std::cout << "Sorted by - " << [sortChoice](){if (sortChoice == 'm')return "Memory"; else return "CPU";}() << "\n\n";  //no need for lamda here ,its jsut for fun
        
        std::cout << "\n" << std::setw(10) << std::left << "PID" << std::setw(20) <<  "NAME" << std::setw(14) << "MEMORY (MB)" <<  std::setw(10) << "CPU (%)" << "\n";
        for (size_t i = 0; i < maxProcesses; i++)
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

    tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);
    std::cout << "EXITING...\n ";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}


