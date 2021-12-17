// EDITOR OF DETECTORS PROJECT
#include <iostream>
#include <fstream>
#include <vector>

void CopyNewDet(std::string myName){
 std::ofstream myfile;
 myfile.open ("testprint.txt", std::ios_base::app);
 
/* Open file src/detectors.cc and go to the line before 'LIST OF DETECTORS' (for example). Then insert this text there: */
 myfile << "\n";
 myfile << "Detector " << myName << "() {" << "\n";
 myfile << " std::vector<CylDetLayer> myDetLayers.clear();" << "\n";
 myfile << " std::string Dname=\"" << myName << "\";" << "\n";
 myfile << " double DLumi=0.;" << "\n";
 myfile << " Detector myDetector(Dname,DLumi,myDetLayers);" << "\n";
 myfile << " return myDetector;" << "\n";
 myfile << "}" << "\n";

/* Then go to the line before "BUILDING THE LIST OF STUDIED DETECTORS" and insert: */
 myfile << "\n";
 myfile << " Detector " << myName << "X=" << myName << "();" << "\n";
 myfile << " knownDet.push_back(" << myName << "X);" << "\n";
 
 myfile.close();
 
/* Also copy the detector's name 'myName' in the storage file 'knowndet.txt' */
 myfile.open ("knowndet.txt", std::ios_base::app);
 myfile << "\n";
 myfile << myName;
 myfile.close();
 
}

int main(){
 std::string DetName;
 std::cout << "Please enter the name of the new detector:" << '\n';
 std::getline(std::cin,DetName);

/* Maybe have a storage file 'knowndet.txt' where the names of all known detectors are listed, read this file / import this list in std::vector<std::string> KnownDet;*/
 std::vector<std::string> KnownDet={"MATHUSLA0","MATHUSLA1","MATHUSLA2","FASER","FASER2","ANUBIS0","ANUBIS1","CODEXB0","CODEXB1","AL3X"};
 bool nameistaken=false;
 for(int i=0;i<KnownDet.size();i++){
  if(KnownDet[i]==DetName)nameistaken=true;
 }
 if(nameistaken){
  std::cout << "Sorry: this name is already in use! Aborting." << '\n';
 } else {
  std::cout << "This name is available. Producing the skeleton code." << '\n';
 }
 if(!nameistaken){ 
  CopyNewDet(DetName);
 }

return 0;
}
