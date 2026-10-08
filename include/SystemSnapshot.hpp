#pragma once 

#include <vector>

#include "MemoryMonitor.hpp"
#include "ProcessMonitor.hpp"
#include "NetworkMonitor.hpp"

struct SystemSnapshot{
    double cpuUsage;
    MemoryStats memory;
    std::vector<ProcessInfo> processes;
    NetworkStats network;
};