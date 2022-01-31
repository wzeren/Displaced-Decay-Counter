#ifndef _DMAPP2_
#define _DMAPP2_

#include "include/CDetector.h"
#include "HepMC/IO_GenEvent.h"

Detector MAPP2();

bool MAPP2Cuts(HepMC::GenEvent* evtInput);

#endif
