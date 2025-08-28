#include "include/Detectors/MATHUSLA40.h"

	//zmin = 70, zmax = 70 + 40 = 110    xmin=-20  xmax=20   ymin=81   ymax=81+11=92
    // Building MATHUSLA40 from dh-m-high bricks
    
Detector MATHUSLA40() {
 double xmin=-20., xmax=20., ymin=81., ymax=92., dh=1., dphi=0.00785398, zcoord=90., dl=40.;//0.00785398 = (pi/2)/ 200
 std::vector<CylDetLayer> MathuBricks;
 MathuBricks.clear();
 
 //************* PRINT details of the construction in a file
 //   std::vector<std::array<double,2>> TDcoord;
 //   TDcoord.clear(); 
 //   std::ofstream myfile;
 //   myfile.open ("Detectors/MATHUSLA40_log.txt");
 //************* 
 
 int hct0=75, phct0=200;
 for(int hct=0; hct<hct0; hct++){
  int count=0;
  double hcoord=40.+hct*dh;
  for(int phct=0; phct<phct0; phct++){
   double phcoord=atan(1.)+phct*dphi;
   double xcoord=-hcoord*cos(phcoord), ycoord=hcoord*sin(phcoord);   
   if(ycoord>=ymin && ycoord<=ymax && xcoord>=xmin && xcoord<=xmax){
    count=count+1;
   }
  }
  if(count!=0){
   std::array<double,2> brkcoord={zcoord,hcoord};
   
 //************* 
 //   TDcoord.push_back(brkcoord);
 //************* 
 
   CylDetLayer newbrick=CylBrick(brkcoord,dl,dh,count*dphi,1.);
   MathuBricks.push_back(newbrick);
  }
 }
 
 //************* 
 //   myfile << "coord: " << TDcoord.size() << " , bricks: " << MathuBricks.size() << "\n";
 //   for(int i=0;i<TDcoord.size();i++){
 //    myfile << "z: " << TDcoord[i][0] << " , h: " << TDcoord[i][1] << "\n";
 //   }
 //   myfile.close();
 //************* 
 std::string Dname="MATHUSLA40";
 double DLumi=3000.;
 Detector myMATHUSLA40(Dname,DLumi,MathuBricks);
 return myMATHUSLA40;
}

bool MATHUSLA40Cuts(HepMC::GenEvent* evtin){
 bool cuts=true;
 return cuts;
};
