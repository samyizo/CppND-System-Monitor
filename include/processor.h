#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <string>
#include <fstream>

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  const std::string kFilePath{"/proc/stat"};
};

#endif