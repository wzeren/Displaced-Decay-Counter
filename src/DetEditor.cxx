//********************** EDITOR OF DETECTORS PROJECT *****************************
/*! \file
 * The DetEditor sub-program generates code for the inclusion of new detectors.
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <experimental/filesystem>
#include <algorithm>
#include <iterator>

namespace fs = std::experimental::filesystem;

/*! 
 * Auxilliary function copying code lines at specific locations in /src/detectors.cc and /include/detectors.h .
*/

void modifyDetectorsSource(const std::string myName, const std::string lookForExpr, const std::string input, const std::string fileExtension){

  fs::path currentPath(fs::current_path());
  fs::path inputPath;
  if(fileExtension.compare("cc") == 0)
    inputPath = currentPath / "../src/detectors.cc";
  else
    inputPath = currentPath / "../include/detectors.h";
  
  if(!fs::exists(inputPath))
    throw inputPath.string() + " does not exist.";
  
  if (!fs::is_regular_file(inputPath))
    throw inputPath.string() + " is not a regular file.";
 
  std::ifstream file(inputPath, std::ios::in);
  if (!file.is_open())
        throw inputPath.string() + " cannot be opened.";

 
  const std::size_t& size = fs::file_size(inputPath);
  std::string content(size, '\0');
  file.read(content.data(), size);
  file.close();
  
  size_t strPosition = content.find(lookForExpr);
 
  if(strPosition != std::string::npos){

    content.insert(strPosition-1,input);
    
    fs::remove_all(inputPath);
    
    std::ofstream outputFile(inputPath);
    outputFile << content;
    outputFile.close();
  }
  else
    throw inputPath.string() + " content is invalid. New detector cannot be added!";
}

/*! 
 * Auxilliary function copying code lines at specific locations in /src/CAnalyseEvent.cc and /include/CAnalyseEvent.h .
*/

void modifyDetCutsSource(const std::string myName, const std::string lookForExpr, const std::string input, const std::string fileExtension){

  fs::path currentPath(fs::current_path());
  fs::path inputPath;
  if(fileExtension.compare("cc") == 0)
    inputPath = currentPath / "../src/CAnalyseEvent.cc";
  else
    inputPath = currentPath / "../include/CAnalyseEvent.h";
  
  if(!fs::exists(inputPath))
    throw inputPath.string() + " does not exist.";
  
  if (!fs::is_regular_file(inputPath))
    throw inputPath.string() + " is not a regular file.";
 
  std::ifstream file(inputPath, std::ios::in);
  if (!file.is_open())
        throw inputPath.string() + " cannot be opened.";

 
  const std::size_t& size = fs::file_size(inputPath);
  std::string content(size, '\0');
  file.read(content.data(), size);
  file.close();
  
  size_t strPosition = content.find(lookForExpr);
 
  if(strPosition != std::string::npos){

    content.insert(strPosition-1,input);
    
    fs::remove_all(inputPath);
    
    std::ofstream outputFile(inputPath);
    outputFile << content;
    outputFile.close();
  }
  else
    throw inputPath.string() + " content is invalid. New detector cannot be added!";
}

/*! 
 * Principal function copying the code associated with the new detector.
*/

void CopyNewDet(std::string myName){
 std::ofstream myfile;
 
/* Opens file src/Detectors/<myName>.cc and inserts the text defining a trivial
    detector with the new identifier: */
 myfile.open ("../src/Detectors/" + myName + ".cc");
 myfile << "#include \"include/Detectors/" << myName << ".h\"" << "\n";
 myfile << "\n";
 myfile << "Detector " << myName << "() {" << "\n";
 myfile << " std::vector<CylDetLayer> myDetLayers;" << "\n";
 myfile << " myDetLayers.clear();" << "\n";
 myfile << " std::string Dname=\"" << myName << "\";" << "\n";
 myfile << " double DLumi=0.;" << "\n";
 myfile << " Detector myDetector(Dname,DLumi,myDetLayers);" << "\n";
 myfile << " return myDetector;" << "\n";
 myfile << "}" << "\n";
 myfile << "\n";
 myfile << "bool " << myName << "Cuts(HepMC::GenEvent* evtin){" << "\n";
 myfile << " bool cuts=true;" << "\n";
 myfile << " return cuts;" << "\n";
 myfile << "};" << "\n";
 myfile.close();
 
/* Open file include/Detectors/<myName>.h and inserts the proper headers: */
 myfile.open ("../include/Detectors/" + myName + ".h");
 myfile << "#ifndef _D" << myName << "_" << "\n";
 myfile << "#define _D" << myName << "_" << "\n";
 myfile << "\n";
 myfile << "#include \"include/CDetector.h\"" << "\n";
 myfile << "#include \"HepMC/IO_GenEvent.h\"" << "\n";
 myfile << "\n";
 myfile << "Detector " << myName << "();" << "\n";
 myfile << "\n";
 myfile << "bool " << myName << "Cuts(HepMC::GenEvent* evtInput);" << "\n";
 myfile << "\n";
 myfile << "#endif" << "\n"; 
 myfile.close();

/* Open src/detectors.cc and go to the line before "BUILDING THE LIST OF STUDIED DETECTORS" and insert: */

    std::stringstream input;    
    input << "\n";
    input << " // Building " << myName << "\n";
    input << " Detector " << myName << "X=" << myName << "();" << "\n";
    input << " knownDet.push_back(" << myName << "X);" << "\n";
    std::string lookForExpr = "// BUILDING THE LIST OF STUDIED DETECTORS";
    modifyDetectorsSource(myName, lookForExpr, input.str(),"cc");
    
/* Open include/detectors.h and go to the line before "// END OF INCLUDE DEFINITIONS" and insert: */

    lookForExpr = "// END OF INCLUDE DEFINITIONS";
    input.str("");    
    input << "#include \"include/Detectors/" << myName << ".h\"" << "\n";
    modifyDetectorsSource(myName, lookForExpr, input.str(),"h");
    
/* Open include/CAnalyseEvent.cc and go to the line before "// APPLYING CUTS TO DETECTORS" and insert: */

    lookForExpr = "// APPLYING CUTS TO DETECTORS";
    input.str("");    
    input << "\n";
    input << "  if(dettest==\"" << myName << "\")testres=" << myName << "Cuts(evt);" << "\n";
    modifyDetCutsSource(myName, lookForExpr, input.str(),"cc");
    
/* Open include/CAnalyseEvent.h and go to the line before "std::vector<Detector> CreateDetectors(std::vector<std::string>);" and insert: */

    lookForExpr = "// END OF INCLUDE DEFINITIONS";
    input.str("");    
    input << "#include \"include/Detectors/" << myName << ".h\"" << "\n";
    modifyDetCutsSource(myName, lookForExpr, input.str(),"h");
 
}

/*!
 * The main routine prompts the user for a detector identifier, then calls the editing function. 
*/

int main(){

  try{
    fs::path currentPath(fs::current_path());
    fs::path inputPath = currentPath / "../bin/knowndet.txt";

    if(!fs::exists(inputPath))
      throw "knowndet.txt does not exist.";

    if (!fs::is_regular_file(inputPath))
      throw "knowndet.txt is not a regular file.";

    std::ifstream file(inputPath, std::ios::in);
    if (!file.is_open())
      return { };

    std::string str;
    std::vector<std::string> KnownDet;
      
    while (std::getline(file, str)){
      if(str.size() > 0)
	KnownDet.push_back(str);
    }

    std::cout << "List of implemented detectors:" << std::endl;
    for(auto str : KnownDet)
      std::cout << str << std::endl;
         
    std::string DetName;
    std::cout << "Please enter the name of the new detector:" << '\n'; // Prompt.
    std::getline(std::cin,DetName);
    
    auto itKnownDet = std::find_if(KnownDet.begin(), KnownDet.end(),[&DetName](std::string name){return name.compare(DetName) == 0;});
    
    if(itKnownDet != KnownDet.end()){
      std::cout << "Sorry: this name is already in use! Aborting." << '\n';
    } else {
      std::cout << "This name is available. Producing the skeleton code." << '\n';
      std::cout << "You may now edit " << DetName << ".cc in src/Detectors/." << '\n';
    }
    if(itKnownDet == KnownDet.end()){                             // Calling the editing function.
      CopyNewDet(DetName);
      KnownDet.push_back(DetName);
      fs::remove_all(inputPath);
      
      std::ofstream output_file(inputPath);
      std::ostream_iterator<std::string> output_iterator(output_file, "\n");
      std::copy(KnownDet.begin(), KnownDet.end(), output_iterator);
      output_file.close();
    }
    
  }
  catch(const fs::filesystem_error &ex){
    std::cerr << ex.what() << std::endl;
  }
  catch(const char * str){
    std::cerr << str << std::endl;
  }
  catch(...){
    std::cerr << "something else is wrong" << std::endl;
  }

return 0;
}
