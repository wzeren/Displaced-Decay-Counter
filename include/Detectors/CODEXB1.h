#ifndef _DCODEXB1_
#define _DCODEXB1_

#include "HepMC/IO_GenEvent.h"
#include "include/CDetector.h"

Detector CODEXB1();

bool CODEXB1Cuts(HepMC::GenEvent* evtInput);

#endif
