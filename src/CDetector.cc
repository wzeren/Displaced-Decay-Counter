#include "include/CDetector.h"


Detector::Detector(std::string myDet,double myLumi,std::vector<CylDetLayer> LayList) {  // direct constructor
  Detname=myDet;
  DetLumi=myLumi;
  CylLayList=LayList;
}

double Detector::DetAcc(double th,double leff) {  // sums decay probabilities from the listed cyl. layers
  double Pdec=0.;
  if(CylLayList.size()>0 && th>=0 && th<=4.*atan(1.) && leff>0){
   for(int i=0; i<CylLayList.size(); i++) {
    Pdec=Pdec+CylLayList[i].inDetDec(th,leff);
   }
   Pdec=Pdec;
  }
  return Pdec;
}

std::string Detector::readname() {
 return Detname;
}

double Detector::readLumi() {
 return DetLumi;
}
