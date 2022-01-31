#include "include/Detectors/CODEXB1.h"

    // Improved CODEXB
    
Detector CODEXB1() {
 // First layer
 std::array<double,2> AA={5.,26.051358},BB={5.,36.0711111},CC={15.,36.0711111},DD={15.,26.051358};
 std::vector<std::array<double,2>> ptlist={AA,BB,CC,DD};
 double codexweight=0.17766086/(8.*atan(1.));            // Angular aperture
 CylDetLayer codexlay1(ptlist,codexweight);
 std::vector<CylDetLayer> ICodexLayers={codexlay1};
 // Second layer
 AA={5.,26.118785},BB={5.,31.56222},CC={15.,31.56222},DD={15.,26.118785};
 ptlist={AA,BB,CC,DD};
 codexweight=0.0127311073/(8.*atan(1.));                 // Angular aperture
 CylDetLayer codexlay2(ptlist,codexweight);
 ICodexLayers.push_back(codexlay2);
 // Third layer
 AA={5.,26.146760},BB={5.,28.3784804},CC={15.,28.3784804},DD={15.,26.146760};
 ptlist={AA,BB,CC,DD};
 codexweight=0.008702541/(8.*atan(1.));                  // Angular aperture
 CylDetLayer codexlay3(ptlist,codexweight);
 ICodexLayers.push_back(codexlay3);
 // Fourth layer
 AA={5.,26.2479356},BB={5.,36.3432954},CC={15.,36.3432954},DD={15.,26.2479356};
 ptlist={AA,BB,CC,DD};
 codexweight=0.084295998/(8.*atan(1.));                  // Angular aperture
 CylDetLayer codexlay4(ptlist,codexweight);
 ICodexLayers.push_back(codexlay4);
 // Fifth layer
 AA={5.,26.455781},BB={5.,36.6310814},CC={15.,36.6310814},DD={15.,26.455781};
 ptlist={AA,BB,CC,DD};
 codexweight=0.024716089/(8.*atan(1.));                  // Angular aperture
 CylDetLayer codexlay5(ptlist,codexweight);
 ICodexLayers.push_back(codexlay5);
 // Sixth layer
 AA={5.,26.549617},BB={5.,34.596021},CC={15.,34.596021},DD={15.,26.549617};
 ptlist={AA,BB,CC,DD};
 codexweight=0.011806297/(8.*atan(1.));                  // Angular aperture
 CylDetLayer codexlay6(ptlist,codexweight);
 ICodexLayers.push_back(codexlay6);
 // Seventh layer
 AA={5.,26.618576},BB={5.,32.674020},CC={15.,32.674020},DD={15.,26.618576};
 ptlist={AA,BB,CC,DD};
 codexweight=0.012533236/(8.*atan(1.));                  // Angular aperture
 CylDetLayer codexlay7(ptlist,codexweight);
 ICodexLayers.push_back(codexlay7);
 // Eighth layer
 AA={5.,26.696529},BB={5.,30.858797},CC={15.,30.858797},DD={15.,26.696529};
 ptlist={AA,BB,CC,DD};
 codexweight=0.0133093/(8.*atan(1.));                    // Angular aperture
 CylDetLayer codexlay8(ptlist,codexweight);
 ICodexLayers.push_back(codexlay8);
 // Nineth layer
 AA={5.,26.784744},BB={5.,29.144419},CC={15.,29.144419},DD={15.,26.784744};
 ptlist={AA,BB,CC,DD};
 codexweight=0.01413877/(8.*atan(1.));                   // Angular aperture
 CylDetLayer codexlay9(ptlist,codexweight);
 ICodexLayers.push_back(codexlay9);
 // Tenth layer
 AA={5.,26.863778},BB={5.,27.832842},CC={15.,27.832842},DD={15.,26.863778};
 ptlist={AA,BB,CC,DD};
 codexweight=0.00920773/(8.*atan(1.));                   // Angular aperture
 CylDetLayer codexlay10(ptlist,codexweight);
 ICodexLayers.push_back(codexlay10);
 std::string Dname="CODEXB1";
 double DLumi=300.;
 Detector myCODEXB(Dname,DLumi,ICodexLayers);
 return myCODEXB;
}

bool CODEXB1Cuts(HepMC::GenEvent* evtin){
 bool cuts=true;
 return cuts;
};
