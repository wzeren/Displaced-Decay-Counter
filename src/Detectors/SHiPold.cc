#include "include/Detectors/SHiPold.h"
#include <cmath>

// SHiPold: old SHiP hidden-sector decay spectrometer design
// implemented so as to reproduce the old conservative acceptance logic.
//
// Old design text:
//   active decay chamber starts at z = 68.8 m downstream of target
//   total chamber length = 60 m
//   first 5 m reserved for vetoes
// => active decay region: 68.8 m < z < 123.8 m
//
// Front surface: ellipse with semi-axes 5.0 m and 2.5 m
//
// The LLP trajectory must remain inside the vessel all the way to the
// downstream end of the active volume.
// For straight-line trajectories from the target, this is equivalent to
// requiring that, at any decay position z, the decay point lies inside a
// z-dependent ellipse obtained by scaling the downstream ellipse by z/z_end.
//
// Therefore, the accepted decay volume is NOT the full cylinder, but the
// truncated elliptical cone:
//   x^2 / a(z)^2 + y^2 / b(z)^2 <= 1
// with
//   a(z) = 5.0 * z / 123.8
//   b(z) = 2.5 * z / 123.8

Detector SHiPold() {
    double zmin = 68.8;
    double dl   = 55.0;      // active decay length only
    double dz   = 1.0;       // slice thickness in z
    double zend = zmin + dl; // 123.8 m

    // downstream ellipse semi-axes
    double a_end = 5.0;
    double b_end = 2.5;

    // brickization params
    double dh   = 0.1;
    int phct0   = 800;
    double dphi = 2.0 * M_PI / phct0;

    std::vector<CylDetLayer> bricks;
    bricks.clear();

    int nslices = int(dl / dz);
    for (int iz = 0; iz < nslices; iz++) {
        double zlow = zmin + iz * dz;
        double zhigh = zlow + dz;
        double zmid = 0.5 * (zlow + zhigh);

        // z-dependent ellipse reproducing the old conservative angular cut
        double a = a_end * zmid / zend;
        double b = b_end * zmid / zend;

        double rmax = a * 1.1; // safety factor
        int hct0 = static_cast<int>(rmax / dh) + 1;

        for (int hct = 0; hct < hct0; hct++) {
            double hcoord = (hct + 0.5) * dh;
            int count = 0;

            for (int phct = 0; phct < phct0; phct++) {
                double phi = phct * dphi;
                double x = hcoord * std::cos(phi);
                double y = hcoord * std::sin(phi);

                if ((x * x) / (a * a) + (y * y) / (b * b) <= 1.0) {
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

    std::string Dname = "SHiPold";
    double DLumi = 4.6e6;
    Detector mySHiPold(Dname, DLumi, bricks);
    return mySHiPold;
}

bool SHiPoldCuts(HepMC::GenEvent* evtin){
    bool cuts = true;
    return cuts;
}


/* estimation of Dlumi, same convention as SHiPhsds.cc

15 years with 6e20 POTs.

L = NPOT times n_target.
n_target = NA rho L_target / A
NA = 6.022e23   #/mol, Avogadro's number
rho = 10.22 g/cm^3, considering molybdenum in TZM alloy
L_target = 1.2 m = 120 cm
A = 95.95 g/mol

So n_target = 7.697e23 nuclei/cm^2
So L = 4.6e45 cm^{-2} = 4.618e6 /fb
*/
