/*! \file
 * Definition of the input interface.
*/
#ifndef _CINPUTINTERFACE_
#define _CINPUTINTERFACE_

#include <iostream>
#include "fstream"
#include <string>
#include <vector>
#include <iterator>
#include "json.hpp"
#include "CInputInterface.h"
#include "include/functions.h"

/*! \brief
 * The inputInterface collects the input in json format.
*/

class inputInterface{
public:
  void setInput(std::ifstream &inputfileEvents, std::ifstream &inputfileLLP, std::string inputPathToResultFile);
  void setInput(int argc, char *argv[]);
  float sigma{};
  int nMC{};
  //  int LLPPID{};
  //  double mass{};
  //  double ctau{}; 
  //  double visibleBR {1.};
  
/*! \brief
 * The CLLP class stores the characteristics of a given LLP.
*/

  class CLLP{
  public:
    int LLPPID{};
    double mass{};
    double ctau{}; 
    double visibleBR {1.};
  };

  std::vector <CLLP> LLPdata;
  
  std::vector <std::tuple<std::string,double>> myDetectorList;
  std::string input_file_format, input_file_path;
  std::string pathToResultFile;
  
private:
  nlohmann::json inputEvents;
  nlohmann::json inputLLPs;
  
  std::map<std::string, std::array<int,2>> detectors;
  nlohmann::json inputDet;
  
};


#endif
