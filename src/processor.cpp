#include <string>
#include <vector>

#include "linux_parser.h"
#include "processor.h"

using std::string;
using std::vector;



//Return the aggregate cpu utilization
float Processor::Utilization() { 
    vector<string> cpu(100);
    cpu = LinuxParser::CpuUtilization();
    float user, nice, system, idle, iowait, irq, softirq, steal, idle_tot, non_idle;
    user = std::stof(cpu[0]);
    nice = std::stof(cpu[1]);
    system = std::stof(cpu[2]);
    idle = std::stof(cpu[3]);
    iowait = std::stof(cpu[4]);
    irq = std::stof(cpu[5]);
    softirq = std::stof(cpu[6]);
    steal = std::stof(cpu[7]);
    //guest = std::stof(cpu[8]);
    //guest_nice = std::stof(cpu[9]);
    idle_tot = idle + iowait;
    non_idle = user + nice + system + irq + softirq + steal;
    return non_idle/(non_idle+idle_tot); 
}