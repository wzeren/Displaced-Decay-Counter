#include "include/Detectors/SHiPhsds.h"
#include <cmath>

// SHiPhsds: pyramidal frustum
//https://cds.cern.ch/record/2878604/files/SPSC-P-369.pdf
//SHiP Hidden Sector Decay Spectrometer
// zmin = 33.0 m, length = 50 m
// upstream: 1.0 × 2.7 m², downstream: 4.0 × 6.0 m²

Detector SHiPhsds() {
    double zmin = 33.0;
    double dl   = 50.0;       // total length
    double dz   = 1.0;        // slice thickness in z

    // dimensions
    double wx_u = 1.0, wy_u = 2.7;
    double wx_d = 4.0, wy_d = 6.0;

    // brickization params
    double dh   = 0.1;                // radial thickness
    int phct0 = 800;
    double dphi = 2*M_PI / phct0;// ~0.00785 rad (like in MATHUSLA40)

    std::vector<CylDetLayer> bricks;
    bricks.clear();

    int nslices = int(dl / dz);//50/1 = 50
    for (int iz = 0; iz < nslices; iz++) {
        double zlow = zmin + iz * dz;
        double zhigh = zlow + dz;
        double zmid = 0.5 * (zlow + zhigh);

        // linear interpolation of rectangle width/height at zmid
        double wx = wx_u + (wx_d - wx_u) * (zmid - zmin) / dl;
        double wy = wy_u + (wy_d - wy_u) * (zmid - zmin) / dl;

        // maximum radius at this slice
        double rmax = std::sqrt(std::pow(wx/2.0,2) + std::pow(wy/2.0,2)) *1.1; //having 1.1 is a good practice, though it does not affect our result at all for the present geometry
        int hct0 = static_cast<int>(rmax / dh) + 1;
        
        // loop over radial shells
        for (int hct = 0; hct < hct0; hct++) {
            double hcoord = (hct + 0.5) * dh;
            int count = 0;

            // loop over phi
            for (int phct = 0; phct < phct0; phct++) {
                double phi = phct * dphi;
                double x = hcoord * cos(phi);
                double y = hcoord * sin(phi);

                if (std::abs(x) <= wx / 2.0 && std::abs(y) <= wy / 2.0) {
                    count++;
                }
            }

            if (count > 0) {
                std::array<double,2> brkcoord = {zmid, hcoord};
                CylDetLayer newbrick = CylBrick(brkcoord, dz, dh, count * dphi, 1.0);
                bricks.push_back(newbrick);
            }
        }
    }

    std::string Dname = "SHiPhsds";
    double DLumi = 4.6e6;
    Detector mySHiPhsds(Dname, DLumi, bricks);
    return mySHiPhsds;
}

bool SHiPhsdsCuts(HepMC::GenEvent* evtin){
    bool cuts = true;
    return cuts;
};



/*estimation of Dlumi, the effective int lumi of SHiP

15 years with 6e20 POTs.

L = NPOT times   n_target.
n_target = NA  rho  L_target / A
NA = 6.022e23   #/mol, Avogadrgo's number
rho = 10.22g/cm^3, since the target consists of TZM alloy   (considering molybdenum)
L_target = 1.2 m = 120 cm.  this is becasue 12 nuclear interaction length for a 400 GeV proton beam is roughly 1.2 m
A = 95.95 g/mol.   (considering molybdenum)
So n_target = 7.697e3 nuclei/cm^2
So L = 4.6e45 cm^{-2} = 4.618e6 /fb
*/
