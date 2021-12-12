#ifndef _DETECTORS_
#define _DETECTORS_


#include "include/CCylSeg.h"
#include "include/CCylDetLayer.h"
#include "include/CDetector.h"

CylDetLayer CylBrick(std::array<double,2> coord, double length, double height, double apphi, double wgh);

Detector MATHUSLA0();
Detector MATHUSLA1();
Detector MATHUSLA2();
Detector FASER1();
Detector FASER2();
Detector ANUBIS0();
Detector ANUBIS1();
Detector AL3X();
Detector CODEXB0();


#endif
