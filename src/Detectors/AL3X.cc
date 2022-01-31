#include "include/Detectors/AL3X.h"

    // AL3X
    
Detector AL3X() {
 std::array<double,2> AA={5.25,0.85},BB={17.25,0.85},CC={17.25,5.},DD={5.25,5.};       // Corner points
 std::vector<std::array<double,2>> ptlist={AA,BB,CC,DD};
 double alexweight=1.;                                   // angular coverage
 CylDetLayer alexlay(ptlist,alexweight);
 std::vector<CylDetLayer> alexLayers={alexlay};
 std::string Dname="AL3X";
 double DLumi=250.;
 Detector myAL3X(Dname,DLumi,alexLayers);
 return myAL3X;
}

bool AL3XCuts(HepMC::GenEvent* evtin){
 bool cuts=true;
 return cuts;
};
