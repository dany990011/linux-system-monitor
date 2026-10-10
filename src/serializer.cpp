#include <string>
#include <sstream>

#include <SystemSnapshot.hpp>

std::string stringSerializeSnapshot(SystemSnapshot snapshot){
    std::string serializedString = "NONE";
    std::ostringstream out;

    out 
    << "CPU " << snapshot.cpuUsage << "\n" 
    << "RAM_TOTAL " << snapshot.memory.totalKb << "\n"
    << "RAM_AVAILABLE" << snapshot.memory.availableKb << "\n"
    << "DOWNLOAD " << snapshot.network.recivedBytes << "\n"
    << "UPLOAD " << snapshot.network.sentBytes << "\n";

    out << "PROCESSES\n";

    for (ProcessInfo process : snapshot.processes){
        out << process.pid << "|";
        out << process.name << "|";
        out << process.cpuPercent << "|";
        out << process.memoryKb << "\n";
    }

    out << "END_PROCESSES\n";
    out << "END_SNAPSHOT\n";

    return out.str();
}

SystemSnapshot deSerializeSnapshot(const std::string& data){
    std::string line, temp;
    std::istringstream stream(data);
    SystemSnapshot newSnapshot{};

    std::string pid,name,cpuPercent,memoryKb;
    while(std::getline(stream, line)){
        if(line.find("CPU") == 0){
            std::istringstream line_stream(line);
            line_stream >> temp >> newSnapshot.cpuUsage;
        }
        else if(line.find("RAM_TOTAL") == 0){
            std::istringstream line_stream(line);
            line_stream >> temp >> newSnapshot.memory.totalKb;
        }
        else if(line.find("RAM_AVAILABLE") == 0){
            std::istringstream line_stream(line);
            line_stream >> temp >> newSnapshot.memory.availableKb;
        }
        else if(line.find("DOWNLOAD") == 0){
            std::istringstream line_stream(line);
            line_stream >> temp >> newSnapshot.network.recivedBytes;
        }
        else if(line.find("UPLOAD") == 0){
            std::istringstream line_stream(line);
            line_stream >> temp >> newSnapshot.network.sentBytes;
        }
        else if(line.find("PROCESSES") == 0){
            while (std::getline(stream, line)){
                if(line == "END_PROCESSES"){
                    break;
                }
                ProcessInfo newProcess{};
                std::istringstream processes_stream(line);
                std::getline(processes_stream,pid,'|');
                std::getline(processes_stream,name,'|');
                std::getline(processes_stream,cpuPercent,'|');
                std::getline(processes_stream,memoryKb,'|');

                newProcess.pid = std::stoi(pid);
                newProcess.name = name;
                newProcess.cpuPercent = std::stod(cpuPercent);
                newProcess.memoryKb = std::stoll(memoryKb);

                newSnapshot.processes.push_back(newProcess);
            }
        }else if(line.find("END_SNAPSHOT") == 0){
            break;
        }
    }
    return newSnapshot;
}