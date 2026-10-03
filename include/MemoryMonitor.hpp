#pragma once

struct MemoryStats
{
    long long totalKb = 0;
    long long availableKb = 0;
};

MemoryStats readMemoryStatus();