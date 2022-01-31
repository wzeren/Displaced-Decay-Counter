#ifndef _DCODEXB1_
#define _DCODEXB1_

#include "include/CDetector.h"
#include "HepMC/IO_GenEvent.h"

Detector CODEXB1();

bool CODEXB1Cuts(HepMC::GenEvent* evtInput);

#endif
