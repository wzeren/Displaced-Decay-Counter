#ifndef _DFASER2_
#define _DFASER2_

#include "include/CDetector.h"
#include "HepMC/IO_GenEvent.h"

Detector FASER2();

bool FASER2Cuts(HepMC::GenEvent* evtInput);

#endif
