#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <functional> 
#include <algorithm>
#include "include/analysis.h"
#include "include/functions.h"
#include "include/tools.h"
#include "fstream"
#include "include/main.h"
#include "json.hpp"
#include <unordered_map>
#include <map>
#include <utility>
#include "include/CInputInterface.h"

class myexception defaultEx;

int die(std::string output){
  std::cout << output << std::endl;
  throw defaultEx;
}

int main(int argc, char* argv[]) {
  try{
    const bool storeDefault = false;
    
    if(storeDefault)
      storeDefaultCards();

    inputInterface input;
    
    if(argc == 4){
      std::string filenameEvents(argv[1]);
      std::string filenameLLPs(argv[2]);
      std::string resultPath(argv[3]);
      std::cout << '\n';
      std::cout << "->+|+<-o->+|+<-o->+|+<-o->+|+<-o->+|+<-o->+|+<-o->+|+<-o->+|+<-o->+|+<-o->+|+<-" << '\n';
      std::cout << "           >>> WELCOME TO DISPLACED DECAY COUNTER <<<" << '\n';      std::cout << "->+|+<-o->+|+<-o->+|+<-o->+|+<-o->+|+<-o->+|+<-o->+|+<-o->+|+<-o->+|+<-o->+|+<-" << '\n';
      std::cout << '\n';
      std::cout << "**** Reading input data from " + filenameEvents + " ****" << '\n';
      std::cout << '\n';
      
      std::ifstream inputfileEvents(filenameEvents);
      if (!inputfileEvents.is_open()){
      	std::cout << filenameEvents + " cannot be opened.";
      	die("Input is invalid!");
      }
      std::ifstream inputfileLLPs(filenameLLPs);
      if (!inputfileLLPs.is_open()){
      	std::cout << filenameLLPs + " cannot be opened.";
      	die("Input is invalid!");
      }
      input.setInput(inputfileEvents, inputfileLLPs, resultPath);
    }
    else if(argc == 10){
      input.setInput(argc,argv);
    }
    
    else{
      std::cout << "Command Line Input: \n";
      std::cout << "./main input_file_format input_file_path LLPPID mass ctau sigma BR_vis nMC path" << std::endl;
      std::cout << "   - input_file_format: LHE, HEPMC or CMND" << std::endl;        
      std::cout << "   - input_file_path: LHE/HEPMC file for LHE/HEPMC, cmnd file for PY8" << std::endl;        
      std::cout << "   - LLPPID: PID of the LLP you want to study" << std::endl;   
      std::cout << "   - mass: LLP mass in GeV" << std::endl;   
      std::cout << "   - ctau: LLP ctau in meter" << std::endl;  
      std::cout << "   - sigma: production cross section in fb" << std::endl;  
      std::cout << "   - BR_vis: decay branching ratio of the LLP into visibles, between 0 and 1" << std::endl;
      std::cout << "   - nMC: the number of MC events to be analyzed" << std::endl;
      std::cout << "   - path to result file" << std::endl;
      std::cout << std::endl << std::endl;
      std::cout << "Input Card: \n";
      std::cout << "./main <inputEvents.dat> <inputLLPs.dat> <pathToResultFile>" << std::endl;
      exit(1);
    }
  
    std::ofstream myfile;
    myfile.open ("Logs/input_summary.txt");
    
    myfile << "***************************************************************" << "\n";
    myfile << "***************** WELCOME TO THE INPUT SUMMARY ****************" << "\n";
    myfile << "***************************************************************" << "\n";
    myfile << "\n";
    myfile << "***************************************************************" << "\n";

    for(auto llp: input.LLPdata){
      myfile << "	LLP characteristics:" << "\n";
      myfile << "***************************************************************" << "\n";
      myfile << " - PID:		" << llp.LLPPID << "\n";
      myfile << " - mass [GeV]:	" << llp.mass << "\n";
      myfile << " - ctau [m]:	" << llp.ctau << "\n";
      myfile << " - \"visible\" BR:	" << llp.visibleBR << "\n";  
      myfile << "***************************************************************" << "\n";
      myfile << "\n";
    }
    
    myfile << "***************************************************************" << "\n";
    myfile << "	Cross-section / event information:" << "\n";
    myfile << "***************************************************************" << "\n";
    myfile << " - input file format:	" << input.input_file_format << "\n";   
    myfile << " - input file path:	" << input.input_file_path << "\n";   
    myfile << " - number of MC events:	" << input.nMC << "\n";  
    myfile << " - normalization XS [fb]:	" << input.sigma << "\n";  
    myfile << "***************************************************************" << "\n";
    myfile << "\n";
    
    std::vector <std::tuple<std::string,double>> myDetectorList;
    myDetectorList.clear();
    
    myfile << "***************************************************************" << "\n";
    myfile << "	Detector information (name, Int. Lumi. in fb^-1):" << "\n";
    myfile << "***************************************************************" << "\n";
        
    for(auto it = input.myDetectorList.begin(); it != input.myDetectorList.end(); ++it){
      myfile << std::get<0>(*it) << " " << std::get<1>(*it) << std::endl;
    }

    
    myfile << "***************************************************************" << "\n";
    myfile << "\n";
    myfile << "***************************************************************" << "\n";
    myfile << "******************** END OF INPUT SUMMARY *********************" << "\n";
    myfile << "***************************************************************" << "\n";
    myfile.close();
    
    analysis defaultAnalysisHandler;

    defaultAnalysisHandler.setAllInput(input);

    if (input.input_file_format == "LHE" || input.input_file_format == "CMND"){
    	if (!defaultAnalysisHandler.initPythia()) return 1;
	if (!defaultAnalysisHandler.runPythia(input.nMC,input.pathToResultFile))  return 1;
    }
    
    else if (input.input_file_format == "HEPMC"){   
       
      if (!defaultAnalysisHandler.runPythia(input.nMC,input.pathToResultFile)) return 1;
    }
    
    std::cout << "->+|+<-o->+|+<-o->+|+<-o->+|+<-o->+|+<-o->+|+<-o->+|+<-o->+|+<-o->+|+<-o->+|+<-" << '\n';
    std::cout << "   >>> DISPLACED DECAY COUNTER is coming to a happy end! <<<" << '\n';
    std::cout << "->+|+<-o->+|+<-o->+|+<-o->+|+<-o->+|+<-o->+|+<-o->+|+<-o->+|+<-o->+|+<-o->+|+<-" << '\n';
    std::cout << '\n';
    
    return 0;
    
  }

  catch (...){
    std::cout << "Disaster!!! " << '\n';
    return 1;
  }
  
}
