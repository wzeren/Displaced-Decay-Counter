#include "include/Detectors/DELIGHTC.h"
#include <cmath>
#include <algorithm>
#include <vector>

//2306.11803  
//DELIGHT-C
//   25.0 m < x < 225.0 m
//    0.0 m < y < 50.0 m
//  -25.0 m < z < 25.0 m

Detector DELIGHTC() {

    double z_min = -25.0;
    double z_max =  25.0;


    double x_min_overall = 25.0;
    double x_max_overall = 225.0;
    double y_min_overall = 0.0;
    double y_max_overall = 50.0;

    std::vector<CylDetLayer> delightLayers;

    // Tiling in x and y 
    int num_x_slices = 100; // number of slices in x
    int num_y_slices = 25; // number of slices in y

    double x_dim = x_max_overall - x_min_overall; // 25 m
    double y_dim = y_max_overall - y_min_overall; // 100 m

    double delta_x_slice = x_dim / static_cast<double>(num_x_slices);
    double delta_y_slice = y_dim / static_cast<double>(num_y_slices);

    for (int i = 0; i < num_x_slices; ++i) {
        for (int j = 0; j < num_y_slices; ++j) {
            double current_x_min = x_min_overall + i * delta_x_slice;
            double current_x_max = x_min_overall + (i + 1) * delta_x_slice;
            double current_y_min = y_min_overall + j * delta_y_slice;
            double current_y_max = y_min_overall + (j + 1) * delta_y_slice;

            // Calculate radial distances (h) for the corners of the current sub-rectangle
            double h_c1 = std::sqrt(current_x_min * current_x_min + current_y_min * current_y_min);
            double h_c2 = std::sqrt(current_x_max * current_x_max + current_y_min * current_y_min);
            double h_c3 = std::sqrt(current_x_max * current_x_max + current_y_max * current_y_max);
            double h_c4 = std::sqrt(current_x_min * current_x_min + current_y_max * current_y_max);

            double h_min_slice = std::min({h_c1, h_c2, h_c3, h_c4});
            double h_max_slice = std::max({h_c1, h_c2, h_c3, h_c4});

            // Calculate azimuthal angles (phi) for the corners of the current sub-rectangle
            double phi_c1 = std::atan2(current_y_min, current_x_min);
            double phi_c2 = std::atan2(current_y_min, current_x_max);
            double phi_c3 = std::atan2(current_y_max, current_x_max);
            double phi_c4 = std::atan2(current_y_max, current_x_min);

            // Ensure angles are in a continuous interval (handle possible wrap-around)
            // Convert all to range [0, 2*pi) to avoid issues when min/max crosses -pi/pi.
            auto norm_phi = [](double a) {
                double res = a;
                if (res < 0) res += 2.0 * M_PI;
                return res;
            };
            double nphi1 = norm_phi(phi_c1);
            double nphi2 = norm_phi(phi_c2);
            double nphi3 = norm_phi(phi_c3);
            double nphi4 = norm_phi(phi_c4);

            // Find min/max in normalized space, but also compute smallest angular span properly.
            double min_phi_slice = std::min({nphi1, nphi2, nphi3, nphi4});
            double max_phi_slice = std::max({nphi1, nphi2, nphi3, nphi4});
            double delta_phi_slice = max_phi_slice - min_phi_slice;

            // However, if the points straddle the 2*pi -> 0 boundary, the direct min/max approach
            // may overestimate the span. So compute alternative span across the boundary:
            double alt_span = (min_phi_slice + 2.0 * M_PI) - max_phi_slice;
            if (alt_span < delta_phi_slice) {
                // use the wrap-around span
                delta_phi_slice = alt_span;
            }

            // Weight is fractional azimuthal coverage
            double angular_weight_slice = delta_phi_slice / (2.0 * M_PI);

            // Build z-h corner points for the CylDetLayer (ordered AA,BB,CC,DD)
            std::array<double,2> AA_slice = { z_min, h_min_slice };
            std::array<double,2> BB_slice = { z_max, h_min_slice };
            std::array<double,2> CC_slice = { z_max, h_max_slice };
            std::array<double,2> DD_slice = { z_min, h_max_slice };

            std::vector<std::array<double,2>> ptlist_slice = { AA_slice, BB_slice, CC_slice, DD_slice };

            CylDetLayer sliceLay(ptlist_slice, angular_weight_slice);
            delightLayers.push_back(sliceLay);
        }
    }

    std::string Dname = "DELIGHTC";


    double DLumi = 30000.0; //30 ab^{-1} int lumi

    Detector myDELIGHT(Dname, DLumi, delightLayers);
    return myDELIGHT;
}


bool DELIGHTCCuts(HepMC::GenEvent* evtin) {
    bool cuts = true;

    // Example: keep all events by default; user can add LLP-specific kinematic/decay-volume cuts here.
    // for (HepMC::GenEvent::particle_const_iterator p = evtin->particles_begin();
    //      p != evtin->particles_end(); ++p) {
    //     if ((*p)->pdg_id() == <LLP_PDG_ID>) {
    //         // Example kinematic cut: require LLP energy > 100 GeV
    //         if ((*p)->momentum().e() < 100.0) {
    //             cuts = false;
    //             break;
    //         }
    //     }
    // }
    return cuts;
}

