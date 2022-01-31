#ifndef _DMATHUSLA0_
#define _DMATHUSLA0_

#include "include/CDetector.h"
#include "HepMC/IO_GenEvent.h"

Detector MATHUSLA0();

bool MATHUSLA0Cuts(HepMC::GenEvent* evtInput);

#endif
