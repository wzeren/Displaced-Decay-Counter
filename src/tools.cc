#include "include/tools.h"

void storeDefaultCards(){

  std::map<std::string, std::array<int,2>> defaultDetectors, detectors;
  
  defaultDetectors.insert({{"MATHUSLA0",{1,3000}}});
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
  nlohmann::json m1(defaultDetectors);
  std::ofstream("detectors.dat") << m1;

  nlohmann::json input;
  
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
