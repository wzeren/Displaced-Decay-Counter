#ifndef _DMATHUSLA40_
#define _DMATHUSLA40_

#include "include/CDetector.h"
#include "HepMC/IO_GenEvent.h"

Detector MATHUSLA40();

bool MATHUSLA40Cuts(HepMC::GenEvent* evtInput);

#endif
