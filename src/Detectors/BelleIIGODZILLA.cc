#include "include/Detectors/BelleIIGODZILLA.h"
#include <cmath> 
#include <algorithm> 
#include <vector> 

// Detector for GODZILLA at SuperKEKB
// Dimensions: 25m x 10m x 50m cube.
// Position: x approx -27m, y approx 18m, z approx 20m.
// Integrated Luminosity: 50 ab^-1.

Detector BelleIIGODZILLA() {
    // The detector is 50m long, centered at z approx 20m.
    double z_center = 20.0;
    double z_half_dim = 50.0 / 2.0; // 25.0m
    double z_min = z_center - z_half_dim; // 20.0 - 25.0 = -5.0m
    double z_max = z_center + z_half_dim; // 20.0 + 25.0 = 45.0m

    // The Cartesian x-y dimensions and position for the overall detector.
    double x_center = -27.0;
    double y_center = 18.0;
    double x_dim = 25.0;
    double y_dim = 10.0;

    double x_min_overall = x_center - x_dim / 2.0; // -27.0 - 12.5 = -39.5m
    double x_max_overall = x_center + x_dim / 2.0; // -27.0 + 12.5 = -14.5m
    double y_min_overall = y_center - y_dim / 2.0; // 18.0 - 5.0 = 13.0m
    double y_max_overall = y_center + y_dim / 2.0; // 18.0 + 5.0 = 23.0m

    std::vector<CylDetLayer> godzillaLayers;

    // Using 5 divisions along x and 2 divisions along y, resulting in 10 layers.
    int num_x_slices = 5;
    int num_y_slices = 2;

    double delta_x_slice = x_dim / num_x_slices;
    double delta_y_slice = y_dim / num_y_slices;

    for (int i = 0; i < num_x_slices; ++i) {
        for (int j = 0; j < num_y_slices; ++j) {
            double current_x_min = x_min_overall + i * delta_x_slice;
            double current_x_max = x_min_overall + (i + 1) * delta_x_slice;
            double current_y_min = y_min_overall + j * delta_y_slice;
            double current_y_max = y_min_overall + (j + 1) * delta_y_slice;

            // Radial distances (h) for the corners of the current sub-rectangle
            double h_c1 = std::sqrt(current_x_min * current_x_min + current_y_min * current_y_min);
            double h_c2 = std::sqrt(current_x_max * current_x_max + current_y_min * current_y_min);
            double h_c3 = std::sqrt(current_x_max * current_x_max + current_y_max * current_y_max);
            double h_c4 = std::sqrt(current_x_min * current_x_min + current_y_max * current_y_max);

            double h_min_slice = std::min({h_c1, h_c2, h_c3, h_c4});
            double h_max_slice = std::max({h_c1, h_c2, h_c3, h_c4});

            // Azimuthal angles (phi) for the corners of the current sub-rectangle
            double phi_c1 = std::atan2(current_y_min, current_x_min);
            double phi_c2 = std::atan2(current_y_min, current_x_max);
            double phi_c3 = std::atan2(current_y_max, current_x_max);
            double phi_c4 = std::atan2(current_y_max, current_x_min);

            double min_phi_slice = std::min({phi_c1, phi_c2, phi_c3, phi_c4});
            double max_phi_slice = std::max({phi_c1, phi_c2, phi_c3, phi_c4});

            // The total angular span in radians for this slice.
            double delta_phi_slice = max_phi_slice - min_phi_slice;

            // The weight for CylDetLayer is typically Delta_phi / (2 * PI)
            double angular_weight_slice = delta_phi_slice / (2.0 * M_PI);

            // The corner points in (z, h) for the CylDetLayer for this slice.
            std::array<double,2> AA_slice={z_min, h_min_slice};
            std::array<double,2> BB_slice={z_max, h_min_slice};
            std::array<double,2> CC_slice={z_max, h_max_slice};
            std::array<double,2> DD_slice={z_min, h_max_slice};

            std::vector<std::array<double,2>> ptlist_slice={AA_slice,BB_slice,CC_slice,DD_slice};

            CylDetLayer sliceLay(ptlist_slice, angular_weight_slice);
            godzillaLayers.push_back(sliceLay);
        }
    }

    std::string Dname="BelleIIGODZILLA";
    // Integrated luminosity for GAZELLE is 50 ab^-1, which is 50,000 fb^-1
    double DLumi=50000.0;

    Detector myGODZILLA(Dname,DLumi,godzillaLayers);
    return myGODZILLA;
    // Example of a possible cut:
    // if (evtin->event_scale() < 100.0) { 
    //     cuts = false;
    // }
    // for (HepMC::GenEvent::particle_const_iterator p = evtin->particles_begin(); p != evtin->particles_end(); ++p) {
    //     if ((*p)->pdg_id() == <LLP_PDG_ID>) { 
    //         if ((*p)->momentum().e() < 50.0) { 
    //             cuts = false;
    //             break;
    //         }
    //     }
    // }
}

bool BelleIIGODZILLACuts(HepMC::GenEvent* evtin){
 bool cuts=true;
 return cuts;
};
