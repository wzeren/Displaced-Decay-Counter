#ifndef _DMATHUSLA2_
#define _DMATHUSLA2_

#include "HepMC/IO_GenEvent.h"
#include "include/CDetector.h"

Detector MATHUSLA2();

bool MATHUSLA2Cuts(HepMC::GenEvent* evtInput);

#endif
