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

int main(int argc, char* argv[]) {
  try{
  //JJJ: clean up the initialisation
  std::string parton_generation{};
  std::string input_path{};
  int LLPPID{0};
  int nMC{0};
  double k_factor {1.};
  double visibleBR {1.};
  
  if(argc == 7){
    parton_generation= charToString(argv[1]);  
    input_path= charToString(argv[2]);  
    LLPPID = atof(argv[3]); 
    nMC = atof(argv[4]); 
    k_factor = atof(argv[5]); 
    visibleBR = atof(argv[6]); 
  }
  //JJJ: do sanity checks of the input
  //JJJ: throw exceptions!
  else if(argc == 2){
    std::string temp{};
    std::string inputline;
    std::string filename(argv[1]);
    std::cout << "Reading input data from " + filename << '\n';
    
    std::ifstream inputfile(filename);
    if (inputfile.is_open()){
      temp.clear();
      inputfile >> temp;
      parton_generation = temp;
      temp.clear();
      inputfile >> input_path;
      temp.clear();
      inputfile >> temp;
      if(!std::all_of(temp.begin(), temp.end(), [](char ch) {return (std::isdigit(ch));}))
	die("Input is invalid!");
      LLPPID = atoi(temp.c_str());
      temp.clear();
      inputfile >> temp;
      if(!std::all_of(temp.begin(), temp.end(), [](char ch) {return (std::isdigit(ch));}))
	die("Input is invalid!");
      nMC = atoi(temp.c_str());
      temp.clear();
      inputfile >> temp;
      if(!std::all_of(temp.begin(), temp.end(), [](char ch) {return (std::isdigit(ch) || std::ispunct(ch));}))
	die("Input is invalid!");
      k_factor = stod(temp);
      temp.clear();
      inputfile >> temp;
      if(!std::all_of(temp.begin(), temp.end(), [](char ch) {return (std::isdigit(ch) || std::ispunct(ch));}))
	die("Input is invalid!");
      visibleBR = stod(temp);     
      inputfile.close();
    }
    else{
      std::cout << filename + " cannot be opened.";
      die("Input is invalid!");
    }
  }
  else{
    std::cout << "./main parton-event-generation input-path LLPPID nMC K-factor visible-BR" << std::endl;
    std::cout << "   - parton-event-generation: MG5 or PY8" << std::endl;        
    std::cout << "   - input-path: LHE file for MG5, cmnd file for PY8" << std::endl;        
    std::cout << "   - LLPPID: PID of the LLP you want to study" << std::endl;  
    std::cout << "   - nMC: the number of MC events to be analyzed" << std::endl;
    std::cout << "   - K-factor: rescaling the production cross section according to higher-order computation or experimental measurement" << std::endl;
    std::cout << "   - visible-BR: decay branching ratio of the LLP into visibles, between 0 and 1" << std::endl;
    std::cout << std::endl << std::endl;
    std::cout << "./main input.dat" << std::endl;
      exit(1);
  }
  
  std::cout << "parton event generation: " << parton_generation << std::endl;   
    std::cout << "input file path: " << input_path << std::endl;   
    std::cout << "PID of the LLP: " << LLPPID << std::endl;
    std::cout << "nMC: " << nMC << std::endl;   
    std::cout << "K-factor: " << k_factor << std::endl; 
    std::cout << "visible-BR: " << visibleBR << std::endl; 
    
    
   

//MAPP1 and MAPP2 coordinates
    //Detector Initialization
    int amountOfCorners=4;
    std::vector<Vertex> frontCorners(amountOfCorners);
    std::vector<Vertex> backCorners(amountOfCorners);


    //MAPP1 Cornerpoints
    Vertex frontP1(3.27,1.0,-52.82,"Cartesian");
    Vertex frontP2(5.42,1.0,-48.31,"Cartesian");
    Vertex frontP3(3.27,-2.0,-52.82,"Cartesian");
    Vertex frontP4(5.42,-2.0,-48.31,"Cartesian");
    Vertex backP1(4.0,1.0,-61.39,"Cartesian");
    Vertex backP2(6.19,1.0,-56.89,"Cartesian");
    Vertex backP3(4.0,-2.0,-61.39,"Cartesian");
    Vertex backP4(6.19,-2.0,-56.89,"Cartesian");
    
    frontCorners[0]=frontP1;
    frontCorners[1]=frontP2;
    frontCorners[2]=frontP3;
    frontCorners[3]=frontP4;
    backCorners[0]=backP1;
    backCorners[1]=backP2;
    backCorners[2]=backP4;
    backCorners[3]=backP3;

    CubicDetector MAPP1(frontCorners,backCorners,amountOfCorners);

    //MAPP2 Cornerpoints
    frontP1.update(3.27,1.0,-52.83,"Cartesian");
    frontP2.update(12.24,1.0,-33.63,"Cartesian");
    frontP3.update(3.27,-2.0,-52.83,"Cartesian");
    frontP4.update(12.24,-2.0,-33.63,"Cartesian");
    backP1.update(4.0,1.0,-61.39,"Cartesian");
    backP2.update(16.53,1.0,-35.45,"Cartesian");
    backP3.update(4.0,-2.0,-61.39,"Cartesian");
    backP4.update(16.53,-2.0,-35.45,"Cartesian");

    frontCorners[0]=frontP1;
    frontCorners[1]=frontP2;
    frontCorners[2]=frontP3;
    frontCorners[3]=frontP4;
    backCorners[0]=backP1;
    backCorners[1]=backP2;
    backCorners[2]=backP4;
    backCorners[3]=backP3;
        
    CubicDetector MAPP2(frontCorners,backCorners,amountOfCorners);



    analysis mychecker;
    
    mychecker.setVerbose();
    mychecker.setPARTONGENERATION(parton_generation);
    mychecker.setINPUTPATH(input_path);
    mychecker.setLLPPID(LLPPID);
    mychecker.setKFACTOR(k_factor);
    mychecker.setVISIBLEBR(visibleBR);

    if (!mychecker.initPythia())
        return 1;
    if (!mychecker.runPythia(nMC,MAPP1,MAPP2))
        return 1;
    return 0;


  }
  catch (...)
  {
    std::cout << "Desaster!!! " << '\n';
    return 1;
  }
  
}
