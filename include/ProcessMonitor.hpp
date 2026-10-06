#pragma once

#include <string>
#include <vector>

struct ProcessInfo{
    int pid;
    std::string name;
    long long memoryKb;
    long long cpuTicks = 0;
    double cpuPercent = 0.0;
};

long long findProcessCpuTicks(int processId);

std::vector<ProcessInfo> readProcesses();