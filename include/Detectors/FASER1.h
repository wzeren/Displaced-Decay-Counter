#ifndef _DFASER1_
#define _DFASER1_

#include "HepMC/IO_GenEvent.h"
#include "include/CDetector.h"

Detector FASER1();

bool FASER1Cuts(HepMC::GenEvent* evtInput);

#endif
