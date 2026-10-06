#include "ProcessMonitor.hpp"

#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>
#include <cctype>

std::vector<ProcessInfo> readProcesses() {
    std::string dirName;
    std::string line;
    std::string tempString;
    std::vector<ProcessInfo> processes;
    for (const auto& entry : std::filesystem::directory_iterator("/proc")){
        dirName = entry.path().filename().string();
        if (std::all_of(dirName.begin(), dirName.end(), [](char c) {return std::isdigit(static_cast<unsigned char>(c));}) ){
            ProcessInfo tempProcessObj{};
            tempProcessObj.pid = std::stoi(dirName);
            std::ifstream process_file(entry.path().string() + "/status");
            while(std::getline(process_file,line)){
                if(line.find("Name") == 0){
                    std::istringstream stream(line);
                    stream >> tempString >> tempProcessObj.name;
                }else if(line.find("VmRSS") == 0){
                    std::istringstream stream(line);
                    stream >> tempString >> tempProcessObj.memoryKb;
                }
            }
            processes.push_back(tempProcessObj);
        }
    }

    return processes;
}


long long findProcessCpuTicks(int processId){
    std::string subString, valueString;
    long long totalTicks = 0;
    int pos = 2;
    int desiredPos1 = 14, desiredPos2 = 15;
    std::ifstream processStat("/proc/" + std::to_string(processId) + "/stat");
    if (!processStat.is_open()){
        return -1;
    }
    while(std::getline(processStat, subString, ')')){

    }
    std::istringstream stream(subString);
    while(std::getline(stream, valueString, ' ')){
        if(pos == desiredPos1){
            totalTicks += std::stoll(valueString);
        }else if(pos == desiredPos2){
            totalTicks += std::stoll(valueString);
            break;
        }
        pos++;
    }
    return totalTicks;
    //std::getline(processStat, processStatString);
    //subString = processStatString.substr(processStatString.find(")")+1, processStatString.length()-1);
    //while(std::getline(subString, valueString, ' '){
    //    
    //})
}
