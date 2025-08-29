#include "include/Detectors/BelleII.h"

//Belle II main detector
//We take the fiducial volume to be -40cm < z < 120 cm and 10 cm < r < 80 cm
//The user can freely adjust these numbers according to their need

Detector BelleII() {
    // Define corner points
    std::array<double, 2> AA = {-0.4, 0.1}, BB = {1.2, 0.1}, CC = {1.2, 0.8}, DD = {-0.4, 0.8};  // Corner points
    std::vector<std::array<double, 2>> ptlist = {AA, BB, CC, DD};  // List of points defining the detector

    double belleIIweight = 1.0;  // Angular coverage

    // Create a cylindrical detector layer
    CylDetLayer belleIIlay(ptlist, belleIIweight);
    std::vector<CylDetLayer> belleIILayers = {belleIIlay};  // Vector of detector layers

    std::string Dname = "BelleII";  // Detector name
    double DLumi = 50.0e3;  // Integrated luminosity in fb^-1 (converted from ab^-1)

    // Create and return the detector
    Detector myBelleII(Dname, DLumi, belleIILayers);
    return myBelleII;
}

bool BelleIICuts(HepMC::GenEvent* evtin){
 bool cuts=true;
 return cuts;
};
