#include "processor.h"
#include <sstream>
#include <string>
#include <iostream>

using std::string;
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  
  float utilization = 0;
  string cpuline, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  string line;
  std::ifstream stream(kFilePath);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpuline >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
 utilization = (std::stof(user) + std::stof(nice) + std::stof(system) + std::stof(irq) + std::stof(softirq) + std::stof(steal)) / (std::stof(user) + std::stof(nice) + std::stof(system) + std::stof(irq) + std::stof(softirq) + std::stof(steal) + std::stof(idle) + std::stof(iowait)); 
    // std::cout << std::stof(user) << "\n";
  }
  return utilization;
  
                               }