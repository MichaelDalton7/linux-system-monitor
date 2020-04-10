#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
std::vector<std::string> CpuUtilization();
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
  
// Custom Constants
const std::string kUidKeyIdentifier{"Uid:"};  
const std::string kRamKeyIdentifier{"VmSize:"};
const std::string kTotalProcessesKeyIdentifier{"processes"};
const std::string kRunningProcessesKeyIdentifier{"procs_running"};
const std::string kTotalMemoryKeyIdentifier{"MemTotal:"};
const std::string kFreeMemoryKeyIdentifier{"MemFree:"};
const std::string kAvailableMemoryKeyIdentifier{"MemAvailable:"};
const std::string kBuffersKeyIdentifier{"Buffers:"};
const std::string kCpuKeyIdentifier{"cpu"};

typedef std::string (*HandleKeyFound)(std::istringstream &linestream);

// Custom functions
int GetProcessesCount(std::string keyIdentifier);
std::string SearchKeyValueFile(std::string fileName, std::string searchedKey, HandleKeyFound KeyFoundFunc);
std::string GetNextValueFromStream(std::istringstream &linestream);
std::string GetRamInMbFromStream(std::istringstream &linestream);
std::string GetIdleJiffiesFromStream(std::istringstream &linestream);
std::string GetActiveJiffiesFromStream(std::istringstream &linestream);
std::string ConvertToMb(std::string size, std::string unit);
std::string GetValueFromLineStream(std::istringstream &linestream, int searchedIndex);
};  // namespace LinuxParser

#endif