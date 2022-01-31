#ifndef _DCODEXB0_
#define _DCODEXB0_

#include "include/CDetector.h"
#include "HepMC/IO_GenEvent.h"

Detector CODEXB0();

bool CODEXB0Cuts(HepMC::GenEvent* evtInput);

#endif
