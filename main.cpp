#include <iostream>
#include <map>
#include <tuple>
#include <sstream>
#include <string>
#include "log.h"
using string = std::string;
using namespace teaflow;
int main() {
  std::cout << "test" << std::endl;
  LOGg << "this is hello";
  LOGv << "test";
  LOGg << "this is hello2";
  return 0;
}