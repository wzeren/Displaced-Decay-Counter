#include "include/Detectors/FCChhFT.h"

Detector FCChhFT() { 
 std::array<double,2> AA={10,20./1000.},BB={16.,20./1000.},CC={16.,1.6},DD={10.,1.6};       // Corner points
 std::vector<std::array<double,2>> ptlist={AA,BB,CC,DD};
 double fcchhftweight=1.;           // angular coverage
 CylDetLayer fcchhftlay(ptlist,fcchhftweight);
 std::vector<CylDetLayer> myDetLayers={fcchhftlay};
 std::string Dname="FCChhFT";
 double DLumi=20000;//20/ab
 Detector myDetector(Dname,DLumi,myDetLayers);
 return myDetector;
}

bool FCChhFTCuts(HepMC::GenEvent* evtin){
 bool cuts=true;
 return cuts;
};
