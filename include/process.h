#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp
  void Pid(int pid);
  void User (std::string user);
  void Command (std::string command);
  void CpuUtilization(float cpu_util);
  void Ram (std::string ram);
  void UpTime (long int up_time);

  // TODO: Declare any necessary private members
 private:
 int pid_;
 std::string user_;
 std::string command_;
 float cpu_util_;
 std::string ram_;
 long int up_time_;
};

#endif