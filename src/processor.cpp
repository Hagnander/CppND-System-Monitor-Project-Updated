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
    
    user_ = std::stof(cpu[0]);
    nice_ = std::stof(cpu[1]);
    system_ = std::stof(cpu[2]);
    idle_ = std::stof(cpu[3]);
    iowait_ = std::stof(cpu[4]);
    irq_ = std::stof(cpu[5]);
    softirq_ = std::stof(cpu[6]);
    steal_ = std::stof(cpu[7]);
    //guest = std::stof(cpu[8]);
    //guest_nice = std::stof(cpu[9]);
    idle_tot_ = idle_ + iowait_;
    non_idle_ = user_ + nice_ + system_ + irq_ + softirq_ + steal_;
    return non_idle_/(non_idle_ + idle_tot_); 
}