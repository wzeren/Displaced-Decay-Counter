#include "include/Detectors/ANUBIS1.h"

// Building ANUBIS from 1m-high, 1m-deep bricks

Detector ANUBIS1() {
  double ymin = 24., ymax = 80., xcnt = 0., zcnt = 14., Rad = 9., dphi = 0.00392699, dl = 1., dh = 1.5;
  std::vector<CylDetLayer> AnubisBricks;
  AnubisBricks.clear();

  //************* PRINT details of the construction in a file
  //   std::vector<std::array<double,3>> TDcoord;
  //   TDcoord.clear();
  //   std::ofstream myfile;
  //   myfile.open ("Detectors/ANUBIS1_log.txt");
  //*************

  int hct0 = 44, zct0 = 20, phct0 = 200;
  for (int hct = 0; hct < hct0; hct++) {
    double hcoord = 24. + hct * dh;
    for (int zct = 0; zct < zct0; zct++) {
      double zcoord = 4. + dl * zct;
      int count = 0;
      for (int phct = 0; phct < phct0; phct++) {
        double phcoord = 3. / 2. * atan(1.) + phct * dphi;
        double xcoord = -hcoord * cos(phcoord), ycoord = hcoord * sin(phcoord);
        double distcnt = sqrt((zcoord - zcnt) * (zcoord - zcnt) + (xcoord - xcnt) * (xcoord - xcnt));
        if (ycoord >= ymin && ycoord <= ymax && distcnt < Rad) {
          count = count + 1;
        }
      }
      if (count != 0) {
        std::array<double, 2> brkcoord = {zcoord, hcoord};

        //*************
        //     std::array<double,3> testcoord={zcoord,hcoord,1.*count};
        //     TDcoord.push_back(testcoord);
        //*************

        CylDetLayer newbrick = CylBrick(brkcoord, dl, dh, count * dphi, 1.);
        AnubisBricks.push_back(newbrick);
      }
    }
  }

  //*************
  //   myfile << "coord: " << TDcoord.size() << " , bricks: " << AnubisBricks.size() << "\n";
  //   for(int i=0;i<TDcoord.size();i++){
  //    myfile << "z: " << TDcoord[i][0] << " , h: " << TDcoord[i][1] << " , count: " << TDcoord[i][2] << "\n";
  //   }
  //   myfile.close();
  //*************

  std::string Dname = "ANUBIS1";
  double DLumi = 3000.;
  Detector myANUBIS(Dname, DLumi, AnubisBricks);
  return myANUBIS;
}

bool ANUBIS1Cuts(HepMC::GenEvent* evtin) {
  bool cuts = true;
  return cuts;
};
