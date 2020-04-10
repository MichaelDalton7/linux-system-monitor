#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
// TODO: Remove this line
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::stod;
using std::stol;
using std::vector;

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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string fileName = "/proc" + kMeminfoFilename;
  string totalMemoryString = LinuxParser::SearchKeyValueFile(fileName, kTotalMemoryKeyIdentifier, GetRamInMbFromStream);
  string freeMemoryString = LinuxParser::SearchKeyValueFile(fileName, kFreeMemoryKeyIdentifier, GetRamInMbFromStream);
  if (!totalMemoryString.empty() && !freeMemoryString.empty()) {
    double totalMemory = stod(totalMemoryString);
    double freeMemory = stod(freeMemoryString);
    return (totalMemory - freeMemory) / totalMemory;
  }
  return 0.0; 
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { return 0; }

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  // TotalJiffies = IdleJiffies + ActiveJiffies
  return LinuxParser::IdleJiffies() + LinuxParser::ActiveJiffies(); 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  string fileName = "/proc" + kStatFilename;
  string activeJiffiesCountString = LinuxParser::SearchKeyValueFile(fileName, kCpuKeyIdentifier, GetActiveJiffiesFromStream);
  if (!activeJiffiesCountString.empty()) {
    return stol(activeJiffiesCountString);
  }
  return 0; 
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  string fileName = "/proc" + kStatFilename;
  string idleJiffiesCountString = LinuxParser::SearchKeyValueFile(fileName, kCpuKeyIdentifier, GetIdleJiffiesFromStream);
  if (!idleJiffiesCountString.empty()) {
    return stol(idleJiffiesCountString);
  }
  return 0; 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 

// CPU_Percentage = (totald - idled)/totald
  return {};
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  return GetProcessesCount(kTotalProcessesKeyIdentifier); 
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  return GetProcessesCount(kRunningProcessesKeyIdentifier);
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    return line;
  }
  return line; 
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string fileName = kProcDirectory + to_string(pid) + kStatusFilename;
  return LinuxParser::SearchKeyValueFile(fileName, kRamKeyIdentifier, GetRamInMbFromStream);
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string fileName = kProcDirectory + to_string(pid) + kStatusFilename;
  return LinuxParser::SearchKeyValueFile(fileName, kUidKeyIdentifier, GetNextValueFromStream);
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string line, name, userId, separator;
  string processOwnerUid = LinuxParser::Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> name >> separator >> userId;
      if (processOwnerUid == userId) {
        return name;
      }
    }
  }
  return name;
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string fileName = kProcDirectory + to_string(pid) + kStatFilename;
  string key, line;
  std::ifstream stream(fileName);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    int startTimeIndex = 21;
    string startTimeString = LinuxParser::GetValueFromLineStream(linestream, startTimeIndex);
    if (!startTimeString.empty()) {
      return stol(startTimeString)/sysconf(_SC_CLK_TCK);
    }
  }
  return 0; 
}

int LinuxParser::GetProcessesCount(string keyIdentifier) {
  string fileName = "/proc" + kStatFilename;
  string processesString = LinuxParser::SearchKeyValueFile(fileName, keyIdentifier, GetNextValueFromStream);
  if (!processesString.empty()) {
    return stoi(processesString);
  }
  return 0; 
}

string LinuxParser::GetValueFromLineStream(std::istringstream &linestream, int searchedIndex) {
  string value;
  int streamIndex = 0;
  while (linestream >> value) {
    if (streamIndex == searchedIndex) {
      return value;
    }
    streamIndex++;
  }
  return string();
}

string LinuxParser::SearchKeyValueFile(string fileName, string searchedKey, HandleKeyFound KeyFoundFunc) {
  string key, line;
  std::ifstream stream(fileName);
  if (stream.is_open()) {
     while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == searchedKey) {
        return KeyFoundFunc(linestream);
      }
    }
  }
  return string();
}

string LinuxParser::ConvertToMb(string size, string unit) {
  if (unit == "mB") {
    return size;
  } else if (unit == "kB") {
    double convertedSize = (stod(size)/1000);
    string sizeString = to_string(convertedSize);
    // Remove unneseccary trailing 0s from the string
    sizeString.erase(sizeString.find_last_not_of("0") + 1, string::npos);
    return sizeString;
  }
  return size;
}

string LinuxParser::GetNextValueFromStream(std::istringstream &linestream) {
  string nextValue;
  linestream >> nextValue;
  return nextValue;
}

string LinuxParser::GetRamInMbFromStream(std::istringstream &linestream) {
  string memorySize, memoryUnit;
  linestream >> memorySize >> memoryUnit;
  return LinuxParser::ConvertToMb(memorySize, memoryUnit);
}

string LinuxParser::GetIdleJiffiesFromStream(std::istringstream &linestream) {
  // IdleJiffies = idle + iowait
  string user, nice, system, idle, iowait;
  linestream >> user >> nice >> system >> idle >> iowait;
  return to_string(stol(idle) + stol(iowait));
}

string LinuxParser::GetActiveJiffiesFromStream(std::istringstream &linestream) {
  // ActiveJiffies = user + nice + system + irq + softirq + steal
  string user, nice, system, idle, iowait, irq, softirq, steal;
  linestream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
  return to_string(stol(user) + stol(nice) + stol(system) + stol(irq) + stol(softirq) + stol(steal));
}