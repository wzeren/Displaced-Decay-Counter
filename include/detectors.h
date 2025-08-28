#ifndef _DETECTORS_
#define _DETECTORS_

#include "include/Detectors/MATHUSLA0.h"
#include "include/Detectors/MATHUSLA1.h"
#include "include/Detectors/MATHUSLA2.h"
#include "include/Detectors/FASER1.h"
#include "include/Detectors/FASER2.h"
#include "include/Detectors/AL3X.h"
#include "include/Detectors/CODEXB0.h"
#include "include/Detectors/CODEXB1.h"
#include "include/Detectors/ANUBIS0.h"
#include "include/Detectors/ANUBIS1.h"
#include "include/Detectors/MAPP1.h"
#include "include/Detectors/MAPP2.h"
#include "include/Detectors/FACET.h"
#include "include/Detectors/MATHUSLA40.h"
#include "include/Detectors/ANUBISceiling.h"

// END OF INCLUDE DEFINITIONS

/*! \brief
 * The CreateDetectors function looks for the detectors from a list (vector) of identifiers
    (strings) and returns a vector of detectors that can be tested against MC events.
 */

std::vector<Detector> CreateDetectors(std::vector<std::string>);

#endif
