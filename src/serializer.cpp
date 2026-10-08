#include <string>
#include <sstream>

#include <SystemSnapshot.hpp>

std::string stringSerializeSnapshot(SystemSnapshot snapshot){
    std::string serializedString = "NONE";
    std::ostringstream out;

    out 
    << "CPU=" << snapshot.cpuUsage << "\n" 
    << "RAM=" << snapshot.memory.totalKb - snapshot.memory.availableKb << "\n"
    << "DOWNLOAD=" << snapshot.network.recivedBytes << "\n"
    << "UPLOAD=" << snapshot.network.sentBytes << "\n";

    out << "PROCESSES\n";

    for (ProcessInfo process : snapshot.processes){
        out << process.pid << "|";
        out << process.name << "|";
        out << process.cpuPercent << "|";
        out << process.memoryKb << "\n";
    }

    out << "END\n";

    return out.str();
}