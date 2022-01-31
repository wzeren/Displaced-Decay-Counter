#ifndef _DMATHUSLA2_
#define _DMATHUSLA2_

#include "include/CDetector.h"
#include "HepMC/IO_GenEvent.h"

Detector MATHUSLA2();

bool MATHUSLA2Cuts(HepMC::GenEvent* evtInput);

#endif
