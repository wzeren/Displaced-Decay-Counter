#include "include/Detectors/BelleIILGAZELLEB1.h"
#include <cmath>                // Required for M_PI and std::atan2, std::sqrt, std::min, std::max
#include <algorithm>            // Required for std::min, std::max on initializer lists

// Detector for Large Gazelle-B1 (LGB1) at SuperKEKB
// Dimensions: 6m x 16m x 24m cube.
// Position: x approx 35m, y approx 2.3m, z approx 0m.
// Integrated Luminosity: 50 ab^-1.
Detector BelleIILGAZELLEB1() {
    // Determine the z-coordinates (along the beam axis)
    // The detector is 24m long, centered at z approx 0m.
    double z_min = -12.0; // Starting z-coordinate (0.0 - 24.0 / 2)
    double z_max = 12.0;  // Ending z-coordinate (0.0 + 24.0 / 2)

    // Determine the radial coordinates (h) from the x-y dimensions and position.
    // The detector is a 6m x 16m rectangle in the x-y plane.
    // Position: x approx 35m, y approx 2.3m.
    // Assuming 6m along x-axis, 16m along y-axis based on typical rectangular definition.
    double x_center = 35.0;
    double y_center = 2.3;
    double x_half_dim = 6.0 / 2.0;  // 3m
    double y_half_dim = 16.0 / 2.0; // 8m

    // Calculate the x and y ranges
    double x_min_cart = x_center - x_half_dim; // 35.0 - 3.0 = 32.0m
    double x_max_cart = x_center + x_half_dim; // 35.0 + 3.0 = 38.0m
    double y_min_cart = y_center - y_half_dim; // 2.3 - 8.0 = -5.7m
    double y_max_cart = y_center + y_half_dim; // 2.3 + 8.0 = 10.3m

    // Now, find the radial distances (h) for the four corners of this Cartesian rectangle:
    // (x_min_cart, y_min_cart), (x_max_cart, y_min_cart), (x_max_cart, y_max_cart), (x_min_cart, y_max_cart)
    double h_corner1 = std::sqrt(x_min_cart * x_min_cart + y_min_cart * y_min_cart); // sqrt(32^2 + (-5.7)^2)
    double h_corner2 = std::sqrt(x_max_cart * x_max_cart + y_min_cart * y_min_cart); // sqrt(38^2 + (-5.7)^2)
    double h_corner3 = std::sqrt(x_max_cart * x_max_cart + y_max_cart * y_max_cart); // sqrt(38^2 + 10.3^2)
    double h_corner4 = std::sqrt(x_min_cart * x_min_cart + y_max_cart * y_max_cart); // sqrt(32^2 + 10.3^2)

    double h_min = std::min({h_corner1, h_corner2, h_corner3, h_corner4}); // Smallest radial distance
    double h_max = std::max({h_corner1, h_corner2, h_corner3, h_corner4}); // Largest radial distance

    // Define the corner points in (z, h) for the CylDetLayer.
    // These points form the rectangular cross-section in the cylindrical (z,h) plane.
    std::array<double,2> AA={z_min, h_min};
    std::array<double,2> BB={z_max, h_min};
    std::array<double,2> CC={z_max, h_max};
    std::array<double,2> DD={z_min, h_max};

    std::vector<std::array<double,2>> ptlist={AA,BB,CC,DD};

    // Calculate the angular aperture (azimuthal coverage).
    // We determine the minimum and maximum azimuthal angles (phi) spanned by the
    // x-y projection of the detector, using atan2(y, x).
    double phi_corner1 = std::atan2(y_min_cart, x_min_cart);
    double phi_corner2 = std::atan2(y_min_cart, x_max_cart);
    double phi_corner3 = std::atan2(y_max_cart, x_max_cart);
    double phi_corner4 = std::atan2(y_max_cart, x_min_cart);

    double min_phi = std::min({phi_corner1, phi_corner2, phi_corner3, phi_corner4});
    double max_phi = std::max({phi_corner1, phi_corner2, phi_corner3, phi_corner4});

    // The total angular span in radians.
    double delta_phi = max_phi - min_phi;

    // The weight for CylDetLayer is typically Delta_phi / (2 * PI)
    // M_PI is from <cmath> in C++ or can be defined as 3.14159265358979323846
    double lgb1Weight = delta_phi / (2.0 * M_PI);

    // Create the Cylindrical Detector Layer
    CylDetLayer lgb1Lay(ptlist, lgb1Weight);
    std::vector<CylDetLayer> lgb1Layers={lgb1Lay};

    // Set the detector name and integrated luminosity
    std::string Dname="BelleIILGAZELLEB1";
    // Integrated luminosity for GAZELLE is 50 ab^-1, which is 50,000 fb^-1
    double DLumi=50000.0;

    // Construct and return the Detector object
    Detector myLGB1(Dname,DLumi,lgb1Layers);
    return myLGB1;
}

bool BelleIILGAZELLEB1Cuts(HepMC::GenEvent* evtin){
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
