#include "include/Detectors/CODEXB0.h"

    // Uncle Simon's CODEXB
    
Detector CODEXB0() {
 std::array<double,2> AA={5.,26.},BB={15.,26.},CC={15.,36.},DD={5.,36.};       // Corner points
 std::vector<std::array<double,2>> ptlist={AA,BB,CC,DD};
 double codexweight=2.*atan(5./31.)/(8.*atan(1.));            // Angular aperture
 CylDetLayer codexlay(ptlist,codexweight);
 std::vector<CylDetLayer> codexLayers={codexlay};
 std::string Dname="CODEXB0";
 double DLumi=300.;
 Detector myCODEXB(Dname,DLumi,codexLayers);
 return myCODEXB;
}

bool CODEXB0Cuts(HepMC::GenEvent* evtin){
 bool cuts=true;
 return cuts;
};
