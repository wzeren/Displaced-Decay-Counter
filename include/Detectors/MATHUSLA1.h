#ifndef _DMATHUSLA1_
#define _DMATHUSLA1_

#include "include/CDetector.h"
#include "HepMC/IO_GenEvent.h"

Detector MATHUSLA1();

bool MATHUSLA1Cuts(HepMC::GenEvent* evtInput);

#endif
