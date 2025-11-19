/*! \file
 * Definition of the CreateDetectors function, giving access to the selected detectors.
*/
#include "include/detectors.h"

/*! \fn CreateDetectors
 * The CreateDetectors function looks for the detectors from a list (vector) of
    identifiers (strings) and returns a vector of detectors that can be tested
    against MC events.
   It first declares all known detectors, adding them to the knownDet vector.
    Then it looks at the inputed 'wish-list', compares its elements to the identifiers
    of the known detectors and builds the desired list.
   A log file is copied to bin/Detectors/Detector_log.txt , 
    detailing the success or problems in establishing the list.
*/

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

 // Building MATHUSLA40
 Detector MATHUSLA40X=MATHUSLA40();
 knownDet.push_back(MATHUSLA40X);

 // Building ANUBISceiling
 Detector ANUBISceilingX=ANUBISceiling();
 knownDet.push_back(ANUBISceilingX);

 // Building BelleII
 Detector BelleIIX=BelleII();
 knownDet.push_back(BelleIIX);

 // Building BelleIIBabyGAZELLE
 Detector BelleIIBabyGAZELLEX=BelleIIBabyGAZELLE();
 knownDet.push_back(BelleIIBabyGAZELLEX);

 // Building BelleIIGODZILLA
 Detector BelleIIGODZILLAX=BelleIIGODZILLA();
 knownDet.push_back(BelleIIGODZILLAX);

 // Building BelleIILGAZELLEB1
 Detector BelleIILGAZELLEB1X=BelleIILGAZELLEB1();
 knownDet.push_back(BelleIILGAZELLEB1X);

 // Building BelleIILGAZELLEB2
 Detector BelleIILGAZELLEB2X=BelleIILGAZELLEB2();
 knownDet.push_back(BelleIILGAZELLEB2X);

 // Building SHiPhsds
 Detector SHiPhsdsX=SHiPhsds();
 knownDet.push_back(SHiPhsdsX);

 // Building FOREHUNT
 Detector FOREHUNTX=FOREHUNT();
 knownDet.push_back(FOREHUNTX);

// BUILDING THE LIST OF STUDIED DETECTORS
   std::vector<std::array<double,3>> TDcoord;
   TDcoord.clear(); 
   std::ofstream myfile;
  myfile.open ("Logs/Detector_log.txt");
  myfile << "***************************************************************" << "\n";
  myfile << "***************** WELCOME TO THE DETECTOR LOG *****************" << "\n";
  myfile << "***************************************************************" << "\n";
  myfile << "\n";
  myfile << "Checking the availability of detectors..." << "\n";
  myfile << "\n";
 
 for(int i=0;i<nameList.size();i++){
  std::string searchname=nameList[i];
  bool foundDet=false;
  myfile << "***************************************************************" << "\n";
  myfile << "	Requested detector:	" << searchname << "\n";
  myfile << "***************************************************************" << "\n";
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
  myfile << "\n";
 }
  myfile << "***************************************************************" << "\n";
  myfile << "\n";
  myfile << "***************************************************************" << "\n";
  myfile << "******************* END OF THE DETECTOR LOG *******************" << "\n";
  myfile << "***************************************************************" << "\n";
  myfile.close();
 
 return myList;
}

