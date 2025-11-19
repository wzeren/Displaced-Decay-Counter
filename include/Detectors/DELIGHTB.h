#ifndef _DDELIGHTB_
#define _DDELIGHTB_

#include "include/CDetector.h"
#include "HepMC/IO_GenEvent.h"

Detector DELIGHTB();

bool DELIGHTBCuts(HepMC::GenEvent* evtInput);

#endif
