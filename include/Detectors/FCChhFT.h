#ifndef _DFCChhFT_
#define _DFCChhFT_

#include "include/CDetector.h"
#include "HepMC/IO_GenEvent.h"

Detector FCChhFT();

bool FCChhFTCuts(HepMC::GenEvent* evtInput);

#endif
