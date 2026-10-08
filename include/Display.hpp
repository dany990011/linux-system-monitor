#pragma once

#include "MemoryMonitor.hpp"
#include "SortMode.hpp"
#include "ProcessMonitor.hpp"
#include "NetworkMonitor.hpp"
#include "SystemSnapshot.hpp"
#include <vector>
#include <cstddef>

void printMoinitor(SystemSnapshot snapshot, SortMode sortMode, size_t displayCount);