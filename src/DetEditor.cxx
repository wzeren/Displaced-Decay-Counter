//********************** EDITOR OF DETECTORS PROJECT *****************************
#include <iostream>
#include <fstream>
#include <vector>

/* Function Copying the code associated with the new detector */

void CopyNewDet(std::string myName){
 std::ofstream myfile;
 
/* Opens file src/Detectors/<myName>.cc and inserts the text defining a trivial
    detector with the new identifier: */
 myfile.open ("Detectors/" + myName + ".cc");
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
 myfile.close();
 
/* Open file include/Detectors/<myName>.h and inserts the proper headers: */
 myfile.open ("../include/Detectors/" + myName + ".h");
 myfile << "#ifndef _D" << myName << "_" << "\n";
 myfile << "#define _D" << myName << "_" << "\n";
 myfile << "\n";
 myfile << "#include \"include/CDetector.h\"" << "\n";
 myfile << "\n";
 myfile << "Detector " << myName << "();" << "\n";
 myfile << "\n";
 myfile << "#endif" << "\n"; 
 myfile.close();

/* TO DO: open src/detectors.cc and go to the line before "BUILDING THE LIST OF STUDIED DETECTORS" and insert:
  Copy by hand for now. */
 myfile.open ("testprint.txt", std::ios_base::app);
 myfile << "\n";
 myfile << " Detector " << myName << "X=" << myName << "();" << "\n";
 myfile << " knownDet.push_back(" << myName << "X);" << "\n";

/* TO DO: open include/detectors.h and go to the line before "std::vector<Detector> CreateDetectors(std::vector<std::string>);" and insert:
  Copy by hand for now. */
 myfile << "#include \"include/Detectors/" << myName << ".h\"" << "\n";
 myfile.close();
 
/* Also copy the detector's name 'myName' in the storage file 'knowndet.txt' */
 myfile.open ("knowndet.txt", std::ios_base::app);
 myfile << myName << "\n";
 myfile.close();
 
}

/* Main function, prompting the user for a detector identifier,
    then calling the editing function. */

int main(){
 std::string DetName;
 std::cout << "Please enter the name of the new detector:" << '\n'; // Prompt.
 std::getline(std::cin,DetName);

/* TO DO: check the storage file 'knowndet.txt' where the names of all known detectors are listed, read this file / import this list in std::vector<std::string> KnownDet;
  Vector edited by hand for now. */
 std::vector<std::string> KnownDet={"MATHUSLA0","MATHUSLA1","MATHUSLA2","FASER","FASER2","ANUBIS0","ANUBIS1","CODEXB0","CODEXB1","AL3X","MAPP1","MAPP2","FACET"};
 bool nameistaken=false;
 for(int i=0;i<KnownDet.size();i++){
  if(KnownDet[i]==DetName)nameistaken=true;    // Checking usage of the desired name.
 }
 if(nameistaken){
  std::cout << "Sorry: this name is already in use! Aborting." << '\n';
 } else {
  std::cout << "This name is available. Producing the skeleton code." << '\n';
  std::cout << "You may now edit " << DetName << ".cc in src/Detectors/." << '\n';
 }
 if(!nameistaken){                             // Calling the editing function.
  CopyNewDet(DetName);
 }

return 0;
}
