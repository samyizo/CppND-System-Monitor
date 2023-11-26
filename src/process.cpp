#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;



Process::Process(int constPid) {
	pid = constPid;

   cpuUtil = CpuUtilization();
   command = LinuxParser::Command(pid);
   ram = LinuxParser::Ram(pid);
   user = LinuxParser::User(pid);
   upTime = LinuxParser::UpTime(pid);

   
} 

// TODO: Return this process's ID 
int Process::Pid() { return pid; } 

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
 float cpuUtil = float(LinuxParser::ActiveJiffies(Pid())/10.f) / float((LinuxParser::UpTime(Pid())));
  return cpuUtil;
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid); }

// TODO: Return this process's memory utilization
string Process::Ram() const {return LinuxParser::Ram(pid); }

// TODO: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const { 
  try{return std::stof(Ram()) < std::stof(a.Ram());}
          catch (std::invalid_argument &e)  {return true;}                               
}