#include "include/CInputInterface.h"


void inputInterface::setInput(std::ifstream &inputfile){
  inputfile >> input;
  
  input["input"]["input_file_format"].get_to(input_file_format);
  input["input"]["input_file_path"].get_to(input_file_path);
  input["input"]["sigma"].get_to(sigma);
  input["input"]["nMC"].get_to(nMC);
  
  input["LLP"]["LLPPID"].get_to(LLPPID);
  input["LLP"]["mass"].get_to(mass);
  input["LLP"]["ctau"].get_to(ctau);
  input["LLP"]["visibleBR"].get_to(visibleBR);
  
  //Read detector settings
  nlohmann::json temp(detectors);
  std::ifstream("detectors.dat") >> temp;
  inputDet=temp;
  myDetectorList.clear();
  for (auto& x : nlohmann::json::iterator_wrapper(inputDet)){
    //      detectors.insert({{(std::string)x.key(),{(int)x.value()[0],(int)x.value()[1]}}});
    //      myfile << (std::string)x.key() << ":	" << (int)x.value()[0] << ",	" << (int)x.value()[1] << std::endl;
    if((int)x.value()[0]==1){
      std::tuple<std::string,double> newDet((std::string)x.key(), (int)x.value()[1]);
      myDetectorList.push_back(newDet);
    }
  }
}


void inputInterface::setInput(int argc, char *argv[]){
  input_file_format= charToString(argv[1]);
  input_file_path= charToString(argv[2]);  
  LLPPID = atof(argv[3]); 
  mass = atof(argv[4]);
  ctau = atof(argv[5]);
  sigma = atof(argv[6]); 
  visibleBR = atof(argv[7]); 
  nMC = atof(argv[8]); 
  
    //Read detector settings
  nlohmann::json temp(detectors);
  std::ifstream("detectors.dat") >> temp;
  inputDet=temp;
  myDetectorList.clear();
  for (auto& x : nlohmann::json::iterator_wrapper(inputDet)){
    //      detectors.insert({{(std::string)x.key(),{(int)x.value()[0],(int)x.value()[1]}}});
    //      myfile << (std::string)x.key() << ":	" << (int)x.value()[0] << ",	" << (int)x.value()[1] << std::endl;
    if((int)x.value()[0]==1){
      std::tuple<std::string,double> newDet((std::string)x.key(), (int)x.value()[1]);
      myDetectorList.push_back(newDet);
    }
  }
  
}

