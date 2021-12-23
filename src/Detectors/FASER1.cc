#include "include/Detectors/FASER1.h"

    // FASER
    
Detector FASER1() {
 std::array<double,2> AA={478.5,0.},BB={480.,0.},CC={480.,0.1},DD={478.5,0.1};       // Corner points
 std::vector<std::array<double,2>> ptlist={AA,BB,CC,DD};
 double faserweight=1.;           // angular coverage
 CylDetLayer faserlay(ptlist,faserweight);
 std::vector<CylDetLayer> faserLayers={faserlay};
 std::string Dname="FASER";
 double DLumi=150.;
 Detector myFASER(Dname,DLumi,faserLayers);
 return myFASER;
}
