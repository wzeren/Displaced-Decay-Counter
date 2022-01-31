#ifndef _DFASER1_
#define _DFASER1_

#include "include/CDetector.h"
#include "HepMC/IO_GenEvent.h"

Detector FASER1();

bool FASER1Cuts(HepMC::GenEvent* evtInput);

#endif
