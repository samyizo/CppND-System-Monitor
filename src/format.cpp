#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
// string Format::ElapsedTime(long seconds[[maybe_unused]]) { return string(); }

string Format::ElapsedTime(long seconds) { 
  long hours = (seconds / 3600) % 100;
  long minutes = (seconds / 60) % 60;
  long finalseconds = seconds % 60;
  return to_string(hours) + ":" + to_string(minutes) + ":" + to_string(finalseconds); 
}