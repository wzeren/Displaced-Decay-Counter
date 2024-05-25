#include "include/Detectors/FACET.h"

Detector FACET() {
  std::array<double, 2> AA = {101., 0.1}, BB = {119., 0.1}, CC = {119., 0.5}, DD = {101., 0.5};  // Corner points
  std::vector<std::array<double, 2>> ptlist = {AA, BB, CC, DD};
  double facetweight = 1.;  // angular coverage
  CylDetLayer facetlay(ptlist, facetweight);
  std::vector<CylDetLayer> myDetLayers = {facetlay};
  std::string Dname = "FACET";
  double DLumi = 3000.;
  Detector myDetector(Dname, DLumi, myDetLayers);
  return myDetector;
}

bool FACETCuts(HepMC::GenEvent* evtin) {
  bool cuts = true;
  return cuts;
};
