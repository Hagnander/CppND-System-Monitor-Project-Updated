#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    vector<int> pid = LinuxParser::Pids();
    processes_.resize(pid.size());
    int i = 0;
    for (auto it = begin (pid); it != end (pid); ++it) {
        processes_[i].Pid(*it);
        processes_[i].User(LinuxParser::User(*it));
        processes_[i].Ram(LinuxParser::Ram(*it));
        processes_[i].CpuUtilization(static_cast<float>(LinuxParser::ActiveJiffies(*it))/LinuxParser::Jiffies());
        processes_[i].UpTime(LinuxParser::UpTime(*it));
        processes_[i].Command(LinuxParser::Command(*it));
        i++;
    }
    std::sort(processes_.begin(), processes_.end());

    return processes_;
}

//Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

//Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

//Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

//Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

//Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

//Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
