#include "include/detectors.h"

/* The CreateDetectors function looks for the detectors from a list (vector) of
    identifiers (strings) and returns a vector of detectors that can be tested
    against MC events.
   It first declares all known detectors, adding them to the knownDet vector.
    Then it looks at the inputed 'wish-list', compares its elements to the identifiers
    of the known detectors and builds the desired list.
   A log file is copied to bin/Detectors/Detector_log.txt , 
    detailing the success or problems in establishing the list. */

//                   LIST OF DETECTORS

std::vector<Detector> CreateDetectors(std::vector<std::string> nameList) {
 std::vector<Detector> myList,knownDet;
 myList.clear();
 knownDet.clear();
 
//   KNOWN DETECTORS
     // Uncle Simon's MATHUSLA
 Detector MATHUSLAO=MATHUSLA0();
 knownDet.push_back(MATHUSLAO);
    
    // Improved MATHUSLA
 Detector MATHUSLAI=MATHUSLA1();
 knownDet.push_back(MATHUSLAI);
    
    // Building MATHUSLA from 3m-high bricks
 Detector MATHUSLAB=MATHUSLA2();
 knownDet.push_back(MATHUSLAB);
    
    // FASER
 Detector FASERI=FASER1();
 knownDet.push_back(FASERI);
    
    // FASER2
 Detector FASERII=FASER2();
 knownDet.push_back(FASERII);
    
    // Uncle Simon's ANUBIS
 Detector ANUBISO=ANUBIS0();
 knownDet.push_back(ANUBISO);
    
    // Building ANUBIS from 1m-high, 1m-deep bricks
 Detector ANUBISB=ANUBIS1();
 knownDet.push_back(ANUBISB);
    
    // Building AL3X
 Detector AL3X0=AL3X();
 knownDet.push_back(AL3X0);
    
    // Building a simple CODEXB
 Detector CODEXBO=CODEXB0();
 knownDet.push_back(CODEXBO);
    
    // Building an improved CODEXB
 Detector CODEXBI=CODEXB1();
 knownDet.push_back(CODEXBI);

    // Building MAPP1
 Detector MAPP1X=MAPP1();
 knownDet.push_back(MAPP1X);
    
    // Building MAPP2
 Detector MAPP2X=MAPP2();
 knownDet.push_back(MAPP2X);
 
    // Building FACET
 Detector FACETX=FACET();
 knownDet.push_back(FACETX);

// BUILDING THE LIST OF STUDIED DETECTORS
   std::vector<std::array<double,3>> TDcoord;
   TDcoord.clear(); 
   std::ofstream myfile;
   myfile.open ("Detectors/Detector_log.txt");
   myfile << "Checking the availability of detectors." << "\n";
 
 for(int i=0;i<nameList.size();i++){
  std::string searchname=nameList[i];
  bool foundDet=false;
  for(int j=0;j<knownDet.size();j++){
   std::string foundname=knownDet[j].readname();
   if(foundname==searchname){
    if(foundDet){
     myfile << "Warning! Detector " << searchname << " seems to be defined twice!" << "\n";
    }
    if(!foundDet){
     myList.push_back(knownDet[j]);
     foundDet=true;
    }
   }
  }
  if(foundDet){
   myfile << "Detector " << searchname << " was found." << "\n";
  }
  if(!foundDet){
   myfile << "Warning! Detector " << searchname << " does not seem to be declared!" << "\n";
  }
 }
   myfile.close();
 
 return myList;
}

