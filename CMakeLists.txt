cmake_minimum_required(VERSION 3.16)

project(SystemMonitor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(monitor
    src/main.cpp
    src/CpuMonitor.cpp
    src/ProcessMonitor.cpp
    src/MemoryMonitor.cpp
    src/Terminal.cpp
    src/Display.cpp
    src/NetworkMonitor.cpp
    src/TcpServer.cpp
    src/serializer.cpp
)

add_executable(client_monitor
    src/client.cpp
    src/serializer.cpp
)

target_include_directories(monitor PRIVATE include)
target_include_directories(client_monitor PRIVATE include)