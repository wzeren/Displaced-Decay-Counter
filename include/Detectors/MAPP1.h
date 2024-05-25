#ifndef _DMAPP1_
#define _DMAPP1_

#include "HepMC/IO_GenEvent.h"
#include "include/CDetector.h"

Detector MAPP1();

bool MAPP1Cuts(HepMC::GenEvent* evtInput);

#endif
