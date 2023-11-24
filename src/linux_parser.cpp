#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
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
  string os, kernel, version;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  
string line;
  string key;
  string value;
  float numerator;
  float denominator;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemFree:") { // 
          numerator += std::stof(value);
        }
        if (key == "MemTotal:") {
        denominator += std::stof(value);
        }
      }
    }
  }
 return (denominator - numerator) / denominator;  
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
string line, uptimestring;
long uptime = 0;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line); 
    std::istringstream linestream(line);
        linestream >> uptimestring;
    	uptime = std::stol(uptimestring);
    }
  return uptime;
}


// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return ActiveJiffies() + IdleJiffies(); }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  string line, key;
  long activeJiffies = 0;
  int currentListNum = 1;
  vector<string> returnVector{};

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (currentListNum == 13 | currentListNum == 14) {
        	activeJiffies += std::stol(key);
           }     
        currentListNum++;
      }
    }
 return activeJiffies;
  }
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  
  vector<string> jiffies = LinuxParser::CpuUtilization();
  long IdleJiffies = 0;
  IdleJiffies += std::stol(jiffies[1]);
  IdleJiffies += std::stol(jiffies[2]);
    IdleJiffies += std::stol(jiffies[3]);
    IdleJiffies += std::stol(jiffies[6]);
    IdleJiffies += std::stol(jiffies[7]);
    IdleJiffies += std::stol(jiffies[8]);
  return IdleJiffies; 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  
  vector<string> jiffies = LinuxParser::CpuUtilization();
  long IdleJiffies = 0;
  IdleJiffies += std::stol(jiffies[4]);
  IdleJiffies += std::stol(jiffies[5]);
  return IdleJiffies; 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 

  string line, key;
  vector<string> returnVector{};

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        	returnVector.push_back(key);
           }     
 return returnVector;
    }
  }
  
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  
string line;
  string key;
  string value;
  int totalProcs;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
      	totalProcs = std::stof(value);
        std::cout << totalProcs;
        return totalProcs;
      }
        }
      }

  

  // old way I was using to check pid by pid and check status...I guess this isn't an accurate rep?
  /*
  int numprocesses = 0;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      // std::cout << filename + "\n";
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        numprocesses++;
      }
  
 
}
  }
    return numprocesses;
   */
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  
  string line;
  string key;
  string value;
  int totalProcs;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") { // 
          totalProcs = std::stof(value);
        }
      }
    }
  }
  return totalProcs;
  // old way I was using to go pid by pid, maybe this is still accurate? but way more complex
  /*
  int numprocesses = 0;
  string key, value, line;
  
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        // check if this process in file path /proc/this/status - State = R or not
                  std::ifstream filestream(kProcDirectory + filename + "/status");
        		while (std::getline(filestream, line)) {
                  if (filestream.is_open()) { 
                    std::istringstream linestream(line);
                        linestream >> key >> value;
                        if (key == "State:") {
                        	if (value == "R") {
                            numprocesses++;
                            }
                        }
                        }
                    }
      }
}
  }
    return numprocesses; 
   */
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  
string key, line;
  
std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, key);
    return key;
}

}
// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  
string line, key, value, ram;
int ramToConvert;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") { // 
          ram = value;
          int ramToConvert = stoi(ram)/1000;
          ram = std::to_string(ramToConvert);
        }
      }
    }
  }
  return ram;
                                 }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  
string line, key, value, uid;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") { // 
          uid = value;
          return uid;
        }
      }
    }
  }
  return uid;
 }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string Uid = LinuxParser::Uid(pid);
  string user;
  string line;
  string name;
  string ignore;
  string uidToCheck;
  
  
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> name >> ignore >> uidToCheck) {
        if (uidToCheck == Uid) {
			user = name;
          return user;
        }
      }
    }
  }
  
  return string(); 
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string line, key;
  vector<string> statValues;

  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        statValues.push_back(key);
      }
    }
  }
  return LinuxParser::UpTime() - (stol(statValues[21])/100);
}
