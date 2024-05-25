#include "include/Detectors/MAPP1.h"

Detector MAPP1() {
  //************* PRINT details of the construction in a file
  //   std::vector<std::array<double,3>> TDcoord;
  //   TDcoord.clear();
  //   std::ofstream myfile;
  //   myfile.open ("Detectors/MAPP1_log.txt");
  //*************
  std::vector<CylDetLayer> myDetLayers;
  myDetLayers.clear();
  double dh = 0.2, dphi = 0.00785398, dl = 0.2;
  int hct0 = 25, phct0 = 120, zct0 = 80;
  for (int zct = 0; zct < zct0; zct++) {
    double zcoord = -63. + dl * zct;
    for (int hct = 0; hct < hct0; hct++) {
      int count = 0;
      double hcoord = 3. + hct * dh;
      for (int phct = 0; phct < phct0; phct++) {
        double phcoord = 1. + phct * dphi;
        double xcoord = -hcoord * cos(phcoord), ycoord = hcoord * sin(phcoord);
        if (-0.813502 * ycoord - 0.0692948 * zcoord <= 1. && 0.0351491 * ycoord - 0.0167562 * zcoord >= 1. &&
            0.922095 * ycoord + 0.0827521 * zcoord <= 1. && 0.029519 * ycoord - 0.0143659 * zcoord <= 1. &&
            xcoord >= -2. && xcoord <= 1.) {
          count = count + 1;
          //*************
          //     std::array<double,3> testcoord={zcoord,xcoord,ycoord};
          //     TDcoord.push_back(testcoord);
          //*************
        }
      }
      if (count != 0) {
        std::array<double, 2> brkcoord = {zcoord, hcoord};
        CylDetLayer newbrick = CylBrick(brkcoord, dl, dh, count * dphi, 1.);
        myDetLayers.push_back(newbrick);
      }
    }
  }
  //*************
  //   myfile << "coord: " << TDcoord.size() << " , bricks: " << myDetLayers.size() << "\n";
  //   for(int i=0;i<TDcoord.size();i++){
  //    myfile << "z: " << TDcoord[i][0] << " , x: " << TDcoord[i][1] << " , y: " << TDcoord[i][2] << "\n";
  //   }
  //   myfile.close();
  //*************
  std::string Dname = "MAPP1";
  double DLumi = 30.;
  Detector myDetector(Dname, DLumi, myDetLayers);
  return myDetector;
}

bool MAPP1Cuts(HepMC::GenEvent* evtin) {
  bool cuts = true;
  return cuts;
};
