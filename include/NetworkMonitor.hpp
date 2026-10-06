#pragma once

struct NetworkStats
{
    long long recivedBytes, sentBytes;
};


NetworkStats readNetwork();