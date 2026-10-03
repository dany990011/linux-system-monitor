#include "CpuMonitor.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <iostream>


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
        std::exit(EXIT_FAILURE);
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