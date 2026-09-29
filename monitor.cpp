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

struct CpuStats
{
    long long total, idle;
};

struct ProcessInfo{
    int pid;
    std::string name;
    long long memoryKb;
};

CpuStats readCpuStatus(){

    std::string line;
    std::string cpuLabel;
    long long user;
    long long nice;
    long long system;
    long long idle;
    long long iowait;
    long long irq;
    long long softirq;
    long long steal;

    long long totalTime, idleTime;

    CpuStats measurement;
    std::ifstream file_stat("/proc/stat");

    if(!file_stat){
        std::cout << "ERROR\n";
        exit;
    }

    std::getline(file_stat, line);
    std::istringstream stat_stream(line);

    stat_stream >> cpuLabel >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;

    totalTime = user + nice + system + idle + iowait + irq + softirq + steal;
    idleTime = idle + iowait;

    measurement.total = totalTime;
    measurement.idle = idleTime;
    return measurement;
}

std::vector<ProcessInfo> readProcesses() {
    std::string dirName;
    std::string line;
    std::string tempString;
    std::vector<ProcessInfo> processes;
    for (const auto& entry : std::filesystem::directory_iterator("/proc")){
        dirName = entry.path().filename().string();
        if (std::all_of(dirName.begin(), dirName.end(), [](char c) {return std::isdigit(static_cast<unsigned char>(c));}) ){
            ProcessInfo tempProcessObj{};
            tempProcessObj.pid = std::stoi(dirName);
            std::ifstream process_file(entry.path().string() + "/status");
            while(std::getline(process_file,line)){
                if(line.find("Name") == 0){
                    std::istringstream stream(line);
                    stream >> tempString >> tempProcessObj.name;
                }else if(line.find("VmRSS") == 0){
                    std::istringstream stream(line);
                    stream >> tempString >> tempProcessObj.memoryKb;
                }
            }
            processes.push_back(tempProcessObj);
        }
    }
    std::sort(processes.begin(),processes.end(),[](const ProcessInfo& a,const ProcessInfo& b) {
        return a.memoryKb > b.memoryKb;
    });

    return processes;
}

int main(){

    double kilo = 1024;
    double hundred = 100;
    int maxProcesses = 5;

    while (true){

        //RAM
        
        std::ifstream file("/proc/meminfo");

        if(!file){
            std::cout << "ERROR\n";
            return -1;    
        }
        std::string line, line2;
        std::string label;
        long long value;
        std::string unit;
        long long totalKb = 0;
        long long availableKb = 0;

        std::vector<ProcessInfo> processes;
        while (std::getline(file, line))
        {
            if(line.find("MemTotal") == 0){
                std::istringstream stream(line);
                stream >> label >> value >> unit;
                totalKb = value;
            }
            if(line.find("MemAvailable") == 0){
                std::istringstream stream(line);
                stream >> label >> value >> unit;
                availableKb = value;
            }
        }

        //CPU

        CpuStats measurement1, measurement2; 
        long long totalDIff, idleDiff;
        double cpuUsage;

        measurement1 = readCpuStatus();

        std::this_thread::sleep_for(std::chrono::seconds(1));

        measurement2 = readCpuStatus();

        totalDIff = measurement2.total - measurement1.total;
        idleDiff = measurement2.idle - measurement1.idle;
        cpuUsage = ((totalDIff - idleDiff)*1.0 / totalDIff) * hundred;

        //Processes

        processes = readProcesses();

        if(processes.size() < maxProcesses){
            maxProcesses = processes.size();
        }

        //Prints

        std::cout << "\033[2J\033[H"; //clear

        std::cout << "System Monitor\n";

        std::cout << std::setw(10) <<  std::left << std::fixed << std::setprecision(2) << "CPU Usage: " << cpuUsage <<  "%\n";

        std::cout << "RAM Usage: " << (totalKb-availableKb)/(kilo*kilo) << " GB / " 
        << totalKb/(kilo*kilo) << " GB (" 
        << ((totalKb-availableKb)*hundred/totalKb) 
        << "%)" << "\n";
        //std::cout << "Precentage: " << ((totalKb-availableKb)*hundred/totalKb) << "%)" << "\n";
        
        std::cout << std::setw(10) << std::left << "\nPID" << std::setw(25) <<  "NAME" << std::setw(10) << "MEMORY (MB)\n";
        for (size_t i = 0; i < maxProcesses; i++)
        {
            std::cout << std::setw(10) << processes[i].pid << std::setw(20) << processes[i].name << std::setw(10) << processes[i].memoryKb/1024.0 <<"MB\n";
        }

    }

    return 0;
}


