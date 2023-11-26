#include "processor.h"
#include <sstream>
#include <string>
#include <iostream>
#include "linux_parser.h"

using std::string;
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  long jiffies = LinuxParser::Jiffies();
  long activeJiffies = LinuxParser::ActiveJiffies();
  return activeJiffies * (1.f / jiffies);
}