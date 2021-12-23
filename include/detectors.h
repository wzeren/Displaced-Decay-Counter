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

std::vector<Detector> CreateDetectors(std::vector<std::string>);

#endif
