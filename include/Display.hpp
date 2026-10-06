#pragma once

#include "MemoryMonitor.hpp"
#include "SortMode.hpp"
#include "ProcessMonitor.hpp"
#include "NetworkMonitor.hpp"
#include <vector>
#include <cstddef>

void printMoinitor(double cpuUsage, MemoryStats memory, SortMode sortMode, size_t displayCount, std::vector<ProcessInfo> processes, NetworkStats network);