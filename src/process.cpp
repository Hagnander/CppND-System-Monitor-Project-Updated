#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return cpu_util_; }

// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { return ram_; }

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return up_time_; }

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { return cpu_util_ > a.cpu_util_; }

void Process::Pid(int pid) {pid_ = pid;}

void Process::User (std::string user) {user_ = user;}
void Process::Command (std::string command){command_ = command;}
void Process::CpuUtilization(float cpu_util) {cpu_util_ = cpu_util;}
void Process::Ram (std::string ram){ram_ = ram;}
void Process::UpTime (long int up_time){up_time_ = up_time;}