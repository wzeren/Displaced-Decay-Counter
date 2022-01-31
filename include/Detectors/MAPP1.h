#ifndef _DMAPP1_
#define _DMAPP1_

#include "include/CDetector.h"
#include "HepMC/IO_GenEvent.h"

Detector MAPP1();

bool MAPP1Cuts(HepMC::GenEvent* evtInput);

#endif
