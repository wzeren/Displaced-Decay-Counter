#ifndef _DMAPP2_
#define _DMAPP2_

#include "HepMC/IO_GenEvent.h"
#include "include/CDetector.h"

Detector MAPP2();

bool MAPP2Cuts(HepMC::GenEvent* evtInput);

#endif
