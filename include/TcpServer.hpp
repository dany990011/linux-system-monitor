#pragma once 
#include "SystemSnapshot.hpp"

void runServer(SystemSnapshot& snapshot, std::mutex& snapshotMutex);