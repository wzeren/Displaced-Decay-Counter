#include "include/Detectors/FASER2.h"

// FASER2

Detector FASER2() {
  std::array<double, 2> AA = {475., 0.}, BB = {480., 0.}, CC = {480., 1.}, DD = {475., 1.};  // Corner points
  std::vector<std::array<double, 2>> ptlist = {AA, BB, CC, DD};
  double faserweight = 1.;  // angular coverage
  CylDetLayer faserlay(ptlist, faserweight);
  std::vector<CylDetLayer> faserLayers = {faserlay};
  std::string Dname = "FASER2";
  double DLumi = 3000.;
  Detector myFASER(Dname, DLumi, faserLayers);
  return myFASER;
}

bool FASER2Cuts(HepMC::GenEvent* evtin) {
  bool cuts = true;
  return cuts;
};
