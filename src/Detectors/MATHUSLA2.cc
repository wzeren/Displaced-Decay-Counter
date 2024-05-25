#include "include/Detectors/MATHUSLA2.h"

// Building MATHUSLA from 3m-high bricks

Detector MATHUSLA2() {
  double xmin = -50., xmax = 50., ymin = 60., ymax = 85., dh = 3., dphi = 0.00785398, zcoord = 118., dl = 100.;
  std::vector<CylDetLayer> MathuBricks;
  MathuBricks.clear();

  //************* PRINT details of the construction in a file
  //   std::vector<std::array<double,2>> TDcoord;
  //   TDcoord.clear();
  //   std::ofstream myfile;
  //   myfile.open ("Detectors/MATHUSLA2_log.txt");
  //*************

  int hct0 = 25, phct0 = 200;
  for (int hct = 0; hct < hct0; hct++) {
    int count = 0;
    double hcoord = 40. + hct * dh;
    for (int phct = 0; phct < phct0; phct++) {
      double phcoord = atan(1.) + phct * dphi;
      double xcoord = -hcoord * cos(phcoord), ycoord = hcoord * sin(phcoord);
      if (ycoord >= ymin && ycoord <= ymax && xcoord >= xmin && xcoord <= xmax) {
        count = count + 1;
      }
    }
    if (count != 0) {
      std::array<double, 2> brkcoord = {zcoord, hcoord};

      //*************
      //   TDcoord.push_back(brkcoord);
      //*************

      CylDetLayer newbrick = CylBrick(brkcoord, dl, dh, count * dphi, 1.);
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
  std::string Dname = "MATHUSLA2";
  double DLumi = 3000.;
  Detector myMATHUSLA(Dname, DLumi, MathuBricks);
  return myMATHUSLA;
}

bool MATHUSLA2Cuts(HepMC::GenEvent* evtin) {
  bool cuts = true;
  return cuts;
};
