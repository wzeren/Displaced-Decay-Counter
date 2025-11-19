#ifndef _DDELIGHTC_
#define _DDELIGHTC_

#include "include/CDetector.h"
#include "HepMC/IO_GenEvent.h"

Detector DELIGHTC();

bool DELIGHTCCuts(HepMC::GenEvent* evtInput);

#endif
