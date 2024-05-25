#ifndef _DCODEXB0_
#define _DCODEXB0_

#include "HepMC/IO_GenEvent.h"
#include "include/CDetector.h"

Detector CODEXB0();

bool CODEXB0Cuts(HepMC::GenEvent* evtInput);

#endif
