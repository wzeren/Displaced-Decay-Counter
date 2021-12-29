#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iterator>
#include <functional> 
#include <algorithm>
#include "include/analysis.h"
#include "include/functions.h"
#include "fstream"
#include "include/main.h"
#include "nlohmann/json.hpp"
#include <unordered_map>
#include <map>
#include <utility>

using json = nlohmann::json;

class myexception defaultEx;

int die(std::string output){
  std::cout << output << std::endl;
  throw defaultEx;
}

//./main  input_file_format    input_file_path    LLPPID    mass    ctau     sigma     BR_vis      NMC

int main(int argc, char* argv[]) {
  try{
    bool storeDefault = false;
    
    json input;
    std::map<std::string, std::array<int,2>> defaultDetectors, detectors;
    json inputTest(detectors);
    std::string input_file_format, input_file_path;
    float sigma{};
    int nMC{};
    
    int LLPPID{};
    double mass{};
    double ctau{};
    double visibleBR {1.};
    
    if(storeDefault){
      defaultDetectors.insert({{"MATHUSLA1",{1,3000}}});
      defaultDetectors.insert({{"MATHUSLA2",{1,3000}}});
      defaultDetectors.insert({{"FASER",{1,3000}}});
      defaultDetectors.insert({{"FASER2",{1,3000}}});
      defaultDetectors.insert({{"ANUBIS0",{1,3000}}});
      defaultDetectors.insert({{"ANUBIS1",{1,3000}}});
      defaultDetectors.insert({{"AL3X",{1,3000}}});
      defaultDetectors.insert({{"CODEXB0",{1,3000}}});
      defaultDetectors.insert({{"CODEXB1",{1,3000}}});
      defaultDetectors.insert({{"MAPP1",{1,3000}}});
      defaultDetectors.insert({{"MAPP2",{1,3000}}});
      defaultDetectors.insert({{"FACET",{1,3000}}});
      json m1(defaultDetectors);
      std::ofstream("detectors.dat") << m1;
    }	  
    
    if(storeDefault){
      input =
	{
	  {
	    "input",{
	      {"input_file_format","LHE"},
	      {"input_file_path","../example_input/mg5/pp2W2eN_5GeV_VvSq1em7/unweighted_events.lhe.gz"},
	      {"sigma",1.66274},
	      {"nMC",1000}
	    }
	  },
	  
	  {
	    "LLP",{
	      {"LLPPID",9900012},
	      {"mass",5},
	      {"ctau",1},
	      {"visibleBR",1}
	    }
	  }
	};
      std::ofstream("input.dat") << input;
    }

    
    if(argc == 2){
      std::string filename(argv[1]);
      std::cout << "Reading input data from " + filename << '\n';

      std::ifstream inputfile(filename);
      if (!inputfile.is_open()){
      	std::cout << filename + " cannot be opened.";
      	die("Input is invalid!");
      }

      inputfile >> input;

      input["input"]["input_file_format"].get_to(input_file_format);
      input["input"]["input_file_path"].get_to(input_file_path);
      input["input"]["sigma"].get_to(sigma);
      input["input"]["nMC"].get_to(nMC);
      
      input["LLP"]["LLPPID"].get_to(LLPPID);
      input["LLP"]["mass"].get_to(mass);
      input["LLP"]["ctau"].get_to(ctau);
      input["LLP"]["visibleBR"].get_to(visibleBR);
      
    }
    else if(argc == 9){
      input_file_format= charToString(argv[1]);
      input_file_path= charToString(argv[2]);  
      LLPPID = atof(argv[3]); 
      mass = atof(argv[4]);
      ctau = atof(argv[5]);
      sigma = atof(argv[6]); 
      visibleBR = atof(argv[7]); 
      nMC = atof(argv[8]); 
    }
  
  else{
    std::cout << "./main input_file_format input_file_path LLPPID mass ctau sigma BR_vis NMC" << std::endl;
    std::cout << "   - input_file_format: LHE, HEPMC or CMND" << std::endl;        
    std::cout << "   - input_file_path: LHE/HEPMC file for LHE/HEPMC, cmnd file for PY8" << std::endl;        
    std::cout << "   - LLPPID: PID of the LLP you want to study" << std::endl;   
    std::cout << "   - mass: LLP mass in GeV" << std::endl;   
    std::cout << "   - ctau: LLP ctau in meter" << std::endl;  
    std::cout << "   - sigma: production cross section in fb" << std::endl;  
    std::cout << "   - BR_vis: decay branching ratio of the LLP into visibles, between 0 and 1" << std::endl;
    std::cout << "   - nMC: the number of MC events to be analyzed" << std::endl;
    std::cout << std::endl << std::endl;
    std::cout << "./main input.dat" << std::endl;
      exit(1);
  }
  
  //std::cout << "parton event generation: " << parton_generation << std::endl;
    std::cout <<  "input_file_format: " << input_file_format << std::endl;   
    std::cout << "input_file_path: " << input_file_path << std::endl;   
    std::cout << "PID of the LLP: " << LLPPID << std::endl;
    std::cout << "mass [GeV]: " << mass << std::endl;
    std::cout << "ctau [m]: " << ctau << std::endl;
    std::cout << "sigma [fb]: " << sigma << std::endl;
    std::cout << "BR_vis: " << visibleBR << std::endl; 
    std::cout << "nMC: " << nMC << std::endl;   
    std::cout << "***************************************************************" << std::endl;   
    
    //Read detector settings    
    std::ifstream("detectors.dat") >> inputTest;
    for (auto& x : json::iterator_wrapper(inputTest)){
      //      std::cout << "Detector: " << x.key() << ", Activated: " << x.value()[0] << ", Lumi: " << x.value()[1] << '\n';
      detectors.insert({{(std::string)x.key(),{(int)x.value()[0],(int)x.value()[1]}}});
    }

    std::vector <std::tuple<std::string,double>> myDetectorList;
    myDetectorList.clear();

    std::cout << "You have called the following detectors (name,on/off,Lumi fb^-1):" << std::endl;
    for(auto it = detectors.begin(); it != detectors.end(); ++it){
     std::cout << it->first << " " << it->second[0] << " " << it->second[1] << std::endl;
     if((it->second[0])==1){
      std::tuple<std::string,double> newDet(it->first, it->second[1]);
      myDetectorList.push_back(newDet);
     } 
    }



    analysis mychecker;
    
    //mychecker.setVerbose();
    mychecker.setINPUTFILEFORMAT(input_file_format);
    mychecker.setINPUTFILEPATH(input_file_path);
    mychecker.setLLPPID(LLPPID);
    mychecker.setMASS(mass);
    mychecker.setCTAU(ctau);
    mychecker.setSIGMA(sigma);
    mychecker.setVISIBLEBR(visibleBR);
    mychecker.setDETECTORS(myDetectorList);

    if (input_file_format == "LHE" || input_file_format == "CMND"){
    	if (!mychecker.initPythia()) return 1;
	if (!mychecker.runPythia(nMC))  return 1;
    	
    }
    
    else if (input_file_format == "HEPMC"){   
       
    	//std::cout << "Zong" << std::endl;
    
    	if (!mychecker.runHepMC(nMC)) return 1;
    }
    
    return 0;


  }
  catch (...)
  {
    std::cout << "Disaster!!! " << '\n';
    return 1;
  }
  
}
