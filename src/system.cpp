#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
using std::sort;

// DONE: Return the system's CPU
Processor& System::Cpu() { 
    if (!processorIsCreated_) {
        cpu_ = Processor();
        processorIsCreated_ = true;
    }
    return cpu_; 
}

bool System::compareProcesses(Process& a, Process& b) {
    return b < a;
}

// DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    vector<int> processIdsList = LinuxParser::Pids();
    processes_ = {};
    for (auto pId: processIdsList) {
        processes_.push_back(Process(pId));
    }
    // sort the process by RAM usage
    sort(processes_.begin(), processes_.end(), System::compareProcesses);
    return processes_; 
}

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { 
    if (kernelId_.empty()) {
        kernelId_ = LinuxParser::Kernel();
    }
    return kernelId_;
}

// DONE: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// DONE: Return the operating system name
std::string System::OperatingSystem() {
  if (osName_.empty()) {
      osName_ = LinuxParser::OperatingSystem();
  }
  return osName_;
}

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// DONE: Return the total number of processes on the system
int System::TotalProcesses() { 
    return LinuxParser::TotalProcesses(); 
}

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return 0; }
