#ifndef _CINPUTINTERFACE_
#define _CINPUTINTERFACE_

#include <iostream>
#include "fstream"
#include <string>
#include <vector>
#include <iterator>
#include "nlohmann/json.hpp"
#include "CInputInterface.h"
#include "include/functions.h"

class inputInterface{
public:
  void setInput(std::ifstream &inputfile);
  void setInput(int argc, char *argv[]);
  float sigma{};
  int nMC{};
  int LLPPID{};
  double mass{};
  double ctau{}; 
  double visibleBR {1.};  
  std::vector <std::tuple<std::string,double>> myDetectorList;
  std::string input_file_format, input_file_path;

private:
  nlohmann::json input;
  std::map<std::string, std::array<int,2>> detectors;
  nlohmann::json inputDet;
  
};


#endif
