#ifndef _DMATHUSLA1_
#define _DMATHUSLA1_

#include "HepMC/IO_GenEvent.h"
#include "include/CDetector.h"

Detector MATHUSLA1();

bool MATHUSLA1Cuts(HepMC::GenEvent* evtInput);

#endif
