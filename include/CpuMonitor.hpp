#pragma once

struct CpuStats
{
    long long total, idle;
};

CpuStats readCpuStatus();