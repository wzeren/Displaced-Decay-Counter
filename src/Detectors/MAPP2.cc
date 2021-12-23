#include "include/Detectors/MAPP2.h"

Detector MAPP2() {
 std::vector<CylDetLayer> myDetLayers;
 myDetLayers.clear();
 double dh=0.2, dphi=0.00785398, dl=0.5;
 int hct0=70, phct0=120, zct0=60;
 for(int zct=0; zct<zct0; zct++){
  double zcoord=-63.+dl*zct;
  for(int hct=0; hct<hct0; hct++){
   int count=0;
   double hcoord=3.+hct*dh;
    for(int phct=0; phct<phct0; phct++){
    double phcoord=1.+phct*dphi;
    double xcoord=-hcoord*cos(phcoord), ycoord=hcoord*sin(phcoord);   
    if(-0.809479*ycoord-0.0690327*zcoord<=1. && 
      0.0357762*ycoord-0.0167142*zcoord>=1. && 
      -0.0149185*ycoord-0.0351651*zcoord>=1. && 
      0.0297144*ycoord-0.0143532*zcoord<=1. && 
      xcoord>=-2. && xcoord<=1.){
     count=count+1;
    }
   }
   if(count!=0){
    std::array<double,2> brkcoord={zcoord,hcoord};
    CylDetLayer newbrick=CylBrick(brkcoord,dl,dh,count*dphi,1.);
    myDetLayers.push_back(newbrick);
   }
  }
 }
 std::string Dname="MAPP2";
 double DLumi=300.;
 Detector myDetector(Dname,DLumi,myDetLayers);
 return myDetector;
}
