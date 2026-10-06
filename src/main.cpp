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
#include "ProcessMonitor.hpp"
#include "MemoryMonitor.hpp"
#include "Terminal.hpp"
#include "Display.hpp"
#include "SortMode.hpp"
#include "NetworkMonitor.hpp"


int main(){
    
    bool running = true;
    const size_t maxProcesses = 5;
    char sortChoice = 'c', key;
    long logicalCpus = sysconf(_SC_NPROCESSORS_ONLN);
    const double hundred = 100;
    Terminal terminal;
    SortMode sortMode;


    while (running){

        key = terminal.readKey();

        if (key == 'q'){
            running = false;
            break;
        }
        else
        {
            if (key == 'c')
            {
                sortMode = SortMode::Cpu;
            }
            else if (key == 'm')
            {
                sortMode = SortMode::Memory;
            }
        }

        //RAM

        MemoryStats memory = readMemoryStatus();

        std::vector<ProcessInfo> processes;

        //CPU

        CpuStats measurement1, measurement2; 
        long long totalDIff, idleDiff;
        double cpuUsage;

        measurement1 = readCpuStatus();
        processes = readProcesses();
        

        for (auto &p : processes)
        {
            p.cpuTicks = findProcessCpuTicks(p.pid);
        }

        //Network

        NetworkStats network;
        NetworkStats temp = readNetwork();


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

        //Processes

        std::sort(processes.begin(),processes.end(),[sortMode](const ProcessInfo& a,const ProcessInfo& b) { //captures 'sortMode' from outside lambda scope and saves it for use in lambda. 
            if (sortMode == SortMode::Memory){
                return a.memoryKb > b.memoryKb;
            }else{
                return a.cpuPercent > b.cpuPercent;
            }
            
        });

        size_t displayCount = std::min(maxProcesses, processes.size());

        network = readNetwork();
        network.recivedBytes -= temp.recivedBytes;
        network.sentBytes -= temp.sentBytes; 

        //Prints

        printMoinitor(cpuUsage, memory, sortMode, displayCount, processes, network);

    }

    std::cout << "\n-EXITING...\n ";
    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}


