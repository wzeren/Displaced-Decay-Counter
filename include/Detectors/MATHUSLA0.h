#ifndef _DMATHUSLA0_
#define _DMATHUSLA0_

#include "HepMC/IO_GenEvent.h"
#include "include/CDetector.h"

Detector MATHUSLA0();

bool MATHUSLA0Cuts(HepMC::GenEvent* evtInput);

#endif
