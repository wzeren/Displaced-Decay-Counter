#include "include/Detectors/BelleIIBabyGAZELLE.h"
#include <cmath> // Required for M_PI or other math functions if used
#include <algorithm> // Required for std::min, std::max on initializer lists

// Dimensions: 4m x 4m x 4m cube.
// Position: x approx 10m, y approx -3.7m, z approx 10m.
// Integrated Luminosity: 50 ab^-1.
Detector BelleIIBabyGAZELLE() {
    // Determine the z-coordinates (along the beam axis)
    // The detector is 4m long, starting at z approx 10m.
    double z_min = 10.0; // Starting z-coordinate
    double z_max = 14.0; // Ending z-coordinate (10.0 + 4.0)

    // Determine the radial coordinates (h) from the x-y dimensions and position
    // The detector is a 4m x 4m cube in the x-y plane, centered at (x=10m, y=-3.7m).
    // This implies an x-range of [10-2, 10+2] = [8m, 12m]
    // And a y-range of [-3.7-2, -3.7+2] = [-5.7m, -1.7m]
    // We calculate h_min and h_max by finding the minimum and maximum radial distances
    // from the origin (0,0) to the corners of this x-y rectangle.
    // Corners are (8, -5.7), (12, -5.7), (12, -1.7), (8, -1.7).
    double h1 = std::sqrt(8.0*8.0 + (-5.7)*(-5.7)); // ~9.82 m
    double h2 = std::sqrt(12.0*12.0 + (-5.7)*(-5.7)); // ~13.28 m
    double h3 = std::sqrt(12.0*12.0 + (-1.7)*(-1.7)); // ~12.12 m
    double h4 = std::sqrt(8.0*8.0 + (-1.7)*(-1.7)); // ~8.18 m

    double h_min = std::min({h1, h2, h3, h4}); // Smallest radial distance
    double h_max = std::max({h1, h2, h3, h4}); // Largest radial distance

    // Define the corner points in (z, h) for the CylDetLayer.
    // These points form the rectangular cross-section in the cylindrical (z,h) plane.
    std::array<double,2> AA={z_min, h_min};
    std::array<double,2> BB={z_max, h_min};
    std::array<double,2> CC={z_max, h_max};
    std::array<double,2> DD={z_min, h_max};

    std::vector<std::array<double,2>> ptlist={AA,BB,CC,DD};

    // Calculate the angular aperture (azimuthal coverage)
    // We determine the minimum and maximum azimuthal angles (phi) spanned by the
    // x-y projection of the detector, using atan2(y, x).
    // Angles for the corners of the x-y projection:
    double phi1 = std::atan2(-5.7, 8.0); // Angle for (8, -5.7) ~ -0.617 rad
    double phi2 = std::atan2(-5.7, 12.0); // Angle for (12, -5.7) ~ -0.443 rad
    double phi3 = std::atan2(-1.7, 12.0); // Angle for (12, -1.7) ~ -0.140 rad
    double phi4 = std::atan2(-1.7, 8.0); // Angle for (8, -1.7) ~ -0.209 rad

    double min_phi = std::min({phi1, phi2, phi3, phi4});
    double max_phi = std::max({phi1, phi2, phi3, phi4});

    // The total angular span in radians.
    double delta_phi = max_phi - min_phi;

    // The weight for CylDetLayer is typically Delta_phi / (2 * PI)
    // M_PI is from <cmath> in C++ or can be defined as 3.14159265358979323846
    double babyGazelleWeight = delta_phi / (2.0 * M_PI); 

    // Create the Cylindrical Detector Layer
    CylDetLayer babyGazelleLay(ptlist, babyGazelleWeight);
    std::vector<CylDetLayer> babyGazelleLayers={babyGazelleLay};

    // Set the detector name and integrated luminosity
    std::string Dname="BelleIIBabyGAZELLE"; // Updated name
    // Integrated luminosity for GAZELLE is 50 ab^-1, which is 50,000 fb^-1
    double DLumi=50000.0;

    // Construct and return the Detector object
    Detector myBabyGAZELLE(Dname,DLumi,babyGazelleLayers);
    return myBabyGAZELLE;
}

bool BelleIIBabyGAZELLECuts(HepMC::GenEvent* evtin){
 bool cuts=true;
    // Example of a possible cut:
    // if (evtin->event_scale() < 100.0) { // Example: Cut on event scale
    //     cuts = false;
    // }
    // Loop through particles in the event to apply cuts based on LLP properties
    // for (HepMC::GenEvent::particle_const_iterator p = evtin->particles_begin(); p != evtin->particles_end(); ++p) {
    //     if ((*p)->pdg_id() == <LLP_PDG_ID>) { // Replace <LLP_PDG_ID> with the actual PDG ID of your LLP
    //         if ((*p)->momentum().e() < 50.0) { // Example: Cut on LLP energy
    //             cuts = false;
    //             break; 
    //         }
    //     }
    // }
 return cuts;
};
