#include "include/Detectors/ANUBISceiling.h"
//anubis ceiling, l=53, rin=11.3 and rout=19.9, dx=28.7
Detector ANUBISceiling() {
 double length=53;
 double rin=11.3;
 double rout=19.9;
 double dx=28.7;
 std::array<double,2> AA={-length/2,rin},BB={length/2,rin},CC={length/2,rout},DD={-length/2,rout};           // Corner points
 std::vector<std::array<double,2>> ptlist={AA,BB,CC,DD};
 double anubisceilingweight=0.198864;        // angular coverage   = (ArcTan[(dx/2)/rout]*2)/(2 \[Pi])
 CylDetLayer anubisceilinglay(ptlist,anubisceilingweight);
 std::vector<CylDetLayer> anubisceilingLayers={anubisceilinglay};
 std::string Dname="ANUBISceiling";
 double DLumi=3000.;
 Detector myANUBISceiling(Dname,DLumi,anubisceilingLayers );
 return myANUBISceiling;
}

bool ANUBISceilingCuts(HepMC::GenEvent* evtin){
 bool cuts=true;
 return cuts;
};
