#include "include/Detectors/FCChhCT.h"

Detector FCChhCT() {
    // Define corner points
    std::array<double, 2> AA = {-5, 20./1000.}, BB = {5, 20./1000.}, CC = {5, 1.7}, DD = {-5, 1.7};  // Corner points
    std::vector<std::array<double, 2>> ptlist = {AA, BB, CC, DD};  // List of points defining the detector

    double fcchhctweight = 1.0;  // Angular coverage

    // Create a cylindrical detector layer
    CylDetLayer fcchhctlay(ptlist, fcchhctweight);
    std::vector<CylDetLayer> fcchhctLayers = {fcchhctlay};  // Vector of detector layers

    std::string Dname="FCChhCT";
    double DLumi=20000;//20/ab

    // Create and return the detector
    Detector myDetector(Dname, DLumi, fcchhctLayers);
    return myDetector;
}

bool FCChhCTCuts(HepMC::GenEvent* evtin){
 bool cuts=true;
 return cuts;
};
