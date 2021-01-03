#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;
#define CLK_TCK 100

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

//Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string NotOfInterestText, Memtotal, Memfree;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> NotOfInterestText >> Memtotal >> NotOfInterestText;
  }//JHÄ läs på mer om istringstream, det borde gå att göra koden bättre här
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> NotOfInterestText >> Memfree >> NotOfInterestText;
  }
  return ((std::stof(Memtotal)-std::stof(Memfree))/std::stof(Memtotal));
}

//Read and return the system uptime
long LinuxParser::UpTime() {
  string Uptime = "-1"; //Indication of something is wrong
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> Uptime;
  }
  return stof(Uptime);
}

//Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return UpTime() * CLK_TCK; }

//Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  string line, temp;
  vector<string> values;
  std::ifstream stream(kProcDirectory + std::to_string(pid)+kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> temp)
      values.push_back(temp);
  }
  return (stol(values[13]) + stol(values[14]) + stol(values[15]) + stol(values[16]));
}

//Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<string> jiffies = CpuUtilization();
  return stol(jiffies[CPUStates::kUser_]) + stol(jiffies[CPUStates::kNice_]) +
         stol(jiffies[CPUStates::kSystem_]) + stol(jiffies[CPUStates::kIRQ_]) +
         stol(jiffies[CPUStates::kSoftIRQ_]) + stol(jiffies[CPUStates::kSteal_]);
} 

//Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> jiffies = CpuUtilization();
  return stol(jiffies[CPUStates::kIdle_]) + stol(jiffies[CPUStates::kIOwait_]);
} 

//Read and return cpuload utilization
vector<string> LinuxParser::CpuUtilization() {
  vector<string> cpuload;
  string line, tempstring, cpu;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu;
    while (linestream >> tempstring)
      cpuload.push_back(tempstring);
  }
  return cpuload; 
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {  
  string Tempstring, NumberOfProcesses = "-1"; // -1 = something is wrong
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  while (std::getline(stream, line)){
    if (line.find("processes")!=std::string::npos){
      std::istringstream linestream(line);
      linestream >> Tempstring >> NumberOfProcesses;
    }
  }
  return std::stoi(NumberOfProcesses);
}

//Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string Tempstring, NumberOfRunningProcesses = "-1"; // -1 = something is wrong
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  while (std::getline(stream, line)){
    if (line.find("procs_running")!=std::string::npos){
      std::istringstream linestream(line);
      linestream >> Tempstring >> NumberOfRunningProcesses;
    }
  }
  return std::stoi(NumberOfRunningProcesses);
}

//Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line, temp, command;
  vector<string> values;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> temp)
      values.push_back(temp);
  }
  for (auto it = values.begin(); it != values.end(); ++it) {
    if (it == values.begin())
      command = *it;
    else
      command = command + " " + *it;
  }
  return command;
}

//Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line, temp, vmsize;
  int size;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  while (std::getline(stream, line)){
    std::istringstream linestream(line);
    linestream >> temp >> vmsize;
    if (temp == "VmSize:"){
      size = stoi(vmsize);
      size = size / 1000; //Conversion from KB to MB
      vmsize = to_string(size);
      return vmsize;
    } 
  }
  return "-1"; //something is wrong
}

//Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line, uid, uidno = "-1";
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  while (std::getline(stream, line)){
    std::istringstream linestream(line);
    linestream >> uid >> uidno;
    if (uid == "Uid:"){
      return uidno;
    } 
  }
  return uidno;
}

//Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string line = "  ";
  string uid = Uid(pid);
  std::ifstream stream(kPasswordPath);
  while (std::getline(stream, line)){
    std::size_t found =line.find(uid);
    if (found !=std::string::npos){
      line.erase(found-3,100);
      return line;
    }
  }
  return line;
}

//Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string line, temp ; 
  vector<string> values;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (stream.is_open()){
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> temp)
      values.push_back(temp);
  }
  return LinuxParser::UpTime() - (stol(values[21]) / CLK_TCK);
 }

 