#include "include/Detectors/MATHUSLA0.h"

// Uncle Simon's MATHUSLA

Detector MATHUSLA0() {
  std::array<double, 2> AA = {68., 60.}, BB = {168., 60.}, CC = {168., 85.}, DD = {68., 85.};  // Corner points
  std::vector<std::array<double, 2>> ptlist = {AA, BB, CC, DD};
  // CylSeg mathusl1(AA,BB,1),mathusl2(BB,CC,-1),mathusl3(CC,DD,-1),mathusl4(DD,AA,1);
  // std::vector<CylSeg> mathuslist={mathusl1,mathusl2,mathusl3,mathusl4};
  double mathusweight = 2. * atan(50. / 60.) / (8. * atan(1.));  // Angular aperture
                                                                 //    CylDetLayer mathuslay(mathuslist,mathusweight);g
  CylDetLayer mathuslay(ptlist, mathusweight);
  std::vector<CylDetLayer> MathuLayers = {mathuslay};
  std::string Dname = "MATHUSLA0";
  double DLumi = 3000.;
  Detector myMATHUSLA(Dname, DLumi, MathuLayers);
  return myMATHUSLA;
}

bool MATHUSLA0Cuts(HepMC::GenEvent* evtin) {
  bool cuts = true;
  for (auto p = evtin->particles_begin(); p != evtin->particles_end(); ++p) {
    /*
    //apply trivial cut
    if( (*p)->momentum().perp() > -10. ){
    std::cout << (*p)->pdg_id() << std::endl;
      return true;
    }

    */
  }

  return cuts;
};
