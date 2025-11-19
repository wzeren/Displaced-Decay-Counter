#include "include/Detectors/FOREHUNT.h"
//2306.11803  
//we take FOREHUNT-C as an example which gives the max signal acceptance out of the six detector configuratons for all the benchmark points
//R = 5 m, Length Ld = 50 m,  distance of the near end from the IP = 50 m



Detector FOREHUNT() {
 std::array<double,2> AA={50.,0.},BB={100.,0.},CC={100.,5.},DD={50.,5.};           // Corner points
 std::vector<std::array<double,2>> ptlist={AA,BB,CC,DD};
 double forehuntweight=1.;          // angular coverage
 CylDetLayer forehuntlay(ptlist,forehuntweight);
 std::vector<CylDetLayer> forehuntLayers={forehuntlay};
 std::string Dname="FOREHUNT";
 double DLumi=30000.;  //30 ab^{-1} int lumi
 Detector myFOREHUNT(Dname,DLumi,forehuntLayers);
 return myFOREHUNT;
}

bool FOREHUNTCuts(HepMC::GenEvent* evtin){
 bool cuts=true;
 return cuts;
};


