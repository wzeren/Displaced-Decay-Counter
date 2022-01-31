#include "include/Detectors/ANUBIS0.h"

    // Uncle Simon's ANUBIS
    
Detector ANUBIS0() {
 // First layer
 std::array<double,2> AA={5.,24.},BB={23.,24.},CC={23.,42.667},DD={5.,42.667};
 std::vector<std::array<double,2>> ptlist={AA,BB,CC,DD};
 double anubisweight=2.*atan(9./33.333)/(8.*atan(1.));
 CylDetLayer anubislay1(ptlist,anubisweight);
 std::vector<CylDetLayer> anubisLayers={anubislay1};
 // Second layer
 AA={5.,42.667},BB={23.,42.667},CC={23.,61.333},DD={5.,61.333};
 ptlist={AA,BB,CC,DD};
 anubisweight=2.*atan(9./52.)/(8.*atan(1.));
 CylDetLayer anubislay2(ptlist,anubisweight);
 anubisLayers.push_back(anubislay2);
 // Third layer
 AA={5.,61.333},BB={23.,61.333},CC={23.,80.},DD={5.,80.};
 ptlist={AA,BB,CC,DD};
 anubisweight=2.*atan(9./70.667)/(8.*atan(1.));
 CylDetLayer anubislay3(ptlist,anubisweight);
 anubisLayers.push_back(anubislay3);
 std::string Dname="ANUBIS0";
 double DLumi=3000.;
 Detector myANUBIS(Dname,DLumi,anubisLayers);
 return myANUBIS;
}

bool ANUBIS0Cuts(HepMC::GenEvent* evtin){
 bool cuts=true;
 return cuts;
};
