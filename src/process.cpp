#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;
using std::stod;

Process::Process(int pid): pid_(pid) {}

// DONE: Return this process's ID
int Process::Pid() const { 
  return pid_; 
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
  long processActiveJiffies = LinuxParser::ActiveJiffies(Process::Pid());
  long seconds = Process::UpTime();
  double activeInSeconds = (double) processActiveJiffies / (double) sysconf(_SC_CLK_TCK);
  return activeInSeconds / (double) seconds; 
}

// DONE: Return the command that generated this process
string Process::Command() { 
  if (command_.empty()) {
    command_ = LinuxParser::Command(Process::Pid());
  }
  return command_;
}

// DONE: Return this process's memory utilization
string Process::Ram() const { 
  return LinuxParser::Ram(Process::Pid());
}

// DONE: Return the user (name) that generated this process
string Process::User() { 
  if (userName_.empty()) {
    userName_ = LinuxParser::User(Process::Pid());
  }
  return userName_;
}

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { 
  return LinuxParser::UpTime() - LinuxParser::UpTime(Process::Pid());
}

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
  string thisRamString = this->Ram();
  string otherRamString = a.Ram();
  if (!thisRamString.empty() && !otherRamString.empty()) {
    return stod(thisRamString) < stod(otherRamString);
  }
  return true; 
}