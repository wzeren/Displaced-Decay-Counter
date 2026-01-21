#ifndef _DFCChhCT_
#define _DFCChhCT_

#include "include/CDetector.h"
#include "HepMC/IO_GenEvent.h"

Detector FCChhCT();

bool FCChhCTCuts(HepMC::GenEvent* evtInput);

#endif
