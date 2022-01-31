#include "include/CInputInterface.h"


void inputInterface::setInput(std::ifstream &inputfileEvents, std::ifstream &inputfileLLPs){
  inputfileEvents >> inputEvents;
  inputfileLLPs >> inputLLPs;
  
  inputEvents["input"]["input_file_format"].get_to(input_file_format);
  inputEvents["input"]["input_file_path"].get_to(input_file_path);
  inputEvents["input"]["sigma"].get_to(sigma);
  inputEvents["input"]["nMC"].get_to(nMC);
  
  inputLLPs["LLP"]["LLPPID"].get_to(LLPPID);
  inputLLPs["LLP"]["mass"].get_to(mass);
  inputLLPs["LLP"]["ctau"].get_to(ctau);
  inputLLPs["LLP"]["visibleBR"].get_to(visibleBR);


  /*for(auto it = inputLLPs.begin(); it != inputLLPs.end(); ++it){
    std::cout << *it << std::endl;
    }
  */

  //FOR FLORIAN:
  
  /* for (auto& x : nlohmann::json::iterator_wrapper(inputLLPs))
    {
        std::cout << "key: " << x.key() << ", value: " << x.value()["LLPPID"] << '\n';
    }
  */
  
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

