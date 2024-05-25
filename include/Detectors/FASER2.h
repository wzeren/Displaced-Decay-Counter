#ifndef _DFASER2_
#define _DFASER2_

#include "HepMC/IO_GenEvent.h"
#include "include/CDetector.h"

Detector FASER2();

bool FASER2Cuts(HepMC::GenEvent* evtInput);

#endif
